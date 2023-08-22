#include "settingstab.h"
#include "mainwindow.h"

#include <QLineEdit>
#include <QMessageBox>

SettingsTab::SettingsTab(const QString& tabName, QWidget* parent) : ITab{parent, tabName}, layout{nullptr} {
	removeTabContents(tr("In development"));
}

void SettingsTab::lock() {

}

void SettingsTab::unlock() {

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

void SettingsTab::onDataReceived(const QJsonObject&) {

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

	connect(button, &QPushButton::clicked, button, [&]() {

		if (serverAddress->text().isEmpty()) {
			QMessageBox mb{QMessageBox::Critical, tr("Server address is not set."),
						   tr("You need to set the server address."), QMessageBox::Ok};
			mb.exec();
			return;
		}

		if (serverPort->text().isEmpty()) {
			QMessageBox mb{QMessageBox::Critical, tr("Server port is not set."),
						   tr("You need to set the server port."), QMessageBox::Ok};
			mb.exec();
			return;
		}

		bool portConversionOk;
		int port = serverPort->text().toInt(&portConversionOk);

		if (!portConversionOk || port <= 0) {
			QMessageBox mb{QMessageBox::Critical, tr("Server port is incorrect."),
						   tr("You need to set correct server port."), QMessageBox::Ok};
			mb.exec();
			return;
		}

		if (login->text().isEmpty()) {
			QMessageBox mb{QMessageBox::Critical, tr("Login is not set."),
						   tr("You need to set the login."), QMessageBox::Ok};
			mb.exec();
			return;
		}

		if (password->text().isEmpty()) {
			QMessageBox mb{QMessageBox::Critical, tr("Password is not set."),
						   tr("You need to set the password."), QMessageBox::Ok};
			mb.exec();
			return;
		}

		emit authorizationClicked(serverAddress->text(), port, login->text(), password->text());
	});
}

void SettingsTab::removeTabContents(const QString& /*text*/) {

}

void SettingsTab::load(QSettings& settings) {
//	serverAddress->setText(settings.value("serverAddress", "").toString());
//	serverPort->setText(settings.value("serverPort").toString());
//	login->setText(settings.value("login").toString());
//	password->setText(settings.value("password").toString());
}

void SettingsTab::save(QSettings& settings) {
//	settings.setValue("serverAddress", serverAddress->text());
//	settings.setValue("serverPort", serverPort->text());
//	settings.setValue("login", login->text());
//	settings.setValue("password", password->text());
}
