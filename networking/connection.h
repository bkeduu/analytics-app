#pragma once
#include <QTcpSocket>
#include <QMutex>
#include <QMutexLocker>
#include <QTimer>
#include <QHostInfo>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

class Networker final : public QObject {
Q_OBJECT

public:
	Networker(QObject* parent, const QString& address = "", int port = 7777);

	void setHostAddress(const QString& addr);
	void setHostPort(int port);
	void connectToHost();
	QAbstractSocket::SocketState state() const { return socket->state(); }

	void sendToHost(const QString& data);
	void close();

	virtual ~Networker() final override;

signals:
	void connected();
	void dataReceived(const QJsonObject&);
	void disconnected();
	void unableToConnect();
	void serverLookupFailed();

private slots:
	void readFromSocket();

private:
	QTcpSocket* socket;

	QHostAddress mAddr;
	QString mHost;
	int mPort;
};


