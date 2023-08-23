#pragma once

#include <QGridLayout>
#include <QLabel>
#include <QFile>
#include <QRadioButton>
#include <QJsonObject>
#include <QProgressBar>

#include "itab.h"

enum BatteryInformation {
	Charging,
	Discharging,
	NoData
};

class StatusTab final : public ITab {
	Q_OBJECT

public:
	StatusTab(const QString& tabName, QWidget* parent = nullptr);
	virtual ~StatusTab() final override {};

	virtual void lock() final override;
	virtual void unlock() final override;

	void switchMode(int newMode);

signals:
	void modeChanged(int);

public slots:
	virtual void onAuthorized() final override;
	virtual void onDataReceived(const QJsonObject&) final override;
	void onButtonModeChange(int);

private:
	enum class TabWidget { Generation, Consumers, WorkMode, BatteryStatus };

	QLabel* createLabel(QWidget* parent, const QString& text) const;
	QLabel* createLabel(QWidget* parent, const QString& imagePath, const QSize& sz) const;
	QProgressBar* createProgressBar(QWidget* parent, const QSize& minSize) const;
	QWidget* createWidget(TabWidget widgetType, QWidget* parent);

	virtual void createTabContents() final override;
	virtual void removeTabContents(const QString& = tr("You need to authorize before starting")) final override;

	QWidget* mGenerators,* mConsumers,* mModes,* mBattery;
	QLayout* mLayout;
	QButtonGroup* mButtonGroup;

	QHash<QString, QWidget*> mWidgetLocator;
};
