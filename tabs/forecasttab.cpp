#include "forecasttab.h"

#include <QJsonArray>
#include <QVector>

ForecastTab::ForecastTab(const QString& tabName, QWidget* parent) : ITab{parent, tabName}, mPlot{nullptr}, mLayout{nullptr}{
	removeTabContents(tr("Waiting for forecast data from server"));
}

void ForecastTab::lock() {

}

void ForecastTab::unlock() {

}

void ForecastTab::onForecastDataReceived(const QJsonObject& data) {
	if (!mPlot)
		createTabContents();

	if (!data.contains("forecast_dates") || !data.contains("forecast") || !data.contains("forecast_dates"))
		throw InternalErrorException{QString{"Data structure with wrong value received at %1. The app will be closed."}.arg(FLF)};

	static const QStringList plotLabels{tr("Solar"), tr("Wind"), tr("Diesel"), tr("Consumers")};

	QJsonArray labelsArray = data.value("forecast_dates").toArray();
	QStringList labels;

	for (auto it = labelsArray.begin(); it != labelsArray.end(); ++it)
		labels << (*it).toString();

	QJsonObject forecastData = data.value("forecast").toObject();

	if (!forecastData.contains("solar") || !forecastData.contains("wind") || !forecastData.contains("diesel"))
		throw InternalErrorException{QString{"Data structure with wrong value received at %1. The app will be closed."}.arg(FLF)};

	QVector<QVector<double>> generatorValues(3, {});

	QJsonArray solarData = forecastData.value("solar").toArray();
	QJsonArray windData = forecastData.value("wind").toArray();
	QJsonArray dieselData = forecastData.value("diesel").toArray();

	if (solarData.size() != labels.size() || windData.size() != labels.size() || dieselData.size() != labels.size())
		throw InternalErrorException{QString{"Data structure with wrong value received at %1. The app will be closed."}.arg(FLF)};

	for (int i = 0; i < solarData.size(); ++i)
		generatorValues[0].push_back(solarData[i].toDouble());

	for (int i = 0; i < windData.size(); ++i)
		generatorValues[1].push_back(windData[i].toDouble());

	for (int i = 0; i < dieselData.size(); ++i)
		generatorValues[2].push_back(dieselData[i].toDouble());

	QJsonArray consumersData = forecastData.value("consumers").toArray();

	if (consumersData.size() != labels.size())
		throw InternalErrorException{QString{"Data structure with wrong value received at %1. The app will be closed."}.arg(FLF)};

	QVector<QVector<double>> consumersValues(1, {});

	for (int i = 0; i < consumersData.size(); ++i)
		consumersValues[0].push_back(consumersData[i].toDouble());

	mPlot->onGraphicsData(labels, plotLabels, generatorValues, consumersValues);
	mPlot->resize(size());
}

void ForecastTab::createTabContents() {
	removeTabContents();

	mLayout = new QHBoxLayout{this};
	mPlot = new StackedBarWithLinesWidget{this, "", "", "kW"};  // TODO power units
	mLayout->addWidget(mPlot);
	setLayout(mLayout);
}

void ForecastTab::removeTabContents(const QString& text) {
	clearTab();
	delete mLayout;

	if (text.length()) {
		mLayout = new QGridLayout{this};
		setLayout(mLayout);
		mLayout->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		QLabel* textLabel = new QLabel{this};
		textLabel->setProperty("class", "tab-standalone-text");
		textLabel->setText(text);
		textLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
		textLabel->setAlignment(Qt::AlignCenter);

		mLayout->addWidget(textLabel);
	}
}

void ForecastTab::showEvent(QShowEvent*) {
	if (mPlot)
		mPlot->show();
}

void ForecastTab::hideEvent(QHideEvent*) {
	if (mPlot)
		mPlot->hide();
}
