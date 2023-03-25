#include "forecasttab.h"

ForecastTab* ForecastTab::instance = nullptr;
QMutex ForecastTab::lock{};

ForecastTab* ForecastTab::getWidget(const QString& tabName, QWidget* parent) {
	QMutexLocker locker{&ForecastTab::lock};
		if (!instance)
			instance = new ForecastTab{tabName, parent};
		return instance;
}

ForecastTab::ForecastTab(const QString& tabName, QWidget* parent) : ITab{parent, tabName}, layout{nullptr} {
	layout = new QGridLayout{this};


}
