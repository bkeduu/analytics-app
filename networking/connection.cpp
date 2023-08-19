#include "networking/connection.h"

#include <QJsonArray>
#include <QFile>

Networker::Networker(QObject* parent, const QString& address, int port) : QObject{parent}, mHost{address}, mPort{port} {
	socket = new QTcpSocket{this};

	connect(socket, SIGNAL(readyRead()), this, SLOT(readFromSocket()));
	connect(socket, SLOT(disconnected()), this, SIGNAL(disconnected()));
}

void Networker::setHostAddress(const QString& addr) {
	mHost = addr;
}

void Networker::setHostPort(int port) {
	mPort = port;
}

void Networker::connectToHost() {
	QHostInfo lookupResult = QHostInfo::fromName(mHost);
	QList<QHostAddress> addresses = lookupResult.addresses();
	if (lookupResult.error() != QHostInfo::NoError) {
		emit serverLookupFailed();
		return;
	}

	for (auto it = addresses.begin(); it != addresses.end(); ++it) {
		if (!((*it).isNull()) && (*it).protocol() == QAbstractSocket::IPv4Protocol) {
			socket->connectToHost(*it, mPort);
			if (!socket->waitForConnected(3000)) {
				emit unableToConnect();
				return;
			}

			emit connected();
			return;
		}
	}
}

void Networker::sendToHost(const QString& data) {
	socket->write(data.toStdString().c_str());
}

void Networker::readFromSocket() {
	QString line = socket->readLine();
	QJsonObject inputObject = QJsonDocument::fromJson(line.toUtf8()).object();

	emit dataReceived(inputObject);
}

Networker::~Networker() {
	QFile file{":/static/requests/shutdown.json"};
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	sendToHost(QString::fromStdString(file.readAll().toStdString()));
	socket->close();
}
