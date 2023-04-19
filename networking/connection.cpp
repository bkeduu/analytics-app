#include "networking/connection.h"

#include <QJsonArray>

Networker::Networker(QObject* parent, QHostAddress address, int port) : QObject{parent}, mAddr{address}, mPort{port} {
	socket = new QTcpSocket{this};
	connect(socket, SIGNAL(readyRead()), this, SLOT(readFromSocket()));

	connectionTimeout = new QTimer{this};

	connectionTimeout->setInterval(15000);
	connectionTimeout->start();

	connect(connectionTimeout, SIGNAL(timeout()), this, SLOT(onConnectionTimeout()));
}

void Networker::setHostAddress(QHostAddress addr) {
	mAddr = addr;
}

void Networker::setHostPort(int port) {
	mPort = port;
}

void Networker::connectToHost() {
	socket->connectToHost(mAddr, mPort);
	socket->waitForConnected();
}

void Networker::sendToHost(const QString& data) {
	socket->write(data.toStdString().c_str());
}

void Networker::readFromSocket() {
	connectionTimeout->start();

	QString line = socket->readLine();

	QJsonDocument input = QJsonDocument::fromJson(line.toUtf8());
	QJsonObject inputObject = input.object();

	MessageType messageType = MessageType(inputObject.value("type").toInt(-1));

	QJsonArray dataObject = inputObject.value("data").toArray();

	switch (messageType) {
	case AuthorizationReply:
		emit authorized(dataObject);
		break;
	case ESPStatus:
		emit ESPStatusReceived(dataObject);
		break;
	case RelaySwitch:
		// wtf?
		break;
	case ConsumersData:
		emit consumersReceived(dataObject);
		break;
	case SensorsData:
		emit dataReceived(dataObject);
		break;
	case Shutdown:
		emit disconnected();
		break;
	default:
		// wtf?
		break;
	}
}

void Networker::onConnectionTimeout() {

}

Networker::~Networker() {
	socket->close();
}

