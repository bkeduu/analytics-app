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

	virtual void createTabContents() final override;
	virtual void removeTabContents(const QString& text = tr("You need to authorize before starting"));

	void load(QSettings& settings) final;
	void save(QSettings& settings) final;

signals:
	void serverAddressUpdated(const QString& newAddress);
	void serverPortUpdated(const QString& newPort);
	void loginUpdated(const QString& newLogin);
	void passwordUpdated(const QString& newpassword);

	void authorizationClicked();

public slots:
	virtual void onAuthorized() final override;

private:
	SettingsTab(const QString& tabName, QWidget* parent = nullptr);
	QLayout* layout;

	QLineEdit* serverAddress;
	QLineEdit* serverPort;
	QLineEdit* login;
	QLineEdit* password;

	static SettingsTab* instance;
	static QMutex lock;

	virtual ~SettingsTab() {};
};
