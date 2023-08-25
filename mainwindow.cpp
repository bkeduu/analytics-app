#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "client.h"

#include <QGroupBox>
#include <QMessageBox>
#include <QApplication>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

MainWindow::MainWindow(Client* client, QWidget *parent) : QMainWindow{parent}, ui{new Ui::MainWindow},
	layout{nullptr}, mClient{client} {
	ui->setupUi(this);

	QFont appFont{this->font()};
	appFont.setPixelSize(18);
	appFont.setHintingPreference(QFont::PreferFullHinting);
	qApp->setFont(appFont);

	QFile globalStylesheet{":/static/stylesheets/global.css"};
	globalStylesheet.open(QIODevice::ReadOnly | QIODevice::Text);
	qApp->setStyleSheet(globalStylesheet.readAll());
	globalStylesheet.close();

	setWindowTitle(tr("Analytics system"));
	setWindowIcon(QIcon{":/static/images/window_icon.png"});

	layout = new CustomLayout{this->centralWidget()};
	layout->setContentsMargins(5, 5, 5, 5);

	mMainContent = createMainContents();
	mStartScreen = createStartScreen();

	layout->addWidget(mMainContent);
	layout->addWidget(mStartScreen);

	mMainContent->hide();

	connect(client, &Client::connecting, this, [this]() {
		mAuthLabel->setText(tr("Connecting to server..."));
	});

	connect(client, SIGNAL(connected()), this, SLOT(onConnect()));
	connect(client, SIGNAL(serverLookupFailed()), this, SLOT(onServerLookupFail()));
	connect(client, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
	connect(client, SIGNAL(unableToConnect()), this, SLOT(onUnableToConnect()));

	connect(client, SIGNAL(authorized(bool)), this, SLOT(onAuthorize(bool)));
	connect(client, SIGNAL(consumersData(QJsonObject)), this, SLOT(onConsumersData(QJsonObject)));
	connect(client, SIGNAL(sensorsData(QJsonObject)), this, SLOT(onSensorsData(QJsonObject)));
	connect(client, SIGNAL(modeSwitched(int)), this, SLOT(onModeSwitch(int)));
	connect(client, SIGNAL(graphicsData(QJsonObject)), this, SLOT(onGraphicsData(QJsonObject)));

	// TODO на главном экране марджины слева и справа не совпадают
}

QWidget* MainWindow::createMainContents() {
	QTabWidget* tabWidget = new QTabWidget{this->centralWidget()};
	tabWidget->setSizePolicy(QSizePolicy{QSizePolicy::Maximum, QSizePolicy::Maximum});
	tabWidget->tabBar()->setSizePolicy(QSizePolicy{QSizePolicy::Maximum, QSizePolicy::Maximum});
	tabWidget->tabBar()->setDocumentMode(true);
	tabWidget->tabBar()->setExpanding(true);

	mStatusTab = QSharedPointer<StatusTab>::create(QT_TR_NOOP("Status"), this);
	mForecastTab = QSharedPointer<ForecastTab>::create(QT_TR_NOOP("Forecast"), this);
	mGenerationTab = QSharedPointer<GenerationTab>::create(QT_TR_NOOP("Generation"), this);
	mConsumersTab = QSharedPointer<ConsumersTab>::create(QT_TR_NOOP("Consumers"), this);
	mSettingsTab = QSharedPointer<SettingsTab>::create(QT_TR_NOOP("Settings"), this);

	tabWidget->addTab(mStatusTab.data(), tr(mStatusTab->getName().toStdString().c_str()));
	tabWidget->addTab(mForecastTab.data(), tr(mForecastTab->getName().toStdString().c_str()));
	tabWidget->addTab(mGenerationTab.data(), tr(mGenerationTab->getName().toStdString().c_str()));
	tabWidget->addTab(mConsumersTab.data(), tr(mConsumersTab->getName().toStdString().c_str()));
	tabWidget->addTab(mSettingsTab.data(), tr(mSettingsTab->getName().toStdString().c_str()));

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

	tabWidget->tabBar()->setIconSize(QSize(35, 35));
	tabWidget->setCurrentWidget(mStatusTab.data());
	tabWidget->setMinimumSize(this->centralWidget()->size());

	connect(mStatusTab.data(), SIGNAL(modeChanged(int)), this, SLOT(onModeChange(int)));

	this->tabWidget = tabWidget;
	return tabWidget;
}

QWidget* MainWindow::createStartScreen() {
	// TODO паддинги в result виджете
	QWidget* result = new QWidget{this->centralWidget()};
	QVBoxLayout* resultLayout = new QVBoxLayout{result};
	result->setLayout(resultLayout);

	QGroupBox* formWidget = new QGroupBox{result};
	QVBoxLayout* formLayout = new QVBoxLayout{formWidget};
	formLayout->setSpacing(0);
	formLayout->setContentsMargins(0, 0, 0, 0);
	formLayout->setAlignment(Qt::AlignCenter);
	formWidget->setLayout(formLayout);

	static QString groupBoxStylesheet;
	if (groupBoxStylesheet.isEmpty()) {
		QFile stylesheet{":/static/stylesheets/qgroupbox.css"};
		stylesheet.open(QIODevice::ReadOnly | QIODevice::Text);
		groupBoxStylesheet = stylesheet.readAll();
		stylesheet.close();
	}
	formWidget->setProperty("class", "authGroupBox");
	formWidget->setStyleSheet(groupBoxStylesheet);

	mAuthLabel = new QLabel{formWidget};
	mAuthLabel->setText(tr("Log in to the system"));
	mAuthLabel->setStyleSheet("font-weight:bold; font-size: 32px;");

	mLoginField = new CustomLineEdit{tr("Login"), tr("Login"), formWidget};
	mPasswordField = new CustomLineEdit{tr("Password"), tr("Password"), formWidget};
	mServerAddressField = new CustomLineEdit{tr("Server address"), tr("Server address"), formWidget};
	mServerPortField = new CustomLineEdit{tr("Server port"), tr("Server port"), formWidget};

	mPasswordField->setPasswordMode(true);

	formLayout->addSpacing(10);
	formLayout->addWidget(mAuthLabel, 1, Qt::AlignCenter);
	formLayout->addSpacing(40);
	formLayout->addWidget(mLoginField, 1, Qt::AlignCenter);
	formLayout->addWidget(mPasswordField, 1, Qt::AlignCenter);
	formLayout->addWidget(mServerAddressField, 1, Qt::AlignCenter);
	formLayout->addWidget(mServerPortField, 1, Qt::AlignCenter);
	formLayout->addSpacing(30);

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
	authorizeButton->setAutoDefault(true);

	formLayout->addWidget(authorizeButton, 1, Qt::AlignCenter);
	formLayout->addSpacing(10);

	formWidget->setMinimumSize(500, 300);
	formWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	resultLayout->addWidget(formWidget, 1, Qt::AlignCenter);

	connect(authorizeButton, &QPushButton::clicked, authorizeButton, [&]() {
		QString login = mLoginField->text().trimmed();
		QString password = mPasswordField->text().trimmed();
		QString serverAddress = mServerAddressField->text().trimmed();
		int serverPort;

		if (login.isEmpty()) {
			QMessageBox mb{QMessageBox::Critical, tr("Empty login"),
						   tr("You need to fill all fields, but login field is empty."), QMessageBox::Ok, this};
			mb.setWindowIcon(QIcon{":/static/images/error.png"});
			mb.exec();
			return;
		}

		if (password.isEmpty()) {
			QMessageBox mb{QMessageBox::Critical, tr("Empty password"),
						   tr("You need to fill all fields, but password field is empty."), QMessageBox::Ok, this};
			mb.setWindowIcon(QIcon{":/static/images/error.png"});
			mb.exec();
			return;
		}

		if (serverAddress.isEmpty()) {
			QMessageBox mb{QMessageBox::Critical, tr("Empty server address"),
						   tr("You need to fill all fields, but server address field is empty."), QMessageBox::Ok, this};
			mb.setWindowIcon(QIcon{":/static/images/error.png"});
			mb.exec();
			return;
		}

		bool conversionOk = false;
		serverPort = mServerPortField->text().trimmed().toInt(&conversionOk);

		if (!conversionOk || serverPort < 0) {
			QMessageBox mb{QMessageBox::Critical, tr("Incorrect server port"),
						   tr("Server port field is empty or contains incorrect value."), QMessageBox::Ok, this};
			mb.setWindowIcon(QIcon{":/static/images/error.png"});
			mb.exec();
			return;
		}

		mAuthLabel->setText(tr("Authorization..."));
		emit authorize(login, password, serverAddress, serverPort);
	});

	return result;
}

void MainWindow::load(QSettings& settings) {
	restoreGeometry(settings.value("windowGeometry").toByteArray());

	settings.beginGroup("loginData");

	mLoginField->setText(settings.value("login").toString());
	mServerAddressField->setText(settings.value("serverAddress").toString());
	mServerPortField->setText(settings.value("serverPort").toString());

	settings.endGroup();
}

void MainWindow::save(QSettings& settings) {
	settings.setValue("windowGeometry", saveGeometry());

	settings.beginGroup("loginData");

	settings.setValue("login", mLoginField->text());
	settings.setValue("serverAddress", mServerAddressField->text());
	settings.setValue("serverPort", mServerPortField->text());

	settings.endGroup();
}

void MainWindow::resizeEvent(QResizeEvent* event) {
	QSize s = centralWidget()->size();
	QMargins m = centralWidget()->layout()->contentsMargins();
	s.setHeight(s.height() - m.top() - m.bottom());
	s.setWidth(s.width() - m.left() - m.right());
	tabWidget->setMinimumSize(s);
	QMainWindow::resizeEvent(event);
}

void MainWindow::onAuthorize(bool status) {
	if (status) {
		// authorization successfull, starting animation

		QGraphicsOpacityEffect* mainEffect = new QGraphicsOpacityEffect{mMainContent};
		QPropertyAnimation* mainAnimation = new QPropertyAnimation{mainEffect, "opacity"};

		mMainContent->setGraphicsEffect(mainEffect);
		mainAnimation->setStartValue(0);
		mainAnimation->setEndValue(1);
		mainAnimation->setDuration(500);

		QGraphicsOpacityEffect* authEffect = new QGraphicsOpacityEffect{mStartScreen};
		QPropertyAnimation* authAnimation = new QPropertyAnimation{authEffect, "opacity"};

		mStartScreen->setGraphicsEffect(authEffect);
		authAnimation->setStartValue(1);
		authAnimation->setEndValue(0);
		authAnimation->setDuration(500);

		QSequentialAnimationGroup* group = new QSequentialAnimationGroup{};
		group->addAnimation(mainAnimation);
		group->addAnimation(authAnimation);

		authAnimation->start();

		connect(authAnimation, &QPropertyAnimation::finished, authAnimation, [this]() {
			mStartScreen->hide();
		});

		mMainContent->show();
		mainAnimation->start();

		QFont appFont{this->font()};
		appFont.setPixelSize(18);
		appFont.setHintingPreference(QFont::PreferFullHinting);
		qApp->setFont(appFont);

		QFile globalStylesheet{":/static/stylesheets/global.css"};
		globalStylesheet.open(QIODevice::ReadOnly | QIODevice::Text);
		qApp->setStyleSheet(globalStylesheet.readAll());
		globalStylesheet.close();

		unlockTabs();

		resizeEvent(nullptr);
	}
	else {
		// authorization unsuccessfull, showing dialog with error message
		QMessageBox mb{QMessageBox::Critical, tr("Authorization unsuccessfull"),
					   tr("Provided login or password is incorrect. Try again."), QMessageBox::Ok, this};
		mb.setWindowIcon(QIcon{":/static/images/error.png"});
		mb.exec();
	}
}

void MainWindow::onSensorsData(const QJsonObject& data) {
	mStatusTab->onSensorsDataReceived(data);
	mConsumersTab->onSensorsDataReceived(data);
}

void MainWindow::onConnect() {

}

void MainWindow::onESPStatusChange(const QJsonObject& data) {
	if (!data.contains("status"))
		throw InternalErrorException{tr("Internal error at %1. The app will be closed.").arg(FLF)};

	if (data.value("status").toInt())
		unlockTabs();
	else
		lockTabs();
}

void MainWindow::onRelayClicked(int group, bool newState) {
	emit relayClicked(group, newState);
}

void MainWindow::onServerLookupFail() {
	QMessageBox mb{QMessageBox::Critical, tr("Unable to lookup server"),
				   tr("Unable to find server with provided hostname. Enter another name and try again."), QMessageBox::Ok, this};
	mb.setWindowIcon(QIcon{":/static/images/error.png"});

	mb.exec();
}

void MainWindow::onUnableToConnect() {

}

void MainWindow::onConsumersData(const QJsonObject& data) {
	mConsumersTab->onConsumersReceived(data);
}

void MainWindow::onDisconnect() {
	QString windowText = tr("The connection to server lost. Control will be blocked. Check the connection and restart the app.");
	if (mClient->authorized())
		lockTabs();
	else
		windowText = tr("The connection to server lost. Try again.");

	QMessageBox mb{QMessageBox::Critical, tr("Connection with server lost"), windowText, QMessageBox::Ok, this};
	mb.setWindowIcon(QIcon{":/static/images/error.png"});
	mb.exec();
}

void MainWindow::onModeSwitch(int newMode) {  // mode switch from server
	mStatusTab->switchMode(newMode);

	if (ModeType(newMode) == ModeType::Manual)
		unlockTabs();
	else
		lockTabs();
}

void MainWindow::onModeChange(int mode) {  // mode switch with button
	if (ModeType(mode) == ModeType::Manual)
		unlockTabs();
	else
		lockTabs();

	emit modeChanged(mode);
}

void MainWindow::onGraphicsData(const QJsonObject& data) {
	mForecastTab->onForecastDataReceived(data);
}

void MainWindow::lockTabs() {
	mStatusTab->lock();
	mForecastTab->lock();
	mGenerationTab->lock();
	mConsumersTab->lock();
	mSettingsTab->lock();
}

void MainWindow::unlockTabs() {
	mStatusTab->unlock();
	mForecastTab->unlock();
	mGenerationTab->unlock();
	mConsumersTab->unlock();
	mSettingsTab->unlock();
}

MainWindow::~MainWindow() {
	delete ui;
}
