#pragma once

#include "utils.h"
#include "connection.h"
#include "mainwindow.h"

#include <QSharedPointer>

enum MessageType {
	AuthorizationSuccess = 0,
	AuthorizationError,
	Authorization,
	SensorsData,
	ConsumersData,
	RelaySwitch,
	ModeSwitch,
	GraphicsData,
	ESPStatus,
	Shutdown
};

enum ModeType {
	Manual = 0,
	MaximumReliability,
	MaximumPower,
	MaximumEcology,
	MaximumEconomy
};

class Client final : public QObject {
	Q_OBJECT

public:
	Client(QObject *parent = nullptr);
	virtual ~Client() final override;

	MainWindow& getMainWindow() { return mWindow; }

signals:
	void serverLookupFailed();
	void connected();
	void unableToConnect();
	void disconnected();

	void authorized(bool status);
	void ESPConnectionChange(bool status);
	void relaySwitched();
	void consumersData(QJsonObject data);
	void sensorsData(QJsonObject data);
	void shutdown();

public slots:
	void onConnected();
	void onDisconnected();
	void onDataReceived(const QJsonObject& data);
	void sendAuth(const QString& login, const QString& password, const QString& serverAddress, int serverPort);
	void onModeChange(int);
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
