#include "client.h"

Client::Client(QObject *parent) : QObject{parent}, mWindow{this}, mSettings{"ICS4", "Analytics app"} {

	connect(&mWindow, SIGNAL(authorize(QString,QString,QString,int)), this, SLOT(sendAuth(QString, QString, QString, int)));

	mNetworker = QSharedPointer<Networker>::create(this);
	connect(mNetworker.data(), SIGNAL(dataReceived(QJsonObject)), this, SLOT(onDataReceived(QJsonObject)));

	mWindow.load(mSettings);
	mWindow.show();
}

void Client::onDataReceived(const QJsonObject& data) {
	switch (MessageType(data.value("type").toInt(-1))) {
	case AuthorizationReply: {  // json with authorization status (failed/success)
		if (!data.contains("data")) {
			throw InternalErrorException{QString{"Data structure with wrong value received. The app will be closed."} + FLF};
		}

		QJsonObject payload = data.value("data").toObject();
		if (!payload.contains("status")) {
			throw InternalErrorException{QString{"Data structure with wrong value received. The app will be closed."} + FLF};
		}

		int status = payload.value("status").toInt(-1);

		switch (status) {
		case 0: {
			// authorization success

			emit authorized(true);
			break;
		}
		case 1: {
			// authorization failed

			emit authorized(false);
			break;
		}
		default: {
			throw InternalErrorException{QString{"Data structure with wrong value received. The app will be closed."} + FLF};
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
			// esp disconnected
			break;
		}
		case 1: {
			//  esp connected
			break;
		}
		default: {
			// wrong data
			// TODO error to view
			throw InternalErrorException{QString{"Data structure with wrong value received. The app will be closed."} + FLF};
			break;
		}
		}

		break;
	}
	case RelaySwitch: {  // wrong data, this type can be only sent to server from client (if we have only 1 client)
		// TODO error to view
		throw InternalErrorException{QString{"Data structure with wrong value received. The app will be closed."} + FLF};
		break;
	}
	case ConsumersData: {  // json with information about consumers
		break;
	}
	case SensorsData: {  // json with information about sensors, that will be displayed at main tab
		break;
	}
	case Shutdown: {  // json when server dies
		break;
	}
	default: {  // wrong data
		throw InternalErrorException{QString{"Data structure with wrong value received. The app will be closed."} + FLF};
		break;
	}
	}

//	connect(mNetworker.data(), SIGNAL(authorized(QJsonObject)), this, SLOT(onAuthorized(QJsonObject)));
//	connect(mNetworker.data(), SIGNAL(disconnected()), this, SLOT(onDisconnect()));
//	// connect(mNetworker.data(), SIGNAL(ESPStatusChanged(const QJsonObject&)), this, SLOT(onESPStatusChanged(const QJsonObject&)));
//	connect(mNetworker.data(), SIGNAL(dataReceived(const QJsonObject&)), mStatusTab.data(), SLOT(onDataReceived(const QJsonObject&)));
//	connect(mNetworker.data(), SIGNAL(dataReceived(const QJsonObject&)), mForecastTab.data(), SLOT(onDataReceived(const QJsonObject&)));
//	connect(mNetworker.data(), SIGNAL(dataReceived(const QJsonObject&)), mGenerationTab.data(), SLOT(onDataReceived(const QJsonObject&)));
//	connect(mNetworker.data(), SIGNAL(dataReceived(const QJsonObject&)), mConsumersTab.data(), SLOT(onDataReceived(const QJsonObject&)));
//	connect(mNetworker.data(), SIGNAL(dataReceived(const QJsonObject&)), mSettingsTab.data(), SLOT(onDataReceived(const QJsonObject&)));
//	connect(mNetworker.data(), SIGNAL(dataReceived(QJsonObject)), mStatusTab.data(), SLOT(onDataReceived(QJsonObject)));
//	connect(mNetworker.data(), SIGNAL(consumersReceived(QJsonObject)), mConsumersTab.data(), SLOT(setJSONDocument(QJsonObject)));
//	connect(mSettingsTab.data(), &SettingsTab::authorizationClicked, this,
//			[&](const QString& serverAddress, int serverPort, const QString& login, const QString& password) {
//				mNetworker.data()->setHostAddress(serverAddress);
//				mNetworker.data()->setHostPort(serverPort);

//				mLogin = login;
//				mPassword = password;

//				mNetworker.data()->connectToHost();
//				authorize();
//			});
}

void Client::sendAuth(const QString& login, const QString& password, const QString& serverAddress, int serverPort) {
	mNetworker->setHostAddress(serverAddress);
	mNetworker->setHostPort(serverPort);
	mNetworker->connectToHost();

	static QString request;
	if (request.isEmpty()) {
		QFile requestFile{":/static/requests/authorization.json"};
		if (!requestFile.isOpen())
			requestFile.open(QIODevice::ReadOnly | QIODevice::Text);
		request = requestFile.readAll();
		requestFile.close();
	}

	mNetworker->sendToHost(request.arg(login, password));  // TODO hashing password
}

void Client::sendData() {

}

Client::~Client() {
	mWindow.save(mSettings);
}
