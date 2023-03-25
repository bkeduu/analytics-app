#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QTableView>
#include <QFrame>
#include <QString>
#include <QRadioButton>
#include <QProgressBar>

#include "itab.h"

class StatusTab : public ITab {
	Q_OBJECT
public:
	static StatusTab* getWidget(const QString& tabName, QWidget* parent = nullptr);

private:
	enum class TabWidget { Generation, Consumers, WorkMode, BatteryStatus };

	StatusTab(const QString& tabName, QWidget* parent = nullptr);

	QLabel* createLabel(QWidget* parent, const QString& text, const QSizePolicy& sp);
	QLabel* createLabel(QWidget* parent, const QString& imagePath, const QSizePolicy& sp, const QSize& sz);

	QFrame* generators,* consumers,* modes,* battery;

	QGridLayout* layout;

	static StatusTab* instance;
	static QMutex lock;

	QFrame* createWidget(TabWidget widgetType, QWidget* parent);

	virtual ~StatusTab() {};
};
