#include "settingstab.h"

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
	layout = new QGridLayout{this};

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

	QPushButton* button = new QPushButton{this};
	button->setText(tr("Authorize"));
	button->setMaximumWidth(200);

	layout->addWidget(serverAddress, 0, 0);
	layout->addWidget(serverPort, 1, 0);
	layout->addWidget(login, 2, 0);
	layout->addWidget(password, 3, 0);
	layout->addWidget(button, 0, 1);

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

void SettingsTab::load(QSettings& settings) {
	serverAddress->setText(settings.value("serverAddress", "").toString());
	serverPort->setText(settings.value("serverPort").toString());
	login->setText(settings.value("login").toString());
}

void SettingsTab::save(QSettings& settings) {
	settings.setValue("serverAddress", serverAddress->text());
	settings.setValue("serverPort", serverPort->text());
	settings.setValue("login", login->text());
}
