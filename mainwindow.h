#pragma once

#include "connection.h"

#include "statustab.h"
#include "consumerstab.h"
#include "generationtab.h"
#include "forecasttab.h"
#include "settingstab.h"

#include <QMainWindow>
#include <QTabWidget>
#include <QSettings>
#include <QTabBar>
#include <QMap>
#include <QFile>
#include <QJsonDocument>
#include <QResizeEvent>
#include <QVBoxLayout>
#include <QSharedPointer>

class Client;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow final : public QMainWindow {
    Q_OBJECT

public:
	MainWindow(Client* client, QWidget *parent = nullptr);  // legacy
	virtual ~MainWindow() final override;

	void onRelayClicked(int group, bool newState);  // legacy

	virtual void resizeEvent(QResizeEvent* event) final override;

	void load(QSettings& settings);
	void save(QSettings& settings);

signals:
	void authorize(QString login, QString password, QString serverAddress, int serverPort);
	void relayClicked(int group, bool newState);
	void modeChanged(int);

public slots:
	void onAuthorize(bool status);
	void onESPStatusChange(const QJsonObject& data);
	void onConsumersData(const QJsonObject& data);
	void onSensorsData(const QJsonObject& data);
	void onModeSwitch(int newMode);
	void onGraphicsData(const QJsonObject& data);

	void onServerLookupFail();
	void onUnableToConnect();
	void onConnect();
	void onDisconnect();

	void onModeChange(int);

private:
	QWidget* createMainContents();
	QWidget* createStartScreen();

	void lockTabs();
	void unlockTabs();

    Ui::MainWindow *ui;
	QWidget* mMainContent;
	QWidget* mStartScreen;
	CustomLayout* layout;
	QWidget* tabWidget;

	Client* mClient;

	QLabel* mAuthLabel;

	CustomLineEdit* mLoginField;
	CustomLineEdit* mPasswordField;
	CustomLineEdit* mServerAddressField;
	CustomLineEdit* mServerPortField;

	QSharedPointer<StatusTab> mStatusTab;
	QSharedPointer<ForecastTab> mForecastTab;
	QSharedPointer<GenerationTab> mGenerationTab;
	QSharedPointer<ConsumersTab> mConsumersTab;
	QSharedPointer<SettingsTab> mSettingsTab;
};
