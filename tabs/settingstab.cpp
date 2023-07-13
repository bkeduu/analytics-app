#include "settingstab.h"
#include "mainwindow.h"

#include <QLineEdit>

SettingsTab* SettingsTab::instance = nullptr;
QMutex SettingsTab::lock{};

SettingsTab* SettingsTab::getWidget(const QString& tabName, QWidget* parent) {
	QMutexLocker locker{&SettingsTab::lock};
	if (!instance)
		instance = new SettingsTab{tabName, parent};
	return instance;
}

SettingsTab::SettingsTab(const QString& tabName, QWidget* parent) : ITab{parent, tabName}, layout{nullptr} {
	createTabContents();
}

void SettingsTab::onAuthorized() {
	clearTab();
	delete this->layout;
	layout = new QGridLayout{this};
	this->setLayout(layout);
	layout->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

	QLabel* textLabel = new QLabel{this};
	textLabel->setProperty("class", "tab-standalone-text");
	textLabel->setText(tr("You are successfully authorized"));
	textLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	textLabel->setAlignment(Qt::AlignCenter);

	layout->addWidget(textLabel);
}

void SettingsTab::onTabOpened() {
	if (!mParent->authorized() && button->isVisible())
		button->setFocus(Qt::TabFocusReason);
}

void SettingsTab::createTabContents() {
	clearTab();
	QGridLayout* gridLayout = new QGridLayout{this};
	layout = gridLayout;
	this->setLayout(layout);

	serverAddress = new QLineEdit{this};
	serverAddress->setPlaceholderText(tr("Server\'s IP"));
	serverAddress->setMaximumWidth(200);

	serverPort = new QLineEdit{this};
	serverPort->setPlaceholderText(tr("Server\'s port"));
	serverPort->setMaximumWidth(200);

	login = new QLineEdit{this};
	login->setPlaceholderText(tr("Login"));
	login->setMaximumWidth(200);

	password = new QLineEdit{this};
	password->setPlaceholderText(tr("Password"));
	password->setMaximumWidth(200);
	password->setEchoMode(QLineEdit::Password);

	button = new QPushButton{this};
	button->setText(tr("Authorize"));
	button->setMaximumWidth(200);
	button->setDefault(false);
	button->setAutoDefault(true);

	gridLayout->addWidget(serverAddress, 0, 0);
	gridLayout->addWidget(serverPort, 1, 0);
	gridLayout->addWidget(login, 2, 0);
	gridLayout->addWidget(password, 3, 0);
	gridLayout->addWidget(button, 0, 1);

	connect(serverAddress, &QLineEdit::textChanged, serverAddress, [=](const QString& newAddress) {
		emit serverAddressUpdated(newAddress);
	});

	connect(serverPort, &QLineEdit::textChanged, serverPort, [=](const QString& newPort) {
		emit serverPortUpdated(newPort);
	});

	connect(login, &QLineEdit::textChanged, login, [=](const QString& newLogin) {
		emit loginUpdated(newLogin);
	});

	connect(password, &QLineEdit::textChanged, password, [=](const QString& newPassword) {
		emit passwordUpdated(newPassword);
	});

	connect(button, &QPushButton::clicked, button, [=]() {
		emit authorizationClicked();
	});
}

void SettingsTab::removeTabContents(const QString& /*text*/) {

}

void SettingsTab::load(QSettings& settings) {
	serverAddress->setText(settings.value("serverAddress", "").toString());
	serverPort->setText(settings.value("serverPort").toString());
	login->setText(settings.value("login").toString());
	password->setText(settings.value("password").toString());
}

void SettingsTab::save(QSettings& settings) {
	settings.setValue("serverAddress", serverAddress->text());
	settings.setValue("serverPort", serverPort->text());
	settings.setValue("login", login->text());
	settings.setValue("password", password->text());
}
