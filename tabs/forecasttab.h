#pragma once

#include "itab.h"

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QString>

class ForecastTab : public ITab {
	Q_OBJECT
public:
	static ForecastTab* getWidget(const QString& tabName, QWidget* parent = nullptr);

private:
	ForecastTab(const QString& tabName, QWidget* parent = nullptr);
	QGridLayout* layout;

	static ForecastTab* instance;
	static QMutex lock;

	virtual ~ForecastTab() {};
};

