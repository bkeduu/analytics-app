#pragma once
#include <QTcpSocket>
#include <QMutex>
#include <QMutexLocker>

class Networker : public QObject {
Q_OBJECT

public:
	Networker(QObject* parent, QHostAddress address = QHostAddress::Any, int port = 7777);

	void setHostAddress(QHostAddress addr);
	void setHostPort(int port);
	void connectToHost();

	QAbstractSocket::SocketState state() const { return socket->state(); }

	void sendToHost(const QString& data);

	~Networker();

signals:
	void connected();
	void authorized();
	void consumersArrived();
	void dataArrived();
	void disconnected();

private slots:
	void readFromSocket();

private:
	QTcpSocket* socket;
	QHostAddress mAddr;
	int mPort;
};


