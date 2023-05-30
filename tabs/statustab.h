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

enum BatteryInformation {
	Charging,
	Discharging,
	NoData
};

class StatusTab : public ITab {
	Q_OBJECT

public:
	static StatusTab* getWidget(const QString& tabName, QWidget* parent = nullptr);

public slots:
	void onDataReceived(const QJsonObject& values);
	virtual void onAuthorized() final override;

private:
	enum class TabWidget { Generation, Consumers, WorkMode, BatteryStatus };

	StatusTab(const QString& tabName, QWidget* parent = nullptr);

	QLabel* createLabel(QWidget* parent, const QString& text, const QSizePolicy& sp) const;
	QLabel* createLabel(QWidget* parent, const QString& imagePath, const QSizePolicy& sp, const QSize& sz) const;
	QProgressBar* createProgressBar(QWidget* parent, const QSize& minSize) const;

	virtual void createTabContents() final override;
	virtual void removeTabContents(const QString& = tr("You need to authorize before starting")) final override;

	QFrame* generators,* consumers,* modes,* battery;

	QLayout* layout;

	static StatusTab* instance;
	static QMutex lock;

	QFrame* createWidget(TabWidget widgetType, QWidget* parent);

	QHash<GeneratorsStatus, QWidget*> generatorsLabels;
	QHash<ConsumersStatus, QWidget*> consumersLabels;
	QHash<BatteryStatus, QWidget*> batteryLabels;

	virtual ~StatusTab() {};
};
