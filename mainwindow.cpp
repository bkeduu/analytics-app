#include "mainwindow.h"
#include "statustab.h"
#include "consumerstab.h"
#include "generationtab.h"
#include "forecasttab.h"
#include "settingstab.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow{parent}, ui{new Ui::MainWindow},
	tabWidget{nullptr}, tabDialog{nullptr}, layout{nullptr}, mAuthorized{false}, mESPConnected{false},
	settings{"analytics.conf", QSettings::IniFormat} {
	ui->setupUi(this);

	QFont appFont{this->font()};
	appFont.setPixelSize(18);
	appFont.setHintingPreference(QFont::PreferFullHinting);
	this->setFont(appFont);

	currentWindowTitle = windowTitle = tr("Analytics system");
	setWindowTitle(currentWindowTitle);
	setWindowIcon(QIcon{":/static/images/window_icon.png"});

	connector = new Networker{this};
	windowTitleChangingTimer = new QTimer{this};
	windowTitleChangingTimer->setInterval(1000);
	windowTitleChangingTimer->start();

	connect(windowTitleChangingTimer, &QTimer::timeout, windowTitleChangingTimer, [=]() {
		std::swap(currentWindowTitle, windowTitle);
		setWindowTitle(currentWindowTitle);
		windowTitleChangingTimer->start();
	});

	connect(connector, SIGNAL(authorized(QJsonObject)), this, SLOT(onAuthorized(QJsonObject)));
	connect(connector, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
	// connect(connector, SIGNAL(ESPStatusChanged(const QJsonObject&)), this, SLOT(onESPStatusChanged(const QJsonObject&)));

	layout = new QHBoxLayout{this};
	tabWidget = new QTabWidget{this};

	StatusTab* statusTab = StatusTab::getWidget(tr("Status"), this);
	connect(connector, SIGNAL(dataReceived(QJsonObject)), statusTab, SLOT(onDataReceived(QJsonObject)));
	tabs[Tab::Status] = statusTab;

	ConsumersTab* consumersTab = ConsumersTab::getWidget(tr("Consumers"), this);

	QFile f{":/static/requests/consumers.json"};
	f.open(QIODevice::ReadOnly | QIODevice::Text);
	QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
	QJsonObject obj = doc.object().value("data").toObject();
	consumersTab->setJSONDocument(obj);

	//connect(connector, SIGNAL(consumersReceived(QJsonObject)), consumersTab, SLOT(setJSONDocument(QJsonObject)));
	tabs[Tab::Consumers] = consumersTab;

	tabs[Tab::Generation] = GenerationTab::getWidget(tr("Generation"), this);
	tabs[Tab::Forecast] = ForecastTab::getWidget(tr("Forecast"), this);

	SettingsTab* settingsTab = SettingsTab::getWidget(tr("Settings"), this);

	connect(settingsTab, &SettingsTab::serverAddressUpdated, this, [=](const QString& newAddress) {
		serverAddress = newAddress;
		connector->setHostAddress(QHostAddress{serverAddress});
	});

	connect(settingsTab, &SettingsTab::serverPortUpdated, this, [=](const QString& newPort) {
		serverPort = newPort;
		connector->setHostPort(serverPort.toInt());
	});

	connect(settingsTab, &SettingsTab::loginUpdated, this, [=](const QString& newLogin) {
		login = newLogin;
	});

	connect(settingsTab, &SettingsTab::passwordUpdated, this, [=](const QString& newPassword) {
		password = newPassword;
	});

	connect(settingsTab, &SettingsTab::authorizationClicked, this, [=]() {
		connector->connectToHost();
		authorize();
	});

	tabs[Tab::Settings] = settingsTab;

	foreach (Tab tab, tabs.keys()) {
		tabWidget->addTab(tabs[tab], tabs[tab]->getName());
		tabWidget->setTabIcon(tabWidget->indexOf(tabs[tab]),
							  QIcon{QString{":/static/images/page_"} + QString::number((int)tab) + ".png"});
	}

	tabWidget->tabBar()->setIconSize(QSize(30, 30));

	layout->addWidget(tabWidget);
	tabWidget->setCurrentWidget(tabs[Tab::Status]);
	tabWidget->setSizePolicy(QSizePolicy{QSizePolicy::Maximum, QSizePolicy::Maximum});

	this->setLayout(layout);
	this->load();
}

void MainWindow::resizeEvent(QResizeEvent* event) {
	tabWidget->resize(event->size());
	event->accept();
}

void MainWindow::authorize() {

	setChangingTitle(tr("Authorization..."));

	static QFile file{":/static/requests/authorization.json"};
	if (!file.isOpen())
		file.open(QIODevice::ReadOnly | QIODevice::Text);
	static QString request = file.readAll();

	connector->sendToHost(request.arg(login, password));
}

void MainWindow::onAuthorized(const QJsonObject& dataObject) {

	int status = dataObject.value("status").toInt();
	if (status == 0) {
		// success authorization
		mAuthorized = true;
		mESPConnected = true;
		setChangingTitle(tr("Analytics system"));

		foreach (ITab* tab, tabs) {
			tab->onAuthorize();
		}
	}
	else {
		// error
		setChangingTitle(tr("Unsuccessfull authorization!!!"));
	}
}

void MainWindow::onESPStatusChanged(const QJsonObject& dataObject) {

	int status = dataObject.value("status").toInt();

	if (status) {
		// esp connected
		mESPConnected = true;
		setChangingTitle(tr("Analytics system"));
	}
	else {
		// esp disconnected
		setChangingTitle(tr("ESP disconnected!!!"));
		mESPConnected = false;
	}
}

void MainWindow::onRelayClicked(int group, bool newState) {
	static QFile file{":/static/requests/relay.json"};
	if (!file.isOpen())
		file.open(QIODevice::ReadOnly | QIODevice::Text);
	static QString request = file.readAll();

	connector->sendToHost(request.arg(group).arg(newState));
}

void MainWindow::load() {
	settings.beginGroup("SETTINGS");
	tabs[Tab::Settings]->load(settings);
	settings.endGroup();
}

void MainWindow::save() {
	settings.beginGroup("SETTINGS");
	tabs[Tab::Settings]->save(settings);
	settings.endGroup();
}

void MainWindow::onDisconnect() {



}

MainWindow::~MainWindow() {

	this->save();

	delete ui;
}
