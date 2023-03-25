#include "itab.h"

ITab::ITab(QWidget* parent, const QString& name) : QFrame{parent}, tabName{name} { }

QString ITab::getName() const {
	return tabName;
}
