#include "mainwindow.h"
#include "statustab.h"
#include "consumerstab.h"
#include "generationtab.h"
#include "forecasttab.h"
#include "settingstab.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow{parent}, ui{new Ui::MainWindow},
	tabWidget{nullptr}, tabDialog{nullptr}, layout{nullptr}, mAuthorized{false}, mESPConnected{false} {
	ui->setupUi(this);

	QFont appFont{this->font()};
	appFont.setPixelSize(18);
	appFont.setHintingPreference(QFont::PreferFullHinting);
	this->setFont(appFont);

	currentWindowTitle = windowTitle = tr("Analytics system");
	setWindowTitle(currentWindowTitle);
	setWindowIcon(QIcon{":/static/images/window_icon.png"});

	connector = new Networker{this};

	QFile config{"C:\\Users\\leakt\\Documents\\build-analytics-app-Desktop_Qt_6_4_2_MinGW_64_bit-Debug\\debug\\config.json"};
	config.open(QIODevice::ReadOnly | QIODevice::Text);
	parseConfig(config);

	windowTitleChangingTimer = new QTimer{this};
	windowTitleChangingTimer->setInterval(1000);
	windowTitleChangingTimer->start();

	connect(windowTitleChangingTimer, &QTimer::timeout, windowTitleChangingTimer, [=]() {
		std::swap(currentWindowTitle, windowTitle);
		setWindowTitle(currentWindowTitle);
		windowTitleChangingTimer->start();
	});

	connector->connectToHost();
	authorize();

	connect(connector, SIGNAL(authorized(QJsonArray)), this, SLOT(onAuthorized(QJsonArray)));
	//connect(connector, SIGNAL(consumersReceived(QJsonObject)), this, SLOT(onConsumersReceived(QJsonObject)));
	connect(connector, SIGNAL(ESPStatusChanged(QJsonArray)), this, SLOT(onESPStatusChanged(QJsonArray)));

	layout = new QHBoxLayout{this};
	tabWidget = new QTabWidget{this};

	StatusTab* statusTab = StatusTab::getWidget(tr("Status"), this);
	connect(connector, SIGNAL(dataReceived(const QJsonArray&)), statusTab, SLOT(onDataReceived(const QJsonArray&)));
	tabs[Tab::Status] = statusTab;

	ConsumersTab* consumersTab = ConsumersTab::getWidget(tr("Consumers"), this);
	//connect(connector, SIGNAL(consumersReceived(QJsonObject)), consumersTab, SLOT(setJSONDocument(QJsonObject)));
	tabs[Tab::Consumers] = consumersTab;

	tabs[Tab::Generation] = GenerationTab::getWidget(tr("Generation"), this);
	tabs[Tab::Forecast] = ForecastTab::getWidget(tr("Forecast"), this);
	tabs[Tab::Settings] = SettingsTab::getWidget(tr("Settings"), this);

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
}

void MainWindow::resizeEvent(QResizeEvent* event) {
	tabWidget->resize(event->size());
	event->accept();
}

void MainWindow::parseConfig(QFile& conf) {

	QString configContent;
	configContent = conf.readAll();

	QJsonDocument configJSON = QJsonDocument::fromJson(configContent.toUtf8());
	QJsonObject configObject = configJSON.object();

	connector->setHostAddress(QHostAddress{configObject.value("server_ip").toString()});
	connector->setHostPort(configObject.value("server_port").toInt());

	login = configObject.value("login").toString();
	password = configObject.value("password").toString();
}

void MainWindow::authorize() {

	setChangingTitle(tr("Authorization..."));

	static QFile file{":/static/requests/authorization.json"};
	if (!file.isOpen())
		file.open(QIODevice::ReadOnly | QIODevice::Text);
	static QString request = file.readAll();

	connector->sendToHost(request.arg(login, password));
}

void MainWindow::onAuthorized(QJsonArray dataObject) {

	int status = dataObject[1].toInt();
	if (status == 0) {
		// success authorization
		mAuthorized = true;
		setChangingTitle(tr("Wait for ESP connection..."));
	}
	else {
		// error
		setChangingTitle(tr("Unsuccessfull authorization!!!"));
	}
}

void MainWindow::onESPStatusChanged(QJsonArray dataObject) {

	int status = dataObject[1].toInt();

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

MainWindow::~MainWindow() {
	delete ui;
}
