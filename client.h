#pragma once

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

signals:
	void authorized();
	void ESPStatusChanged();
	void relaySwitched();
	void consumersData();
	void sensorsData();
	void shutdown();

public slots:
	void onDataReceived(const QJsonObject& data);
	void sendData();

private:
	bool mAuthorized;
	bool mESPConnected;

	MainWindow mWindow;
	QSharedPointer<Networker> mNetworker;

	QSettings mSettings;
};

