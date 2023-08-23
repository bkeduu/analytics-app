#pragma once

#include "contentwidget.h"

#include <QJsonObject>

QT_FORWARD_DECLARE_CLASS(QChart)
QT_FORWARD_DECLARE_CLASS(QChartView)
QT_FORWARD_DECLARE_CLASS(QGridLayout)
QT_FORWARD_DECLARE_CLASS(QLineSeries)
QT_FORWARD_DECLARE_CLASS(QBarCategoryAxis)
QT_FORWARD_DECLARE_CLASS(QValueAxis)

class StackedBarWithLinesWidget : public ContentWidget {
    Q_OBJECT

public:
	StackedBarWithLinesWidget(QWidget *parent = nullptr, const QString& title = "", const QString& xAxisTitle = "", const QString& yAxisTitle = "");

public slots:
	void onGraphicsData(const QStringList& labels, const QStringList& plotsLabels,
						const QVector<QVector<double>>& barsValues, const QVector<QVector<double>>& lineValues);

private:
	QChart* mChart;
	QChartView* mChartView;
	QGridLayout* mMainLayout;
	QGridLayout* mFontLayout;
	QBarCategoryAxis* mXAxis;
	QValueAxis* mYAxis;

	const QString mXAxisTitle;
	const QString mYAxisTitle;
};
