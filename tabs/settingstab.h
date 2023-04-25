#pragma once

#include "itab.h"

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QString>

class SettingsTab : public ITab {
	Q_OBJECT
public:
	static SettingsTab* getWidget(const QString& tabName, QWidget* parent = nullptr);

signals:
	void serverAddressUpdated(const QString& newAddress);
	void serverPortUpdated(const QString& newPort);
	void loginUpdated(const QString& newLogin);
	void passwordUpdated(const QString& newpassword);

	void authorizationClicked();

private:
	SettingsTab(const QString& tabName, QWidget* parent = nullptr);
	QGridLayout* layout;

	static SettingsTab* instance;
	static QMutex lock;

	virtual ~SettingsTab() {};
};
