#pragma once

#include "itab.h"

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QString>

class SettingsTab : public ITab {
	Q_OBJECT
public:
	static SettingsTab* getWidget(const QString& tabName, QWidget* parent = nullptr);

private:
	SettingsTab(const QString& tabName, QWidget* parent = nullptr);
	QGridLayout* layout;

	static SettingsTab* instance;
	static QMutex lock;

	virtual ~SettingsTab() {};
};
