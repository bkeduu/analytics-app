#pragma once

#include "utils.h"
#include "connection.h"
#include "mainwindow.h"

#include <QSharedPointer>

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

class Client final : public QObject {
	Q_OBJECT

public:
	Client(QObject *parent = nullptr);
	virtual ~Client() final override;

	MainWindow& getMainWindow() { return mWindow; }

signals:
	void serverLookupFailed();

	void authorized(bool status);
	void ESPConnected();
	void ESPDisconnected();
	void relaySwitched();
	void consumersData();
	void sensorsData(QJsonObject);
	void shutdown();

public slots:
	void onConnected() { mServerConnected = true; }
	void onDisconnected() { mServerConnected = false; }
	void onDataReceived(const QJsonObject& data);
	void sendAuth(const QString& login, const QString& password, const QString& serverAddress, int serverPort);
	void onRelayClicked(int group, bool newState);
	void onUnableToConnect();
	void onServerLookupFailed();

private:
	void sendData(const QString& request) const;

	bool mAuthorized;
	bool mServerConnected;
	bool mESPConnected;

	MainWindow mWindow;
	QSharedPointer<Networker> mNetworker;

	QSettings mSettings;
};
