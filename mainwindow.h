#pragma once

#include "itab.h"
#include "networking/connection.h"

#include <QMainWindow>
#include <QTabWidget>
#include <QTabBar>
#include <QMap>
#include <QFile>
#include <QJsonDocument>
#include <QResizeEvent>
#include <QVBoxLayout>
#include <QLayout>

enum class Tab {
	Status = 1,
	Forecast,
	Generation,
	Consumers,
	Settings
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
	virtual void resizeEvent(QResizeEvent* event);
    ~MainWindow();

	void authorize();
	void onRelayClicked(int group, bool newState);

	void setChangingTitle(QString newTitle) {
		if (currentWindowTitle == tr("Analytics system"))
			windowTitle = newTitle;
		else
			currentWindowTitle = newTitle;
	}

public slots:
	void onAuthorized(const QJsonObject&);
	void onESPStatusChanged(const QJsonObject&);
	//void onConsumersReceived(const QJsonObject&);

private:
	void parseConfig(QFile& conf);

    Ui::MainWindow *ui;
	QTabWidget* tabWidget;
	QDialog* tabDialog;
	QHBoxLayout* layout;

	QString serverAddress;
	QString serverPort;
	QString login;
	QString password;

	bool mAuthorized;
	bool mESPConnected;

	QTimer* windowTitleChangingTimer;
	QString windowTitle;
	QString currentWindowTitle;

	Networker* connector;

	QMap<Tab, ITab*> tabs;
};
