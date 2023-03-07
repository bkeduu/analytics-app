#include "itab.h"

ITab::ITab(QWidget* parent, const QString& name) : QWidget(parent), tabName(name) { }

QString ITab::getName() const {
	return tabName;
}
