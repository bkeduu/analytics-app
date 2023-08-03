#include "client.h"

Client::Client(QObject *parent) : QObject{parent}, mWindow{this}, mSettings{"ICS4", "Analytics app"} {
	mWindow.show();

	mNetworker = QSharedPointer<Networker>::create(this);
	connect(mNetworker.data(), SIGNAL(dataReceived(QJsonObject)), this, SLOT(onDataReceived(QJsonObject)));

	mWindow.load(mSettings);
}

void Client::onDataReceived(const QJsonObject& data) {

	switch (MessageType(data.value("type").toInt(-1))) {
	case AuthorizationReply: {  // json with authorization status (failed/success)
		QJsonObject payload = data.value("data").toObject();
		int status = payload.value("status").toInt(-1);

		switch (status) {
		case 0:
			// authorization success
			break;
		case 1:
			// authorization failed
			break;
		default:
			// wrong data
			// TODO error to view
			break;
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
			break;
		}
		}

		break;
	}
	case RelaySwitch: {  // wrong data, this type can be only sent to server from client (if we have only 1 client)
		// TODO error to view
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
		// TODO error to view
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

void Client::sendData() {

}

Client::~Client() {
	mWindow.save(mSettings);
}
