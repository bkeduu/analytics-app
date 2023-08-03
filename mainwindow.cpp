#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "client.h"

#include <QGroupBox>

MainWindow::MainWindow(Client* client, QWidget *parent) : QMainWindow{parent}, ui{new Ui::MainWindow},
	tabWidget{nullptr}, tabDialog{nullptr}, layout{nullptr}, mClient{client} {
	ui->setupUi(this);

	QFont appFont{this->font()};
	appFont.setPixelSize(18);
	appFont.setHintingPreference(QFont::PreferFullHinting);
	qApp->setFont(appFont);

	QFile globalStylesheet{":/static/stylesheets/global.css"};
	globalStylesheet.open(QIODevice::ReadOnly | QIODevice::Text);
	qApp->setStyleSheet(globalStylesheet.readAll());
	globalStylesheet.close();

	currentWindowTitle = windowTitle = tr("Analytics system");
	setWindowTitle(currentWindowTitle);
	setWindowIcon(QIcon{":/static/images/window_icon.png"});

	windowTitleChangingTimer = new QTimer{this};
	windowTitleChangingTimer->setInterval(1000);
	windowTitleChangingTimer->start();

	connect(windowTitleChangingTimer, &QTimer::timeout, windowTitleChangingTimer, [&]() {
		std::swap(currentWindowTitle, windowTitle);
		setWindowTitle(currentWindowTitle);
		windowTitleChangingTimer->start();
	});

	createStartScreen();
}

void MainWindow::resizeEvent(QResizeEvent* event) {
//	tabWidget->resize(event->size());
//	event->accept();
}

void MainWindow::authorize() {  // rewrite

	setChangingTitle(tr("Authorization..."));

	static QFile file{":/static/requests/authorization.json"};
	if (!file.isOpen())
		file.open(QIODevice::ReadOnly | QIODevice::Text);
	static QString request = file.readAll();

//	mNetworker.data()->sendToHost(request.arg(mLogin, mPassword));
}

void MainWindow::onAuthorized(const QJsonObject& dataObject) {

	int status = dataObject.value("status").toInt();
	if (status == 0) {
		// success authorization
		setChangingTitle(tr("Analytics system"));

		mStatusTab->onAuthorized();
		mForecastTab->onAuthorized();
		mGenerationTab->onAuthorized();
		mConsumersTab->onAuthorized();
		mSettingsTab->onAuthorized();
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
		setChangingTitle(tr("Analytics system"));
	}
	else {
		// esp disconnected
		setChangingTitle(tr("ESP disconnected!!!"));
	}
}

void MainWindow::onRelayClicked(int group, bool newState) {
	static QFile file{":/static/requests/relay.json"};
	if (!file.isOpen())
		file.open(QIODevice::ReadOnly | QIODevice::Text);
	static QString request = file.readAll();

//	connector->sendToHost(request.arg(group).arg(newState));
}

void MainWindow::load(QSettings& settings) {
	this->restoreGeometry(settings.value("windowGeometry").toByteArray());

	settings.beginGroup("SETTINGS");
	mSettingsTab->load(settings);
	settings.endGroup();
}

void MainWindow::save(QSettings& settings) {
	settings.setValue("windowGeometry", this->saveGeometry());

	settings.beginGroup("SETTINGS");
	mSettingsTab->save(settings);
	settings.endGroup();
}

void MainWindow::createMainContents() {
	layout = new QHBoxLayout{this};
	tabWidget = new QTabWidget{this};
	tabWidget->setSizePolicy(QSizePolicy{QSizePolicy::Maximum, QSizePolicy::Maximum});
	tabWidget->tabBar()->setSizePolicy(QSizePolicy{QSizePolicy::Maximum, QSizePolicy::Maximum});
	tabWidget->tabBar()->setDocumentMode(true);
	tabWidget->tabBar()->setExpanding(true);

	mStatusTab = QSharedPointer<StatusTab>::create("Status", this);
	mForecastTab = QSharedPointer<ForecastTab>::create("Forecast", this);
	mGenerationTab = QSharedPointer<GenerationTab>::create("Generation", this);
	mConsumersTab = QSharedPointer<ConsumersTab>::create("Consumers", this);
	mSettingsTab = QSharedPointer<SettingsTab>::create("Settings", this);

	tabWidget->addTab(mStatusTab.data(), tr(mStatusTab->getName().toStdString().c_str()));
	tabWidget->addTab(mForecastTab.data(), tr(mForecastTab->getName().toStdString().c_str()));
	tabWidget->addTab(mGenerationTab.data(), tr(mGenerationTab->getName().toStdString().c_str()));
	tabWidget->addTab(mConsumersTab.data(), tr(mConsumersTab->getName().toStdString().c_str()));
	tabWidget->addTab(mSettingsTab.data(), tr(mStatusTab->getName().toStdString().c_str()));

	tabWidget->setTabIcon(tabWidget->indexOf(mStatusTab.data()),
						  QIcon{QString{":/static/images/page_"} + mStatusTab->getName() + ".png"});
	tabWidget->setTabIcon(tabWidget->indexOf(mForecastTab.data()),
						  QIcon{QString{":/static/images/page_"} + mForecastTab->getName() + ".png"});
	tabWidget->setTabIcon(tabWidget->indexOf(mGenerationTab.data()),
						  QIcon{QString{":/static/images/page_"} + mGenerationTab->getName() + ".png"});
	tabWidget->setTabIcon(tabWidget->indexOf(mConsumersTab.data()),
						  QIcon{QString{":/static/images/page_"} + mConsumersTab->getName() + ".png"});
	tabWidget->setTabIcon(tabWidget->indexOf(mSettingsTab.data()),
						  QIcon{QString{":/static/images/page_"} + mSettingsTab->getName() + ".png"});

	connect(tabWidget, &QTabWidget::currentChanged, tabWidget, [&](int clickedTabIndex) {
		if (tabWidget->indexOf(mStatusTab.data()) == clickedTabIndex)
			mStatusTab->onTabOpened();
	});
	connect(tabWidget, &QTabWidget::currentChanged, tabWidget, [&](int clickedTabIndex) {
		if (tabWidget->indexOf(mForecastTab.data()) == clickedTabIndex)
			mForecastTab->onTabOpened();
	});
	connect(tabWidget, &QTabWidget::currentChanged, tabWidget, [&](int clickedTabIndex) {
		if (tabWidget->indexOf(mGenerationTab.data()) == clickedTabIndex)
			mGenerationTab->onTabOpened();
	});
	connect(tabWidget, &QTabWidget::currentChanged, tabWidget, [&](int clickedTabIndex) {
		if (tabWidget->indexOf(mConsumersTab.data()) == clickedTabIndex)
			mConsumersTab->onTabOpened();
	});
	connect(tabWidget, &QTabWidget::currentChanged, tabWidget, [&](int clickedTabIndex) {
		if (tabWidget->indexOf(mSettingsTab.data()) == clickedTabIndex)
			mSettingsTab->onTabOpened();
	});

	tabWidget->tabBar()->setIconSize(QSize(30, 30));

	layout->addWidget(tabWidget);
	tabWidget->setCurrentWidget(mStatusTab.data());
	tabWidget->setSizePolicy(QSizePolicy{QSizePolicy::Maximum, QSizePolicy::Maximum});

	this->setLayout(layout);
}

void MainWindow::createStartScreen() {
	QVBoxLayout* layout = new QVBoxLayout{this->centralWidget()};
	QGroupBox* formWidget = new QGroupBox{this->centralWidget()};

	QVBoxLayout* formLayout = new QVBoxLayout{formWidget};
	formLayout->setSpacing(0);
	formLayout->setContentsMargins(0, 0, 0, 0);
	formLayout->setAlignment(Qt::AlignCenter);
	formWidget->setLayout(formLayout);

	static QString groupBoxStylesheet{""};

	if (!groupBoxStylesheet.length()) {
		QFile stylesheet{":/static/stylesheets/qgroupbox.css"};
		stylesheet.open(QIODevice::ReadOnly);
		groupBoxStylesheet = stylesheet.readAll();
		stylesheet.close();
	}
	formWidget->setStyleSheet(groupBoxStylesheet);

	CustomLoginField* loginField = new CustomLoginField{tr("Login"), tr("Login"), formWidget};
	CustomLoginField* passwordField = new CustomLoginField{tr("Password"), tr("Password"), formWidget};
	CustomLoginField* serverAddressField = new CustomLoginField{tr("Server address"), tr("Server address"), formWidget};
	CustomLoginField* serverPortField = new CustomLoginField{tr("Server port"), tr("Server port"), formWidget};

	passwordField->setPasswordMode(true);

	formLayout->addWidget(loginField, 1, Qt::AlignCenter);
	formLayout->addWidget(passwordField, 1, Qt::AlignCenter);
	formLayout->addWidget(serverAddressField, 1, Qt::AlignCenter);
	formLayout->addWidget(serverPortField, 1, Qt::AlignCenter);

	static QString buttonStylesheet{""};

	if (!buttonStylesheet.length()) {
		QFile stylesheet{":/static/stylesheets/qpushbutton.css"};
		stylesheet.open(QIODevice::ReadOnly);
		buttonStylesheet = stylesheet.readAll();
		stylesheet.close();
	}

	QPushButton* authorizeButton = new QPushButton{formWidget};
	authorizeButton->setText(tr("Authorize"));
	authorizeButton->setStyleSheet(buttonStylesheet);

	formLayout->addWidget(authorizeButton, 1, Qt::AlignCenter);

	formWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	formWidget->setMinimumHeight(300);
	formWidget->setMinimumWidth(400);

	// layout->addItem(new QSpacerItem{100, 100, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding});
	layout->addStretch();
	layout->addWidget(formWidget);//, 0, 0, Qt::AlignCenter);
	layout->addStretch();
	// layout->addItem(new QSpacerItem{100, 100, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding});

	layout->setAlignment(formWidget, Qt::AlignCenter);

	this->centralWidget()->setLayout(layout);
	//setCentralWidget(formWidget);
}

void MainWindow::clearScreen() {

}

void MainWindow::onDisconnect() {



}

MainWindow::~MainWindow() {
	delete ui;
}
