#pragma once
#include <QTcpSocket>
#include <QMutex>
#include <QMutexLocker>
#include <QTimer>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

enum MessageType {
	Error = 0,
	Authorization,
	AuthorizationReply,
	ESPStatus,
	RelaySwitch,
	ConsumersData,
	SensorsData,
	Shutdown
};

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
	void authorized(const QJsonObject&);
	void consumersReceived(const QJsonObject&);
	void ESPStatusReceived(const QJsonObject&);
	void dataReceived(const QJsonObject&);
	void disconnected();

private slots:
	void readFromSocket();
	void onConnectionTimeout();

private:
	QTcpSocket* socket;

	QTimer* connectionTimeout;

	QHostAddress mAddr;
	int mPort;
};


