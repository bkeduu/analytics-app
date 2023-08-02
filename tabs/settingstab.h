#pragma once

#include "itab.h"

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QString>

class SettingsTab final : public ITab {
	Q_OBJECT
public:
	SettingsTab(const QString& tabName, QWidget* parent = nullptr);

	virtual void createTabContents() final override;
	virtual void removeTabContents(const QString& text = tr("You need to authorize before starting")) final override;

	void load(QSettings& settings) final override;
	void save(QSettings& settings) final override;

	virtual ~SettingsTab() final override {};

signals:
	void serverAddressUpdated(const QString& newAddress);
	void serverPortUpdated(const QString& newPort);
	void loginUpdated(const QString& newLogin);
	void passwordUpdated(const QString& newpassword);

	void authorizationClicked(const QString& serverAddress, int serverPort,
								const QString& login, const QString& password);

public slots:
	virtual void onAuthorized() final override;
	virtual void onTabOpened() final override;
	virtual void onDataReceived(const QJsonObject&) final override;

private:
	QLayout* layout;

	QLineEdit* serverAddress;
	QLineEdit* serverPort;
	QLineEdit* login;
	QLineEdit* password;

	QPushButton* button;
};
