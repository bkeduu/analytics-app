#include "networking/connection.h"
#include "etc/utils.h"

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
	socket->write(data.toUtf8());
	socket->flush();
}

void Networker::readFromSocket() {
	QString line = socket->readLine();
	QJsonDocument document = QJsonDocument::fromJson(line.toUtf8());

	if (document.isNull())
		throw InternalErrorException{tr("Data structure with wrong value received at %1. The app will be closed.").arg(FLF)};

	emit dataReceived(document.object());
}

Networker::~Networker() {
	blockSignals(true);
	socket->close();
}
