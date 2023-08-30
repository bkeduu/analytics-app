#include "client.h"

Client::Client(QObject *parent) : QObject{parent}, mAuthorized{false}, mServerConnected{false},
	mWindow{this}, mSettings{QSettings::UserScope, "ICS4", "Analytics app"} {

	connect(&mWindow, SIGNAL(authorize(QString,QString,QString,int)), this, SLOT(sendAuth(QString,QString,QString,int)));

	mNetworker = QSharedPointer<Networker>::create(this);

	connect(mNetworker.data(), SIGNAL(connected()), this, SLOT(onConnected()));
	connect(mNetworker.data(), SIGNAL(dataReceived(QJsonObject)), this, SLOT(onDataReceived(QJsonObject)));
	connect(mNetworker.data(), SIGNAL(unableToConnect()), this, SLOT(onUnableToConnect()));
	connect(mNetworker.data(), SIGNAL(serverLookupFailed()), this, SLOT(onServerLookupFailed()));
	connect(mNetworker.data(), SIGNAL(disconnected()), this, SLOT(onDisconnected()));

	connect(&mWindow, SIGNAL(relayClicked(int,bool)), this, SLOT(onRelayClicked(int,bool)));
	connect(&mWindow, SIGNAL(modeChanged(int)), this, SLOT(onModeChange(int)));

	mWindow.load(mSettings);
	mWindow.show();
}

void Client::onDataReceived(const QJsonObject& data) {
	switch (MessageType(data.value("type").toInt(-1))) {
	case AuthorizationSuccess: {  // json with authorization status (failed/success)
		mAuthorized = true;
		emit authorized(true);
		break;
	}
	case AuthorizationError: {  // json with authorization status (failed/success)
		mAuthorized = false;

		if (mServerConnected) {
			mNetworker.clear();
			mNetworker = QSharedPointer<Networker>::create(this);
		}

		emit authorized(false);
		break;
	}
	case ESPStatus: {  // json with new esp status (connected/disconnected)
		if (mAuthorized) {
			QJsonObject payload = data.value("data").toObject();
			int status = payload.value("status").toInt(-1);

			switch (status) {
			case 0: {
				mESPConnected = false;
				break;
			}
			case 1: {
				mESPConnected = true;
				break;
			}
			default: {
				throw InternalErrorException{tr("Data structure with wrong value received at %1. The app will be closed.").arg(FLF)};
				break;
			}
			}

			emit ESPConnectionChanged(mESPConnected);
		}
		break;
	}
	case ModeSwitch: {
		if (mAuthorized) {
			if (!data.contains("data"))
			throw InternalErrorException{tr("Data structure with wrong value received at %1. The app will be closed.").arg(FLF)};

			QJsonObject payload = data.value("data").toObject();

			if (!payload.contains("mode"))
			throw InternalErrorException{tr("Data structure with wrong value received at %1. The app will be closed.").arg(FLF)};

			emit modeSwitched(payload.value("mode").toInt());
		}
		break;
	}
	case RelaySwitch: {  // wrong data, this type can be only sent to server from client (if we have only 1 client)
		throw InternalErrorException{tr("Data structure with wrong value received at %1. The app will be closed.").arg(FLF)};
		break;
	}
	case ConsumersData: {  // json with information about consumers
		if (mAuthorized) {
			if (!data.contains("data"))
			throw InternalErrorException{tr("Data structure with wrong value received at %1. The app will be closed.").arg(FLF)};

			emit consumersData(data.value("data").toObject());
		}
		break;
	}
	case GraphicsData: {
		if (mAuthorized) {
			if (!data.contains("data"))
			throw InternalErrorException{tr("Data structure with wrong value received at %1. The app will be closed.").arg(FLF)};

			emit graphicsData(data.value("data").toObject());
		}
		break;
	}
	case SensorsData: {  // json with information about sensors, that will be displayed at main tab
		if (mAuthorized) {
			if (!data.contains("data"))
			throw InternalErrorException{tr("Data structure with wrong value received at %1. The app will be closed.").arg(FLF)};

			emit sensorsData(data.value("data").toObject());
		}
		break;
	}
	case Shutdown: {  // json when server dies
		if (mAuthorized) {
			emit shutdown();
		}
		break;
	}
	default: {  // wrong data type code
		throw InternalErrorException{tr("Data structure with wrong value received at %1. The app will be closed.").arg(FLF)};
		break;
	}
	}
}

void Client::sendAuth(const QString& login, const QString& password, const QString& serverAddress, int serverPort) {
	if (!mServerConnected) {
		mNetworker->setHostAddress(serverAddress);
		mNetworker->setHostPort(serverPort);

		emit connecting();
		mNetworker->connectToHost();
	}

	static QString request;
	if (request.isEmpty()) {
		QFile requestFile{":/static/requests/authorization.json"};
		requestFile.open(QIODevice::ReadOnly | QIODevice::Text);
		if (!requestFile.isOpen())
		throw InternalErrorException{tr("Internal error at %1. The app will be closed.").arg(FLF)};
		request = requestFile.readAll();
		requestFile.close();
	}

	sendData(request.arg(int(Authorization)).arg(login).arg(password));  // TODO hashing password
}

void Client::onServerLookupFailed() {
	emit serverLookupFailed();
}

void Client::onUnableToConnect() {
	emit unableToConnect();
}

void Client::onRelayClicked(int group, bool newState) {
	static QString request;
	if (request.isEmpty()) {
		QFile requestFile{":/static/requests/relay.json"};
		requestFile.open(QIODevice::ReadOnly | QIODevice::Text);
		if (!requestFile.isOpen())
			throw InternalErrorException{tr("Internal error at %1. The app will be closed.").arg(FLF)};
		request = requestFile.readAll();
		requestFile.close();
	}

	sendData(request.arg(int(RelaySwitch)).arg(group).arg(newState));
}

void Client::onConnected() {
	mServerConnected = true;
	emit connected();
}

void Client::onDisconnected() {
	mServerConnected = false;
	emit disconnected();
}

void Client::onModeChange(int mode) {
	static QString request;
	if (request.isEmpty()) {
		QFile requestFile{":/static/requests/mode_switch.json"};
		requestFile.open(QIODevice::ReadOnly | QIODevice::Text);
		if (!requestFile.isOpen())
			throw InternalErrorException{tr("Internal error at %1. The app will be closed.").arg(FLF)};
		request = requestFile.readAll();
		requestFile.close();
	}

	sendData(request.arg(int(ModeSwitch)).arg(mode));
}

void Client::sendData(const QString& request) const  {
	if (mServerConnected)
		mNetworker->sendToHost(request);
}

Client::~Client() {
	QFile requestFile{":/static/requests/shutdown.json"};
	requestFile.open(QIODevice::ReadOnly | QIODevice::Text);
	if (requestFile.isOpen()) {
		sendData(QString{requestFile.readAll()}.arg(int(Shutdown)));
		requestFile.close();
	}

	mWindow.save(mSettings);
}
