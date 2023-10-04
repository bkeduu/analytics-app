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
#include <QLabel>

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

	double minimumValue = 0;
	double maximumValue = DBL_MIN;

	auto it = axisLabels.begin();

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

	if (barsValues.size()) {
		QVector<double> sums(barsValues[0].size(), .0);

		for (int i = 0; i < barsValues.size(); ++i) {
			QBarSet* barData;
			if (plotsLabels.size() < 3)
				barData = new QBarSet{plotsLabels.front()};
			else
				barData = new QBarSet{plotsLabels[i]};

			barData->setBorderColor(QColor{0, 0, 0, 0});

			for (int j = 0; j < barsValues[i].size(); ++j) {
				if (barsValues[i][j] < 0 && barsValues[i][j] < minimumValue)
					minimumValue = barsValues[i][j];

				(*barData) << barsValues[i][j];
				sums[j] += barsValues[i][j];
			}

			barSeries->append(barData);

			connect(barSeries, &QStackedBarSeries::hovered, barData, [this] (bool hovered, int index, QBarSet* set) {
				static QFrame* tooltip = nullptr;
				static QLabel* textLabel = nullptr;

				if (hovered) {  // cursor entered the bar area
					if (!tooltip) {
						tooltip = new QFrame{this};
						tooltip->setFrameShape(QFrame::Box);
						tooltip->setBaseSize(QSize{400, 200});
						tooltip->setStyleSheet("background-color: #dcded3; border-radius: 2px;");

						textLabel = new QLabel{this};

						QHBoxLayout* layout = new QHBoxLayout{tooltip};
						layout->addWidget(textLabel, 1, Qt::AlignCenter);
						layout->setContentsMargins(0, 0, 0, 0);
						tooltip->setLayout(layout);
					}

					textLabel->setText(QString{"%1 %2"}.arg(set->at(index)).arg(mYAxisTitle));
					tooltip->move(mapFromGlobal(QCursor::pos()));
					tooltip->show();
				}
				else {
					if (tooltip) {
						tooltip->hide();
					}
				}
			});
		}

		maximumValue = *std::max_element(sums.begin(), sums.end());
	}

	for (int i = 0; i < lineValues.size(); ++i) {
		QLineSeries* lineSeries = new QLineSeries;

		if (plotsLabels.size() < 3)
			lineSeries->setName(plotsLabels.back());
		else
			lineSeries->setName(plotsLabels[i + (barsValues.size() ? 1 : 0)]);

		mChart->addSeries(lineSeries);

		for (int j = 0; j < lineValues[i].size(); ++j) {
			if (lineValues[i][j] > maximumValue)
				maximumValue = lineValues[i][j];
			else if (lineValues[i][j] < 0 && lineValues[i][j] < minimumValue)
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
