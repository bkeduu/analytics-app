#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QTableView>
#include <QFrame>
#include <QFile>
#include <QString>
#include <QRadioButton>
#include <QJsonObject>
#include <QProgressBar>

#include "itab.h"

enum GeneratorsStatus {
	SolarVoltage,
	SolarCurrent,
	SolarPower,
	SolarProgress,
	WindVoltage,
	WindCurrent,
	WindPower,
	WindProgress,
	DieselVoltage,
	DieselCurrent,
	DieselPower,
	DieselProgress
};

enum ConsumersStatus {
	FirstVoltage,
	FirstCurrent,
	FirstPower,
	FirstProgress,
	SecondVoltage,
	SecondCurrent,
	SecondPower,
	SecondProgress,
	ThirdVoltage,
	ThirdCurrent,
	ThirdPower,
	ThirdProgress
};

enum BatteryStatus {
	BatteryVoltage,
	BatteryInfo,
	BatteryProgress
};

class StatusTab : public ITab {
	Q_OBJECT

public:
	static StatusTab* getWidget(const QString& tabName, QWidget* parent = nullptr);

public slots:
	void onDataReceived(const QJsonObject& values);

private:
	enum class TabWidget { Generation, Consumers, WorkMode, BatteryStatus };

	StatusTab(const QString& tabName, QWidget* parent = nullptr);

	QLabel* createLabel(QWidget* parent, const QString& text, const QSizePolicy& sp) const;
	QLabel* createLabel(QWidget* parent, const QString& imagePath, const QSizePolicy& sp, const QSize& sz) const;
	QProgressBar* createProgressBar(QWidget* parent, const QSize& minSize) const;

	QFrame* generators,* consumers,* modes,* battery;

	QGridLayout* layout;

	static StatusTab* instance;
	static QMutex lock;

	QFrame* createWidget(TabWidget widgetType, QWidget* parent);

	QHash<GeneratorsStatus, QWidget*> generatorsLabels;
	QHash<ConsumersStatus, QWidget*> consumersLabels;
	QHash<BatteryStatus, QWidget*> batteryLabels;

	virtual ~StatusTab() {};
};
