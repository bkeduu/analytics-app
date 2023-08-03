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
	virtual void resizeEvent(QResizeEvent* event) final override;  // legacy
	virtual ~MainWindow() final override;

	void authorize();  // legacy
	void onRelayClicked(int group, bool newState);  // legacy

	void setChangingTitle(QString newTitle) {  // rewrite
		if (currentWindowTitle == tr("Analytics system"))
			windowTitle = newTitle;
		else
			currentWindowTitle = newTitle;
	}


	void load(QSettings& settings);
	void save(QSettings& settings);

public slots:
	void onAuthorized(const QJsonObject&);
	void onESPStatusChanged(const QJsonObject&);
	//void onConsumersReceived(const QJsonObject&);
	void onDisconnect();

private:
	void createMainContents();
	void createStartScreen();
	void clearScreen();

    Ui::MainWindow *ui;
	QTabWidget* tabWidget;
	QDialog* tabDialog;
	QLayout* layout;

	QTimer* windowTitleChangingTimer;
	QString windowTitle;
	QString currentWindowTitle;

	Client* mClient;

	QSharedPointer<StatusTab> mStatusTab;
	QSharedPointer<ForecastTab> mForecastTab;
	QSharedPointer<GenerationTab> mGenerationTab;
	QSharedPointer<ConsumersTab> mConsumersTab;
	QSharedPointer<SettingsTab> mSettingsTab;
};
