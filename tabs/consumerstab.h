#pragma once

#include "itab.h"

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QString>

class ConsumersTab : public ITab {
	Q_OBJECT
public:
	static ConsumersTab* getWidget(const QString& tabName, QWidget* parent = nullptr);

private:
	ConsumersTab(const QString& tabName, QWidget* parent = nullptr);
	QGridLayout* layout;

	static ConsumersTab* instance;
	static QMutex lock;

	virtual ~ConsumersTab() {};
};
