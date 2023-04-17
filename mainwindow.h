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

private:
	void parseConfig(QFile& conf);

    Ui::MainWindow *ui;
	QTabWidget* tabWidget;
	QDialog* tabDialog;
	QHBoxLayout* layout;

	QString login;
	QString password;

	Networker* connector;

	QMap<Tab, ITab*> tabs;
};
