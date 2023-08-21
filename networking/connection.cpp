#include "networking/connection.h"

#include <QJsonArray>
#include <QFile>

Networker::Networker(QObject* parent, const QString& address, int port) : QObject{parent}, mHost{address}, mPort{port} {
	socket = new QTcpSocket{this};

	connect(socket, SIGNAL(readyRead()), this, SLOT(readFromSocket()));
	connect(socket, &QTcpSocket::disconnected, socket, [this]() {
		emit disconnected();
	});
}

void Networker::setHostAddress(const QString& addr) {
	mHost = addr;
}

void Networker::setHostPort(int port) {
	mPort = port;
}

void Networker::connectToHost() {
	QHostAddress addr{mHost};

	if (addr.isNull()) {
		emit serverLookupFailed();
		return;
	}

	socket->connectToHost(addr, mPort);
	if (!socket->waitForConnected(3000)) {
		emit unableToConnect();
		return;
	}

	emit connected();
	return;
}

void Networker::sendToHost(const QString& data) {
	socket->write(data.toStdString().c_str(), data.length());
	socket->flush();
}

void Networker::readFromSocket() {
	QString line = socket->readLine();
	QJsonObject inputObject = QJsonDocument::fromJson(line.toUtf8()).object();

	emit dataReceived(inputObject);
}

Networker::~Networker() {
	blockSignals(true);
	socket->close();
}
