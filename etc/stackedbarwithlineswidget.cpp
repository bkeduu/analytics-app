#include "stackedbarwithlineswidget.h"

#include <cfloat>

#include <QChart>
#include <QChartView>
#include <QGraphicsLayout>
#include <QGridLayout>
#include <QLineSeries>
#include <QBarCategoryAxis>
#include <QStackedBarSeries>
#include <QBarSet>
#include <QValueAxis>

StackedBarWithLinesWidget::StackedBarWithLinesWidget(QWidget *parent, const QString& title,
	const QString& xAxisTitle, const QString& yAxisTitle)
	: ContentWidget{parent}, mXAxis{nullptr}, mYAxis{nullptr}, mXAxisTitle{xAxisTitle}, mYAxisTitle{yAxisTitle} {
    // Create chart view with the chart
	mChart = new QChart;
	mChartView = new QChartView(mChart, this);

    // Create layout for grid and detached legend
	mMainLayout = new QGridLayout;
	mMainLayout->addWidget(mChartView, 0, 1, 3, 1);
	setLayout(mMainLayout);

	mChart->setTitle(title);
	mChart->legend()->setVisible(true);
	mChart->legend()->setAlignment(Qt::AlignBottom);

	mChartView->setRenderHint(QPainter::Antialiasing);
	mChart->layout()->setContentsMargins(0, 0, 0, 0);
}

void StackedBarWithLinesWidget::onGraphicsData(const QStringList& axisLabels, const QStringList& plotsLabels,
				const QVector<QVector<double>>& barsValues, const QVector<QVector<double>>& lineValues) {
	mChart->removeAllSeries();

	if (mXAxis)
		mChart->removeAxis(mXAxis);

	if (mYAxis)
		mChart->removeAxis(mYAxis);

	double minimumValue = DBL_MAX;
	double maximumValue = DBL_MIN;  // TODO расчёт максимума неправильный

	auto it = plotsLabels.begin();

	mXAxis = new QBarCategoryAxis;
	mXAxis->append(axisLabels);
	mXAxis->setRange(axisLabels.first(), axisLabels.last());
	mXAxis->setTitleText(mXAxisTitle);
	mChart->addAxis(mXAxis, Qt::AlignBottom);

	mYAxis = new QValueAxis;
	mYAxis->setTitleText(mYAxisTitle);
	mChart->addAxis(mYAxis, Qt::AlignLeft);

	QStackedBarSeries* barSeries = new QStackedBarSeries;
	mChart->addSeries(barSeries);
	barSeries->attachAxis(mYAxis);
	barSeries->attachAxis(mXAxis);

	QVector<double> sums(barsValues[0].size(), .0);

	for (int i = 0; i < barsValues.size(); ++i) {
		QBarSet* barData = new QBarSet{*it++};

		for (int j = 0; j < barsValues[i].size(); ++j) {
			if (barsValues[i][j] < minimumValue)
				minimumValue = barsValues[i][j];

			(*barData) << barsValues[i][j];
			sums[j] += barsValues[i][j];
		}

		barSeries->append(barData);
	}

	maximumValue = *std::max_element(sums.begin(), sums.end());

	for (int i = 0; i < lineValues.size(); ++i) {
		QLineSeries* lineSeries = new QLineSeries;
		lineSeries->setName(*it++);
		mChart->addSeries(lineSeries);

		for (int j = 0; j < lineValues[i].size(); ++j) {
			if (lineValues[i][j] > maximumValue)
				maximumValue = lineValues[i][j];
			else if (lineValues[i][j] < minimumValue)
				minimumValue = lineValues[i][j];

			lineSeries->append(QPoint{j, static_cast<int>(lineValues[i][j])});
		}

		lineSeries->attachAxis(mXAxis);
		lineSeries->attachAxis(mYAxis);
	}

	minimumValue = (minimumValue == DBL_MIN) ? 0 : minimumValue;
	maximumValue = (maximumValue == DBL_MAX) ? 0 : (1.2 * maximumValue);
	mYAxis->setRange(minimumValue, maximumValue);
	mYAxis->applyNiceNumbers();
}
