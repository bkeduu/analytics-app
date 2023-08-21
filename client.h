#pragma once

#include "utils.h"
#include "connection.h"
#include "mainwindow.h"

#include <QSharedPointer>

enum MessageType {
	AuthorizationSuccess = 0, //Ответ от сервера на клиента и есп, если авторизация успешна
	AuthorizationError, //Ответ от сервера на клиента и есп, если авторизация не успешна
	Authorization, //Запрос на авторизацию от клиента на сервер и есп на сервер
	SensorsData, //Показатели силы тока и напряжения на esp (сервер отправляет запрос на есп, получает ответ и отправляет клиенту)
	ConsumersData, //Сервер отправляет информацию с потребителями клиенту
	RelaySwitch, //Клиент отправляет запрос на переключение реле на сервер, а сервер на есп.
	ModeSwitch, //Клиент отправляет запрос на сервер по режиму работы есп, а сервер отправляет на есп
	GraphicsData, //Сервер отправляет клиенту информацию о графиках
	ESPStatus,
	Shutdown //Желательно чтобы это отправлял клиент и есп, когда они отключаются
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
