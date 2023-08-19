#include "client.h"

Client::Client(QObject *parent) : QObject{parent}, mWindow{this}, mSettings{"ICS4", "Analytics app"} {

	connect(&mWindow, SIGNAL(authorize(QString,QString,QString,int)), this, SLOT(sendAuth(QString,QString,QString,int)));

	mNetworker = QSharedPointer<Networker>::create(this);

	connect(mNetworker.data(), SIGNAL(connected()), this, SLOT(onConnected()));
	connect(mNetworker.data(), SIGNAL(dataReceived(QJsonObject)), this, SLOT(onDataReceived(QJsonObject)));
	connect(mNetworker.data(), SIGNAL(unableToConnect()), this, SLOT(onUnableToConnect()));
	connect(mNetworker.data(), SIGNAL(serverLookupFailed()), this, SLOT(onServerLookupFailed()));
	connect(mNetworker.data(), SIGNAL(disconnected()), this, SLOT(onDisconnected()));

	mWindow.load(mSettings);
	mWindow.show();
}

void Client::onDataReceived(const QJsonObject& data) {
	switch (MessageType(data.value("type").toInt(-1))) {
	case AuthorizationReply: {  // json with authorization status (failed/success)
		if (!data.contains("data")) {
			throw InternalErrorException{QString{"Data structure with wrong value received at %1. The app will be closed."}.arg(FLF)};
		}

		QJsonObject payload = data.value("data").toObject();
		if (!payload.contains("status")) {
			throw InternalErrorException{QString{"Data structure with wrong value received at %1. The app will be closed."}.arg(FLF)};
		}

		int status = payload.value("status").toInt(-1);

		switch (status) {
		case 0: {
			mAuthorized = true;
			emit authorized(true);
			break;
		}
		case 1: {
			mAuthorized = false;
			emit authorized(false);
			break;
		}
		default: {
			throw InternalErrorException{QString{"Data structure with wrong value received at %1. The app will be closed."}.arg(FLF)};
			break;
		}
		}
		break;
	}
	case ESPStatus: {  // json with new esp status (connected/disconnected)
		QJsonObject payload = data.value("data").toObject();
		int status = payload.value("status").toInt(-1);

		switch (status) {
		case 0: {
			mESPConnected = false;
			emit ESPDisconnected();
			break;
		}
		case 1: {
			mESPConnected = true;
			emit ESPConnected();
			break;
		}
		default: {
			throw InternalErrorException{QString{"Data structure with wrong value received at %1. The app will be closed."}.arg(FLF)};
			break;
		}
		}

		break;
	}
	case RelaySwitch: {  // wrong data, this type can be only sent to server from client (if we have only 1 client)
		throw InternalErrorException{QString{"Data structure with wrong value received at %1. The app will be closed."}.arg(FLF)};
		break;
	}
	case ConsumersData: {  // json with information about consumers
		if (!data.contains("data"))
			throw InternalErrorException{QString{"Data structure with wrong value received at %1. The app will be closed."}.arg(FLF)};

		emit sensorsData(data.value("data").toObject());
		break;
	}
	case SensorsData: {  // json with information about sensors, that will be displayed at main tab
		if (!data.contains("data"))
			throw InternalErrorException{QString{"Data structure with wrong value received at %1. The app will be closed."}.arg(FLF)};

		emit sensorsData(data.value("data").toObject());
		break;
	}
	case Shutdown: {  // json when server dies
		emit shutdown();
		break;
	}
	default: {  // wrong data type code
		throw InternalErrorException{QString{"Data structure with wrong value received at %1. The app will be closed."}.arg(FLF)};
		break;
	}
	}
}

void Client::sendAuth(const QString& login, const QString& password, const QString& serverAddress, int serverPort) {
	mNetworker->setHostAddress(serverAddress);
	mNetworker->setHostPort(serverPort);
	mNetworker->connectToHost();

	static QString request;
	if (request.isEmpty()) {
		QFile requestFile{":/static/requests/authorization.json"};
		requestFile.open(QIODevice::ReadOnly | QIODevice::Text);
		if (!requestFile.isOpen())
			throw InternalErrorException{QString{"Internal error at %1. The app will be closed."}.arg(FLF)};
		request = requestFile.readAll();
		requestFile.close();
	}

	sendData(request.arg(login, password));  // TODO hashing password
}

void Client::onServerLookupFailed() {
	emit serverLookupFailed();
}

void Client::onUnableToConnect() {
	// set timer to retry connection and retry last action
}

void Client::sendData(const QString& request) const  {
	if (mServerConnected)
		mNetworker->sendToHost(request);
}

Client::~Client() {
	mWindow.save(mSettings);
}
