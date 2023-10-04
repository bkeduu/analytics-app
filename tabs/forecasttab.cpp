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

void ForecastTab::onGraphicsDataReceived(const QJsonObject& data) {
	if (!mPlot)
		createTabContents();

	if (!data.contains("prediction") || !data.contains("reality"))
		throw InternalErrorException{tr("Data structure with wrong value received at %1. The app will be closed.").arg(FLF)};

	QJsonArray predictionData = data.value("prediction").toArray();

	static const QStringList plotLabels{tr("Generation"), tr("Consumers")};
	QStringList labels;

	QVector<QVector<double>> generatorValues{3, {}};
	QVector<QVector<double>> consumersValues{{}};

	for (auto it = predictionData.begin(); it != predictionData.end(); ++it) {
		QJsonArray value = (*it).toArray();

		labels << value.at(0).toString("");

		generatorValues[0] << value.at(1).toDouble(.0);
		consumersValues[0] << value.at(2).toDouble(.0);
	}

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
