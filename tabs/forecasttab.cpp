#include "forecasttab.h"

ForecastTab* ForecastTab::instance = nullptr;
QMutex ForecastTab::lock{};

ForecastTab* ForecastTab::getWidget(const QString& tabName, QWidget* parent) {
	QMutexLocker locker(&lock);
		if (!instance)
			instance = new ForecastTab(tabName, parent);
		return instance;
}

ForecastTab::ForecastTab(const QString& tabName, QWidget* parent) : ITab(parent, tabName), layout(nullptr) {
	layout = new QGridLayout(this);

	layout->addWidget(new QPushButton("1", this), 0, 1);
	layout->addWidget(new QCheckBox("2", this), 0, 2);
	layout->addWidget(new QCheckBox("3", this), 0, 3);
	layout->addWidget(new QPushButton("4", this), 0, 4);
}
