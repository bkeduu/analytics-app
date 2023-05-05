#pragma once

#include "itab.h"

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QString>

class SettingsTab : public ITab {
	Q_OBJECT
public:
	static SettingsTab* getWidget(const QString& tabName, QWidget* parent = nullptr);

	void load(QSettings& settings) final;
	void save(QSettings& settings) final;

signals:
	void serverAddressUpdated(const QString& newAddress);
	void serverPortUpdated(const QString& newPort);
	void loginUpdated(const QString& newLogin);
	void passwordUpdated(const QString& newpassword);

	void authorizationClicked();

private:
	SettingsTab(const QString& tabName, QWidget* parent = nullptr);
	QGridLayout* layout;

	QLineEdit* serverAddress;
	QLineEdit* serverPort;
	QLineEdit* login;
	QLineEdit* password;

	static SettingsTab* instance;
	static QMutex lock;

	virtual ~SettingsTab() {};
};
