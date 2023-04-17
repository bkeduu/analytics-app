#include "networking/connection.h"

Networker::Networker(QObject* parent, QHostAddress address, int port) : QObject{parent}, mAddr{address}, mPort{port} {
	socket = new QTcpSocket{this};
	connect(socket, SIGNAL(readyRead()), this, SLOT(readFromSocket()));
}

void Networker::setHostAddress(QHostAddress addr) {
	mAddr = addr;
}

void Networker::setHostPort(int port) {
	mPort = port;
}

void Networker::connectToHost() {
	socket->connectToHost(mAddr, mPort);
}

void Networker::sendToHost(const QString& data) {
	socket->write(data.toStdString().c_str());
	socket->waitForBytesWritten();
}

void Networker::readFromSocket() {



}

Networker::~Networker() {
	socket->close();
}

