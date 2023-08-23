#pragma once

#include "itab.h"
#include "etc/stackedbarwithlineswidget.h"

#include <QGridLayout>
#include <QLabel>

class ForecastTab final : public ITab {
	Q_OBJECT
public:
	ForecastTab(const QString& tabName, QWidget* parent = nullptr);

	virtual void createTabContents() final override;
	virtual void removeTabContents(const QString& text = "") final override;

	virtual void lock() final override;
	virtual void unlock() final override;

	virtual ~ForecastTab() final override {};

public slots:
	virtual void onAuthorized() final override;
	virtual void onDataReceived(const QJsonObject&) final override;

private:
	StackedBarWithLinesWidget* mPlot;
	QLayout* mLayout;
};

