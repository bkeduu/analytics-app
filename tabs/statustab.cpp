#include "statustab.h"

#include <QJsonObject>
#include <QJsonArray>

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
	layout = new QGridLayout{this};
	layout->setContentsMargins(9, 9, 9, 9);

	generators = createWidget(TabWidget::Generation, this);
	generators->setSizePolicy(QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});
	consumers = createWidget(TabWidget::Consumers, this);
	consumers->setSizePolicy(QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});
	modes = createWidget(TabWidget::WorkMode, this);
	modes->setSizePolicy(QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});
	battery = createWidget(TabWidget::BatteryStatus, this);
	battery->setSizePolicy(QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});

	layout->addWidget(generators, 0, 0);
	layout->addWidget(modes, 0, 1);
	layout->addWidget(consumers, 1, 0);
	layout->addWidget(battery, 1 , 1);
}

QLabel* StatusTab::createLabel(QWidget* parent, const QString& text, const QSizePolicy& sp) const {
	QLabel* result = new QLabel{parent};
	result->setText(text);
	result->setSizePolicy(sp);
	result->setMargin(0);
	result->setIndent(0);
	result->setContentsMargins(0, 0, 0, 0);
	result->setAlignment(Qt::AlignCenter);
	result->setMaximumSize(QSize{90, 30});
	return result;
}

QLabel* StatusTab::createLabel(QWidget* parent, const QString& imagePath, const QSizePolicy& sp, const QSize& sz) const {
	QLabel* result = new QLabel{parent};
	QPixmap image{imagePath};
	image = image.scaled(sz);
	result->setPixmap(image);
	result->setSizePolicy(sp);
	result->setMargin(0);
	result->setIndent(0);
	result->setContentsMargins(0, 0, 0, 0);
	result->setAlignment(Qt::AlignCenter);
	result->setMaximumSize(QSize{90, 30});
	return result;
}

QProgressBar* StatusTab::createProgressBar(QWidget* parent, const QSize& minSize) const {
	QProgressBar* result = new QProgressBar{parent};

	static QFile stylesheet_file = QFile{":/static/stylesheets/progressbar.css"};
	if (!stylesheet_file.isOpen())
		stylesheet_file.open(QFile::ReadOnly | QFile::Text);
	static QString stylesheet = stylesheet_file.readAll();

	result->setValue(40);
	result->setTextVisible(true);
	result->setMinimumSize(minSize);
	result->setStyleSheet(stylesheet);

	return result;
}

void StatusTab::onDataReceived(const QJsonArray& values) {

	QJsonArray generatorsArray = values[0].toArray();
	QJsonArray solarArray = generatorsArray[0].toArray();
	dynamic_cast<QLabel*>(generatorsLabels[SolarVoltage])->setText(solarArray[0].toString());
	dynamic_cast<QLabel*>(generatorsLabels[SolarCurrent])->setText(solarArray[1].toString());
	dynamic_cast<QLabel*>(generatorsLabels[SolarPower])->setText(solarArray[2].toString());
	dynamic_cast<QProgressBar*>(generatorsLabels[SolarProgress])->setValue(solarArray[4].toInt());

	QJsonArray windArray = generatorsArray[1].toArray();
	dynamic_cast<QLabel*>(generatorsLabels[WindVoltage])->setText(windArray[0].toString());
	dynamic_cast<QLabel*>(generatorsLabels[WindCurrent])->setText(windArray[1].toString());
	dynamic_cast<QLabel*>(generatorsLabels[WindPower])->setText(windArray[2].toString());
	dynamic_cast<QProgressBar*>(generatorsLabels[WindProgress])->setValue(windArray[4].toInt());

	QJsonArray dieselArray = generatorsArray[2].toArray();
	dynamic_cast<QLabel*>(generatorsLabels[DieselVoltage])->setText(dieselArray[0].toString());
	dynamic_cast<QLabel*>(generatorsLabels[DieselCurrent])->setText(dieselArray[1].toString());
	dynamic_cast<QLabel*>(generatorsLabels[DieselPower])->setText(dieselArray[2].toString());
	dynamic_cast<QProgressBar*>(generatorsLabels[DieselProgress])->setValue(dieselArray[4].toInt());

	QJsonArray batteryArray = values[1].toArray();
	dynamic_cast<QLabel*>(batteryLabels[BatteryVoltage])->setText(batteryArray[0].toString());
	int status = batteryArray[3].toInt();

	switch (status) {
	case 0:
		dynamic_cast<QLabel*>(batteryLabels[BatteryInfo])->setText(tr("Charging..."));
		break;
	case 1:
		dynamic_cast<QLabel*>(batteryLabels[BatteryInfo])->setText(tr("Discharging..."));
		break;
	case 2:
		dynamic_cast<QLabel*>(batteryLabels[BatteryInfo])->setText(tr("Unknown state"));
		break;
	}
	dynamic_cast<QProgressBar*>(batteryLabels[BatteryProgress])->setValue(batteryArray[4].toInt());

	QJsonArray consumersArray = values[2].toArray();
	QJsonArray firstArray = consumersArray[0].toArray();
	dynamic_cast<QLabel*>(consumersLabels[FirstVoltage])->setText(firstArray[0].toString());
	dynamic_cast<QLabel*>(consumersLabels[FirstCurrent])->setText(firstArray[1].toString());
	dynamic_cast<QLabel*>(consumersLabels[FirstPower])->setText(firstArray[2].toString());
	dynamic_cast<QProgressBar*>(consumersLabels[FirstProgress])->setValue(firstArray[4].toInt());

	QJsonArray secondArray = consumersArray[1].toArray();
	dynamic_cast<QLabel*>(consumersLabels[SecondVoltage])->setText(secondArray[0].toString());
	dynamic_cast<QLabel*>(consumersLabels[SecondCurrent])->setText(secondArray[1].toString());
	dynamic_cast<QLabel*>(consumersLabels[SecondPower])->setText(secondArray[2].toString());
	dynamic_cast<QProgressBar*>(consumersLabels[SecondProgress])->setValue(secondArray[4].toInt());

	QJsonArray thirdArray = consumersArray[2].toArray();
	dynamic_cast<QLabel*>(consumersLabels[ThirdVoltage])->setText(thirdArray[0].toString());
	dynamic_cast<QLabel*>(consumersLabels[ThirdCurrent])->setText(thirdArray[1].toString());
	dynamic_cast<QLabel*>(consumersLabels[ThirdPower])->setText(thirdArray[2].toString());
	dynamic_cast<QProgressBar*>(consumersLabels[ThirdProgress])->setValue(thirdArray[4].toInt());
}

QFrame* StatusTab::createWidget(TabWidget widgetType, QWidget* parent) {

	QFrame* widget;
	switch (widgetType) {
	case TabWidget::Generation: {
		widget = new QFrame{parent};

		QVBoxLayout* layout = new QVBoxLayout{widget};

		widget->setLayout(layout);
		widget->setFrameShape(QFrame::Box);

		QLabel* generationFrameLabel = new QLabel{widget};
		generationFrameLabel->setText(tr("Generators"));
		generationFrameLabel->setAlignment(Qt::AlignCenter);
		generationFrameLabel->setFrameShape(QFrame::Box);
		generationFrameLabel->setSizePolicy(QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});
		generationFrameLabel->setMaximumSize(QSize{100, 40});
		layout->addWidget(generationFrameLabel, Qt::AlignCenter);

		QFrame* solarInfoWidget = new QFrame{widget};
		QHBoxLayout* solarInfoLayout = new QHBoxLayout{solarInfoWidget};
		solarInfoWidget->setLayout(solarInfoLayout);
		layout->addWidget(solarInfoWidget, Qt::AlignCenter);

		QWidget* statusLabel = nullptr;

		solarInfoLayout->addWidget(createLabel(solarInfoWidget, ":/static/images/solar-power.png",
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}, QSize{30, 30}), 10, Qt::AlignCenter);

		statusLabel = createLabel(solarInfoWidget, QString("V: %1").arg(0),
								QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});
		generatorsLabels[SolarVoltage] = statusLabel;
		solarInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		statusLabel = createLabel(solarInfoWidget, QString("A: %1").arg(0),
								QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});
		generatorsLabels[SolarCurrent] = statusLabel;
		solarInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		statusLabel = createLabel(solarInfoWidget, QString("W: %1").arg(0),
								QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});
		generatorsLabels[SolarPower] = statusLabel;
		solarInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		statusLabel = createProgressBar(solarInfoWidget, QSize{70, 30});
		generatorsLabels[SolarProgress] = statusLabel;
		solarInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		QFrame* windInfoWidget = new QFrame{widget};
		QHBoxLayout* windInfoLayout = new QHBoxLayout{windInfoWidget};
		solarInfoWidget->setLayout(windInfoLayout);
		layout->addWidget(windInfoWidget, Qt::AlignCenter);

		windInfoLayout->addWidget(createLabel(windInfoWidget, ":/static/images/wind-turbine.png",
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}, QSize{30, 30}), 10, Qt::AlignCenter);

		statusLabel = createLabel(windInfoWidget, QString("V: %1").arg(0),
								  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});
		generatorsLabels[WindVoltage] = statusLabel;
		windInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		statusLabel = createLabel(windInfoWidget, QString("A: %1").arg(0),
								  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});
		generatorsLabels[WindCurrent] = statusLabel;
		windInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		statusLabel = createLabel(windInfoWidget, QString("W: %1").arg(0),
								  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});
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
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}, QSize{30, 30}), 10, Qt::AlignCenter);

		statusLabel = createLabel(dieselInfoWidget, QString("V: %1").arg(0),
								  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});
		generatorsLabels[DieselVoltage] = statusLabel;
		dieselInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		statusLabel = createLabel(dieselInfoWidget, QString("A: %1").arg(0),
								  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});
		generatorsLabels[DieselCurrent] = statusLabel;
		dieselInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		statusLabel = createLabel(dieselInfoWidget, QString("W: %1").arg(0),
								  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});
		generatorsLabels[DieselPower] = statusLabel;
		dieselInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		statusLabel = createProgressBar(dieselInfoWidget, QSize{70, 30});
		generatorsLabels[DieselProgress] = statusLabel;
		dieselInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		break;
	}
	case TabWidget::Consumers: {
		widget = new QFrame{parent};
		widget->setFrameShape(QFrame::Box);

		QVBoxLayout* layout = new QVBoxLayout{widget};
		widget->setLayout(layout);

		QLabel* consumersFrameLabel = new QLabel{widget};
		consumersFrameLabel->setText(tr("Consumers"));
		consumersFrameLabel->setAlignment(Qt::AlignCenter);
		consumersFrameLabel->setFrameShape(QFrame::Box);
		consumersFrameLabel->setSizePolicy(QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});
		consumersFrameLabel->setMaximumSize(QSize{120, 40});
		layout->addWidget(consumersFrameLabel, Qt::AlignCenter);

		QFrame* firstInfoWidget = new QFrame{widget};
		QHBoxLayout* firstInfoLayout = new QHBoxLayout{firstInfoWidget};
		firstInfoWidget->setLayout(firstInfoLayout);
		layout->addWidget(firstInfoWidget);

		QWidget* statusLabel = nullptr;

		firstInfoLayout->addWidget(createLabel(firstInfoWidget, tr("1st group"),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), 10, Qt::AlignCenter);

		statusLabel = createLabel(firstInfoWidget, QString("V: %1").arg(0),
								  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});
		consumersLabels[FirstVoltage] = statusLabel;
		firstInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		statusLabel = createLabel(firstInfoWidget, QString("A: %1").arg(0),
								  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});
		consumersLabels[FirstCurrent] = statusLabel;
		firstInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		statusLabel = createLabel(firstInfoWidget, QString("W: %1").arg(0),
								  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});
		consumersLabels[FirstPower] = statusLabel;
		firstInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		statusLabel = createProgressBar(firstInfoWidget, QSize{70, 30});
		consumersLabels[FirstProgress] = statusLabel;
		firstInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		QFrame* secondInfoWidget = new QFrame{widget};
		QHBoxLayout* secondInfoLayout = new QHBoxLayout{secondInfoWidget};
		secondInfoWidget->setLayout(secondInfoLayout);
		layout->addWidget(secondInfoWidget, Qt::AlignCenter);

		secondInfoLayout->addWidget(createLabel(secondInfoWidget, tr("2nd group"),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), 10, Qt::AlignCenter);

		statusLabel = createLabel(secondInfoWidget, QString("V: %1").arg(0),
								  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});
		consumersLabels[SecondVoltage] = statusLabel;
		secondInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		statusLabel = createLabel(secondInfoWidget, QString("A: %1").arg(0),
								  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});
		consumersLabels[SecondCurrent] = statusLabel;
		secondInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		statusLabel = createLabel(secondInfoWidget, QString("W: %1").arg(0),
								  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});
		consumersLabels[SecondPower] = statusLabel;
		secondInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		statusLabel = createProgressBar(secondInfoWidget, QSize{70, 30});
		consumersLabels[SecondProgress] = statusLabel;
		secondInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		QFrame* thirdInfoWidget = new QFrame{widget};
		QHBoxLayout* thirdInfoLayout = new QHBoxLayout{thirdInfoWidget};
		thirdInfoWidget->setLayout(thirdInfoLayout);
		layout->addWidget(thirdInfoWidget);

		thirdInfoLayout->addWidget(createLabel(thirdInfoWidget, tr("2nd group"),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), 10, Qt::AlignCenter);

		statusLabel = createLabel(thirdInfoWidget, QString("V: %1").arg(0),
								  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});
		consumersLabels[ThirdVoltage] = statusLabel;
		thirdInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		statusLabel = createLabel(thirdInfoWidget, QString("A: %1").arg(0),
								  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});
		consumersLabels[ThirdCurrent] = statusLabel;
		thirdInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		statusLabel = createLabel(thirdInfoWidget, QString("W: %1").arg(0),
								  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});
		consumersLabels[ThirdPower] = statusLabel;
		thirdInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		statusLabel = createProgressBar(thirdInfoWidget, QSize{70, 30});
		consumersLabels[ThirdProgress] = statusLabel;
		thirdInfoLayout->addWidget(statusLabel, 10, Qt::AlignCenter);

		break;
	}
	case TabWidget::WorkMode: {
		widget = new QFrame{parent};
		widget->setFrameShape(QFrame::Box);

		QVBoxLayout* layout = new QVBoxLayout{widget};
		widget->setLayout(layout);

		QLabel* modeLabel = new QLabel{widget};
		modeLabel->setText(tr("Work Mode"));
		modeLabel->setFrameShape(QFrame::Box);
		modeLabel->setSizePolicy(QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});
		modeLabel->setMaximumSize(QSize{150, 40});
		layout->addWidget(modeLabel);

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
		widget = new QFrame{parent};
		widget->setFrameShape(QFrame::Box);

		QVBoxLayout* layout = new QVBoxLayout{widget};
		widget->setLayout(layout);

		QFrame* batteryStatusWidget = new QFrame{this};
		layout->addWidget(batteryStatusWidget);
		QHBoxLayout* batteryStatusLayout = new QHBoxLayout{batteryStatusWidget};

		QProgressBar* batteryChargeIndicator = createProgressBar(batteryStatusWidget, QSize{60, 100});
		batteryChargeIndicator->setValue(40);
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
		widget = nullptr;
		break;
	}

	return widget;
}
