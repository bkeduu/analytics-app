#include "itab.h"
#include "mainwindow.h"

ITab::ITab(QWidget* parent, const QString& name) : QFrame{parent}, tabName{name},
	mParent{dynamic_cast<MainWindow*>(parent)} { }

QString ITab::getName() const {
	return tabName;
}
