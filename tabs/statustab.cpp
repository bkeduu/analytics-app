#include "statustab.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QGroupBox>
#include <QResizeEvent>

StatusTab* StatusTab::instance = nullptr;
QMutex StatusTab::lock{};

StatusTab* StatusTab::getWidget(const QString& tabName, QWidget* parent) {
	QMutexLocker locker{&StatusTab::lock};
	if (!instance)
		instance = new StatusTab{tabName, parent};
	return instance;
}

StatusTab::StatusTab(const QString& tabName, QWidget* parent) : ITab{parent, tabName},
	generators{nullptr}, consumers{nullptr}, layout{nullptr} {
	removeTabContents();
}

void StatusTab::onAuthorized() {
	createTabContents();
}

void StatusTab::onTabOpened() {

}

void StatusTab::createTabContents() {
	clearTab();
	delete this->layout;
	QGridLayout* gridLayout = new QGridLayout{this};
	gridLayout->setContentsMargins(15, 15, 15, 15);
	gridLayout->setSpacing(20);
	this->layout = gridLayout;
	this->setLayout(gridLayout);

	generators = createWidget(TabWidget::Generation, this);
	generators->setSizePolicy(QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});
	consumers = createWidget(TabWidget::Consumers, this);
	consumers->setSizePolicy(QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});
	modes = createWidget(TabWidget::WorkMode, this);
	modes->setSizePolicy(QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});
	battery = createWidget(TabWidget::BatteryStatus, this);
	battery->setSizePolicy(QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});

	gridLayout->addWidget(generators, 0, 0);
	gridLayout->addWidget(modes, 0, 1);
	gridLayout->addWidget(consumers, 1, 0);
	gridLayout->addWidget(battery, 1, 1);

	gridLayout->setColumnStretch(0, 2);
	gridLayout->setColumnStretch(1, 1);
}

void StatusTab::removeTabContents(const QString& text) {
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

QLabel* StatusTab::createLabel(QWidget* parent, const QString& text) const {
	QLabel* result = new QLabel{parent};
	result->setText(text);
	result->setSizePolicy(QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});
	result->setMargin(0);
	result->setIndent(0);
	result->setContentsMargins(0, 0, 0, 0);
	result->setAlignment(Qt::AlignCenter);
	result->setMaximumSize(QSize{90, 30});
	return result;
}

QLabel* StatusTab::createLabel(QWidget* parent, const QString& imagePath, const QSize& sz) const {
	QLabel* result = new QLabel{parent};
	QPixmap image{imagePath};
	image = image.scaled(sz);
	result->setPixmap(image);
	result->setSizePolicy(QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});
	result->setMargin(0);
	result->setIndent(0);
	result->setContentsMargins(0, 0, 0, 0);
	result->setAlignment(Qt::AlignCenter);
	result->setMaximumSize(QSize{90, 30});
	return result;
}

QProgressBar* StatusTab::createProgressBar(QWidget* parent, const QSize& minSize) const {
	QProgressBar* result = new QProgressBar{parent};

	static QString progressBarStylesheet = "";
	if (!progressBarStylesheet.length()) {
		QFile stylesheet{":/static/stylesheets/progressbar.css"};
		stylesheet.open(QIODevice::ReadOnly | QIODevice::Text);
		progressBarStylesheet = stylesheet.readAll();
	}

	result->setValue(0);
	result->setTextVisible(true);
	result->setMinimumSize(minSize);
	result->setStyleSheet(progressBarStylesheet);

	return result;
}

void StatusTab::onDataReceived(const QJsonObject& dataObject) {

	QJsonArray solarArray = dataObject.value("solar").toArray();
	dynamic_cast<QLabel*>(generatorsLabels[SolarVoltage])->setText(tr("V: %1").arg(solarArray[0].toDouble()));
	dynamic_cast<QLabel*>(generatorsLabels[SolarCurrent])->setText(tr("A: %1").arg(solarArray[1].toDouble()));
	dynamic_cast<QLabel*>(generatorsLabels[SolarPower])->setText(tr("W: %1").arg(solarArray[2].toDouble()));
	dynamic_cast<QProgressBar*>(generatorsLabels[SolarProgress])->setValue(solarArray[3].toInt());

	QJsonArray windArray = dataObject.value("wind").toArray();
	dynamic_cast<QLabel*>(generatorsLabels[WindVoltage])->setText(tr("V: %1").arg(windArray[0].toDouble()));
	dynamic_cast<QLabel*>(generatorsLabels[WindCurrent])->setText(tr("A: %1").arg(windArray[1].toDouble()));
	dynamic_cast<QLabel*>(generatorsLabels[WindPower])->setText(tr("W: %1").arg(windArray[2].toDouble()));
	dynamic_cast<QProgressBar*>(generatorsLabels[WindProgress])->setValue(windArray[3].toInt());

	QJsonArray dieselArray = dataObject.value("gen").toArray();
	dynamic_cast<QLabel*>(generatorsLabels[DieselVoltage])->setText(tr("V: %1").arg(dieselArray[0].toDouble()));
	dynamic_cast<QLabel*>(generatorsLabels[DieselCurrent])->setText(tr("A: %1").arg(dieselArray[1].toDouble()));
	dynamic_cast<QLabel*>(generatorsLabels[DieselPower])->setText(tr("W: %1").arg(dieselArray[2].toDouble()));
	dynamic_cast<QProgressBar*>(generatorsLabels[DieselProgress])->setValue(dieselArray[3].toInt());

	QJsonArray batteryArray = dataObject.value("bat").toArray();
	dynamic_cast<QLabel*>(batteryLabels[BatteryVoltage])->setText(QString{"%1 V"}.arg(batteryArray[0].toDouble()));
	int status = batteryArray[4].toInt();

	switch (BatteryInformation(status)) {
	case Charging:
		dynamic_cast<QLabel*>(batteryLabels[BatteryInfo])->setText(tr("Charging..."));
		break;
	case Discharging:
		dynamic_cast<QLabel*>(batteryLabels[BatteryInfo])->setText(tr("Discharging..."));
		break;
	default:
		dynamic_cast<QLabel*>(batteryLabels[BatteryInfo])->setText(tr("Unknown..."));
		break;
	}

	dynamic_cast<QProgressBar*>(batteryLabels[BatteryProgress])->setValue(batteryArray[1].toInt());

	QJsonArray firstArray = dataObject.value("1").toArray();
	dynamic_cast<QLabel*>(consumersLabels[FirstVoltage])->setText(tr("V: %1").arg(firstArray[0].toDouble()));
	dynamic_cast<QLabel*>(consumersLabels[FirstCurrent])->setText(tr("A: %1").arg(firstArray[1].toDouble()));
	dynamic_cast<QLabel*>(consumersLabels[FirstPower])->setText(tr("W: %1").arg(firstArray[2].toDouble()));
	dynamic_cast<QProgressBar*>(consumersLabels[FirstProgress])->setValue(firstArray[3].toInt());

	QJsonArray secondArray = dataObject.value("2").toArray();
	dynamic_cast<QLabel*>(consumersLabels[SecondVoltage])->setText(tr("V: %1").arg(secondArray[0].toDouble()));
	dynamic_cast<QLabel*>(consumersLabels[SecondCurrent])->setText(tr("A: %1").arg(secondArray[1].toDouble()));
	dynamic_cast<QLabel*>(consumersLabels[SecondPower])->setText(tr("W: %1").arg(secondArray[2].toDouble()));
	dynamic_cast<QProgressBar*>(consumersLabels[SecondProgress])->setValue(secondArray[3].toInt());

	QJsonArray thirdArray = dataObject.value("3").toArray();
	dynamic_cast<QLabel*>(consumersLabels[ThirdVoltage])->setText(tr("V: %1").arg(thirdArray[0].toDouble()));
	dynamic_cast<QLabel*>(consumersLabels[ThirdCurrent])->setText(tr("A: %1").arg(thirdArray[1].toDouble()));
	dynamic_cast<QLabel*>(consumersLabels[ThirdPower])->setText(tr("W: %1").arg(thirdArray[2].toDouble()));
	dynamic_cast<QProgressBar*>(consumersLabels[ThirdProgress])->setValue(thirdArray[3].toInt());
}

QWidget* StatusTab::createWidget(TabWidget widgetType, QWidget* parent) {
	QGroupBox* widget;

	static QString groupBoxStylesheet{""};

	if (!groupBoxStylesheet.length()) {
		QFile stylesheet{":/static/stylesheets/qgroupbox.css"};
		stylesheet.open(QIODevice::ReadOnly);
		groupBoxStylesheet = stylesheet.readAll();
		stylesheet.close();
	}

	switch (widgetType) {
	case TabWidget::Generation: {
		widget = new QGroupBox{parent};
		QWidget* statusLabel = nullptr;

		QVBoxLayout* layout = new QVBoxLayout{widget};
		widget->setLayout(layout);
		widget->setTitle(tr("Generators"));

		widget->setStyleSheet(groupBoxStylesheet);

		QFrame* solarInfoWidget = new QFrame{widget};
		QHBoxLayout* solarInfoLayout = new QHBoxLayout{solarInfoWidget};
		solarInfoWidget->setLayout(solarInfoLayout);
		layout->addWidget(solarInfoWidget, Qt::AlignCenter);

		solarInfoLayout->addWidget(createLabel(solarInfoWidget, ":/static/images/solar-power.png",
											   QSize{30, 30}), 10, Qt::AlignCenter);

		statusLabel = createLabel(solarInfoWidget, QString{"V: -"});
		generatorsLabels[SolarVoltage] = statusLabel;
		solarInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		statusLabel = createLabel(solarInfoWidget, QString{"A: -"});
		generatorsLabels[SolarCurrent] = statusLabel;
		solarInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		statusLabel = createLabel(solarInfoWidget, QString{"W: -"});
		generatorsLabels[SolarPower] = statusLabel;
		solarInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		statusLabel = createProgressBar(solarInfoWidget, QSize{70, 30});
		generatorsLabels[SolarProgress] = statusLabel;
		solarInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		QFrame* windInfoWidget = new QFrame{widget};
		QHBoxLayout* windInfoLayout = new QHBoxLayout{windInfoWidget};
		windInfoWidget->setLayout(windInfoLayout);
		layout->addWidget(windInfoWidget, Qt::AlignCenter);

		windInfoLayout->addWidget(createLabel(windInfoWidget, ":/static/images/wind-turbine.png",
											  QSize{30, 30}), 10, Qt::AlignCenter);

		statusLabel = createLabel(windInfoWidget, QString{"V: -"});
		generatorsLabels[WindVoltage] = statusLabel;
		windInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		statusLabel = createLabel(windInfoWidget, QString{"A: -"});
		generatorsLabels[WindCurrent] = statusLabel;
		windInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		statusLabel = createLabel(windInfoWidget, QString{"W: -"});
		generatorsLabels[WindPower] = statusLabel;
		windInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		statusLabel = createProgressBar(windInfoWidget, QSize{70, 30});
		generatorsLabels[WindProgress] = statusLabel;
		windInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		QFrame* dieselInfoWidget = new QFrame{widget};
		QHBoxLayout* dieselInfoLayout = new QHBoxLayout{dieselInfoWidget};
		dieselInfoWidget->setLayout(dieselInfoLayout);
		layout->addWidget(dieselInfoWidget, Qt::AlignCenter);

		dieselInfoLayout->addWidget(createLabel(dieselInfoWidget, ":/static/images/diesel.png",
												QSize{30, 30}), 10, Qt::AlignCenter);

		statusLabel = createLabel(dieselInfoWidget, QString{"V: -"});
		generatorsLabels[DieselVoltage] = statusLabel;
		dieselInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		statusLabel = createLabel(dieselInfoWidget, QString{"A: -"});
		generatorsLabels[DieselCurrent] = statusLabel;
		dieselInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		statusLabel = createLabel(dieselInfoWidget, QString{"W: -"});
		generatorsLabels[DieselPower] = statusLabel;
		dieselInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		statusLabel = createProgressBar(dieselInfoWidget, QSize{70, 30});
		generatorsLabels[DieselProgress] = statusLabel;
		dieselInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		break;
	}
	case TabWidget::Consumers: {
		widget = new QGroupBox{parent};
		widget->setTitle(tr("Consumers"));
		widget->setStyleSheet(groupBoxStylesheet);


		QVBoxLayout* layout = new QVBoxLayout{widget};
		widget->setLayout(layout);

		QFrame* firstInfoWidget = new QFrame{widget};
		QHBoxLayout* firstInfoLayout = new QHBoxLayout{firstInfoWidget};
		firstInfoWidget->setLayout(firstInfoLayout);
		layout->addWidget(firstInfoWidget);

		QWidget* statusLabel = nullptr;

		firstInfoLayout->addWidget(createLabel(firstInfoWidget, tr("1st group")), 10, Qt::AlignCenter);

		statusLabel = createLabel(firstInfoWidget, QString{"V: -"});
		consumersLabels[FirstVoltage] = statusLabel;
		firstInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		statusLabel = createLabel(firstInfoWidget, QString{"A: -"});
		consumersLabels[FirstCurrent] = statusLabel;
		firstInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		statusLabel = createLabel(firstInfoWidget, QString{"W: -"});
		consumersLabels[FirstPower] = statusLabel;
		firstInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		statusLabel = createProgressBar(firstInfoWidget, QSize{70, 30});
		consumersLabels[FirstProgress] = statusLabel;
		firstInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		QFrame* secondInfoWidget = new QFrame{widget};
		QHBoxLayout* secondInfoLayout = new QHBoxLayout{secondInfoWidget};
		secondInfoWidget->setLayout(secondInfoLayout);
		layout->addWidget(secondInfoWidget, Qt::AlignCenter);

		secondInfoLayout->addWidget(createLabel(secondInfoWidget, tr("2nd group")), 10, Qt::AlignCenter);

		statusLabel = createLabel(secondInfoWidget, QString{"V: -"});
		consumersLabels[SecondVoltage] = statusLabel;
		secondInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		statusLabel = createLabel(secondInfoWidget, QString{"A: -"});
		consumersLabels[SecondCurrent] = statusLabel;
		secondInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		statusLabel = createLabel(secondInfoWidget, QString{"W: -"});
		consumersLabels[SecondPower] = statusLabel;
		secondInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		statusLabel = createProgressBar(secondInfoWidget, QSize{70, 30});
		consumersLabels[SecondProgress] = statusLabel;
		secondInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		QFrame* thirdInfoWidget = new QFrame{widget};
		QHBoxLayout* thirdInfoLayout = new QHBoxLayout{thirdInfoWidget};
		thirdInfoWidget->setLayout(thirdInfoLayout);
		layout->addWidget(thirdInfoWidget);

		thirdInfoLayout->addWidget(createLabel(thirdInfoWidget, tr("3rd group")), 10, Qt::AlignCenter);

		statusLabel = createLabel(thirdInfoWidget, QString{"V: -"});
		consumersLabels[ThirdVoltage] = statusLabel;
		thirdInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		statusLabel = createLabel(thirdInfoWidget, QString{"A: -"});
		consumersLabels[ThirdCurrent] = statusLabel;
		thirdInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		statusLabel = createLabel(thirdInfoWidget, QString{"W: -"});
		consumersLabels[ThirdPower] = statusLabel;
		thirdInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		statusLabel = createProgressBar(thirdInfoWidget, QSize{70, 30});
		consumersLabels[ThirdProgress] = statusLabel;
		thirdInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		break;
	}
	case TabWidget::WorkMode: {
		widget = new QGroupBox{parent};
		widget->setTitle(tr("Work Mode"));
		widget->setStyleSheet(groupBoxStylesheet);

		QVBoxLayout* layout = new QVBoxLayout{widget};
		widget->setLayout(layout);

		QFrame* manualModeWidget = new QFrame{widget};
		QHBoxLayout* manualModeWidgetLayout = new QHBoxLayout{manualModeWidget};
		manualModeWidget->setLayout(manualModeWidgetLayout);
		manualModeWidget->setSizePolicy(QSizePolicy{QSizePolicy::Maximum, QSizePolicy::Maximum});
		layout->addWidget(manualModeWidget, 20, Qt::AlignLeft);

		QRadioButton* manualModeButton = new QRadioButton{manualModeWidget};
		manualModeButton->setMinimumSize(20, 20);
		manualModeWidgetLayout->addWidget(manualModeButton, 20, Qt::AlignLeft);

		QFrame* manualModeDescriptionWidget = new QFrame{manualModeWidget};
		QVBoxLayout* manualModeDescriptionLayout = new QVBoxLayout{manualModeDescriptionWidget};
		manualModeDescriptionWidget->setLayout(manualModeDescriptionLayout);
		manualModeDescriptionWidget->setSizePolicy(QSizePolicy{QSizePolicy::Maximum, QSizePolicy::Maximum});
		manualModeWidgetLayout->addWidget(manualModeDescriptionWidget, 20, Qt::AlignLeft);

		QLabel* manualModeWidgetName = new QLabel{manualModeDescriptionWidget};
		manualModeWidgetName->setText(tr("Manual mode"));
		manualModeDescriptionLayout->addWidget(manualModeWidgetName, Qt::AlignLeft);
		manualModeWidgetName->setSizePolicy(QSizePolicy{QSizePolicy::Maximum, QSizePolicy::Maximum});

		QFrame* manualModeDescriptionDelimeter = new QFrame{manualModeDescriptionWidget};
		manualModeDescriptionDelimeter->setFrameShape(QFrame::HLine);
		manualModeDescriptionLayout->addWidget(manualModeDescriptionDelimeter, Qt::AlignLeft);

		QLabel* manualModeWidgetDescription = new QLabel{manualModeDescriptionWidget};
		manualModeWidgetDescription->setText(tr("Manual mode description"));
		manualModeDescriptionLayout->addWidget(manualModeWidgetDescription, Qt::AlignLeft);
		manualModeWidgetDescription->setSizePolicy(QSizePolicy{QSizePolicy::Maximum, QSizePolicy::Maximum});

		QFrame* autoModeWidget = new QFrame{widget};
		QHBoxLayout* autoModeWidgetLayout = new QHBoxLayout{autoModeWidget};
		autoModeWidget->setLayout(autoModeWidgetLayout);
		layout->addWidget(autoModeWidget, 20, Qt::AlignLeft);

		QRadioButton* autoModeButton = new QRadioButton{autoModeWidget};
		autoModeButton->setMinimumSize(20, 20);
		autoModeWidgetLayout->addWidget(autoModeButton, 20);

		QFrame* autoModeDescriptionWidget = new QFrame{autoModeWidget};
		QVBoxLayout* autoModeDescriptionLayout = new QVBoxLayout{autoModeDescriptionWidget};
		autoModeDescriptionWidget->setLayout(autoModeDescriptionLayout);
		autoModeWidgetLayout->addWidget(autoModeDescriptionWidget, 20, Qt::AlignLeft);

		QLabel* autoModeWidgetName = new QLabel{autoModeDescriptionWidget};
		autoModeWidgetName->setText(tr("Automatic mode"));
		autoModeDescriptionLayout->addWidget(autoModeWidgetName, Qt::AlignLeft);

		QFrame* autoModeDescriptionDelimeter = new QFrame{autoModeDescriptionWidget};
		autoModeDescriptionDelimeter->setFrameShape(QFrame::HLine);
		autoModeDescriptionLayout->addWidget(autoModeDescriptionDelimeter, Qt::AlignLeft);

		QLabel* autoModeWidgetDescription = new QLabel{autoModeDescriptionWidget};
		autoModeWidgetDescription->setText(tr("Automatic mode description"));
		autoModeDescriptionLayout->addWidget(autoModeWidgetDescription, Qt::AlignLeft);

		// clicked signal prevents infinite recursion against toggled signal
		connect(manualModeButton, &QRadioButton::clicked, manualModeButton, [=](bool state) {
			if (state)
				autoModeButton->setChecked(!state);
			else
				manualModeButton->setChecked(!state);
		});

		connect(autoModeButton, &QRadioButton::clicked, autoModeButton, [=](bool state) {
			if (state)
				manualModeButton->setChecked(!state);
			else
				autoModeButton->setChecked(!state);
		});

		break;
	}
	case TabWidget::BatteryStatus: {
		widget = new QGroupBox{parent};
		widget->setTitle(tr("Battery"));
		widget->setStyleSheet(groupBoxStylesheet);

		QVBoxLayout* layout = new QVBoxLayout{widget};
		widget->setLayout(layout);

		QFrame* batteryStatusWidget = new QFrame{this};
		layout->addWidget(batteryStatusWidget);
		QHBoxLayout* batteryStatusLayout = new QHBoxLayout{batteryStatusWidget};
		batteryStatusWidget->setLayout(batteryStatusLayout);

		QProgressBar* batteryChargeIndicator = createProgressBar(batteryStatusWidget, QSize{60, 100});
		batteryChargeIndicator->setValue(0);
		batteryChargeIndicator->setOrientation(Qt::Vertical);
		batteryLabels[BatteryProgress] = batteryChargeIndicator;
		batteryStatusLayout->addWidget(batteryChargeIndicator);

		QFrame* batteryStatusInformation = new QFrame{widget};
		batteryStatusLayout->addWidget(batteryStatusInformation);
		QVBoxLayout* batteryStatusInformationLayout = new QVBoxLayout{batteryStatusInformation};
		batteryStatusInformation->setLayout(batteryStatusInformationLayout);

		QLabel* batteryCurrentCapacity = new QLabel{batteryStatusInformation};
		batteryCurrentCapacity->setText(QString{tr("%1 V")}.arg(0));
		batteryLabels[BatteryVoltage] = batteryCurrentCapacity;
		batteryStatusInformationLayout->addWidget(batteryCurrentCapacity);

		QFrame* batteryInformationDelimeter = new QFrame{batteryStatusInformation};
		batteryInformationDelimeter->setFrameShape(QFrame::HLine);
		batteryStatusInformationLayout->addWidget(batteryInformationDelimeter);

		QLabel* batteryCurrentStatus = new QLabel{batteryStatusInformation};
		batteryCurrentStatus->setText(QString{tr("Charging...")});
		batteryLabels[BatteryInfo] = batteryCurrentStatus;
		batteryStatusInformationLayout->addWidget(batteryCurrentStatus);

		break;
	}
	default:
		break;
	}

	return widget;
}
