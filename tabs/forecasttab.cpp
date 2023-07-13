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
	removeTabContents(tr("In development"));
}

void ForecastTab::onAuthorized() {

}

void ForecastTab::onTabOpened() {

}

void ForecastTab::createTabContents() {
  // todo
}

void ForecastTab::removeTabContents(const QString& text) {
	clearTab();
	delete this->layout;
	layout = new QGridLayout{this};
	this->setLayout(layout);
	layout->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

	QLabel* textLabel = new QLabel{this};
	textLabel->setProperty("class", "tab-standalone-text");
	textLabel->setText(text);
	textLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	textLabel->setAlignment(Qt::AlignCenter);

	layout->addWidget(textLabel);
}
