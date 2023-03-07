#pragma once

#include "itab.h"

#include <QMainWindow>
#include <QTabWidget>
#include <QTabBar>
#include <QMap>
#include <QResizeEvent>
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

private:
    Ui::MainWindow *ui;
	QTabWidget* tabWidget;
	QDialog* tabDialog;
	QGridLayout* layout;

	QMap<Tab, ITab*> tabs;
};
