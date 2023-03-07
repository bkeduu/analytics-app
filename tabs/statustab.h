#pragma once

#include "itab.h"

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QString>

class StatusTab : public ITab {
	Q_OBJECT
public:
	static StatusTab* getWidget(const QString& tabName, QWidget* parent = nullptr);

private:
	StatusTab(const QString& tabName, QWidget* parent = nullptr);
	QGridLayout* layout;

	static StatusTab* instance;
	static QMutex lock;

	virtual ~StatusTab() {};
};
