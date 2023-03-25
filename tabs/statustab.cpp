#include "statustab.h"

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

QLabel* StatusTab::createLabel(QWidget* parent, const QString& text, const QSizePolicy& sp) {
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

QLabel* StatusTab::createLabel(QWidget* parent, const QString& imagePath, const QSizePolicy& sp, const QSize& sz) {
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

QFrame* StatusTab::createWidget(TabWidget widgetType, QWidget* parent) {

	QFrame* widget;
	switch (widgetType) {
	case TabWidget::Generation: {
		widget = new QFrame{parent};

		QVBoxLayout* layout = new QVBoxLayout{widget};
		layout->setContentsMargins(0, 0, 0, 0);
		layout->setSizeConstraint(QLayout::SetDefaultConstraint);

		widget->setLayout(layout);
		widget->setFrameShape(QFrame::Box);

		QLabel* generationFrameLabel = new QLabel{widget};
		generationFrameLabel->setText(tr("Generators"));
		generationFrameLabel->setAlignment(Qt::AlignCenter);
		generationFrameLabel->setFrameShape(QFrame::Box);
		generationFrameLabel->setSizePolicy(QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});
		generationFrameLabel->setMaximumSize(QSize{100, 40});
		layout->addWidget(generationFrameLabel, 0, Qt::AlignCenter);

		QFrame* solarInfoWidget = new QFrame{widget};
		QHBoxLayout* solarInfoLayout = new QHBoxLayout{solarInfoWidget};
		solarInfoLayout->setContentsMargins(0, 0, 0, 0);
		solarInfoLayout->setSpacing(10);
		solarInfoWidget->setLayout(solarInfoLayout);
		layout->addWidget(solarInfoWidget, 0, Qt::AlignCenter);

		solarInfoLayout->addWidget(createLabel(solarInfoWidget, ":/images/solar-power.png",
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}, QSize{30, 30}), 10, Qt::AlignCenter);
		solarInfoLayout->addWidget(createLabel(solarInfoWidget, QString("V: %1").arg(0),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), 10, Qt::AlignCenter);
		solarInfoLayout->addWidget(createLabel(solarInfoWidget, QString("A: %1").arg(0),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), 10, Qt::AlignCenter);
		solarInfoLayout->addWidget(createLabel(solarInfoWidget, QString("W: %1").arg(0),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), 10, Qt::AlignCenter);
		solarInfoLayout->addWidget(createLabel(solarInfoWidget, ":/images/solar-power.png",
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}, QSize{30, 30}), 10, Qt::AlignCenter);

		QFrame* windInfoWidget = new QFrame{widget};
		QHBoxLayout* windInfoLayout = new QHBoxLayout{windInfoWidget};
		solarInfoWidget->setLayout(windInfoLayout);
		layout->addWidget(windInfoWidget, 0, Qt::AlignCenter);

		windInfoLayout->addWidget(createLabel(windInfoWidget, ":/images/wind-turbine.png",
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}, QSize{30, 30}), 10, Qt::AlignCenter);
		windInfoLayout->addWidget(createLabel(windInfoWidget, QString("V: %1").arg(0),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), 10, Qt::AlignCenter);
		windInfoLayout->addWidget(createLabel(windInfoWidget, QString("A: %1").arg(0),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), 10, Qt::AlignCenter);
		windInfoLayout->addWidget(createLabel(windInfoWidget, QString("W: %1").arg(0),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), 10, Qt::AlignCenter);
		windInfoLayout->addWidget(createLabel(windInfoWidget, ":/images/wind-turbine.png",
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}, QSize{30, 30}), 10, Qt::AlignCenter);

		QFrame* dieselInfoWidget = new QFrame{widget};
		QHBoxLayout* dieselInfoLayout = new QHBoxLayout{dieselInfoWidget};
		dieselInfoWidget->setLayout(dieselInfoLayout);
		layout->addWidget(dieselInfoWidget, 0, Qt::AlignCenter);

		dieselInfoLayout->addWidget(createLabel(dieselInfoWidget, ":/images/diesel.png",
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}, QSize{30, 30}), Qt::AlignCenter);
		dieselInfoLayout->addWidget(createLabel(dieselInfoWidget, QString("V: %1").arg(0),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), Qt::AlignCenter);
		dieselInfoLayout->addWidget(createLabel(dieselInfoWidget, QString("A: %1").arg(0),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), Qt::AlignCenter);
		dieselInfoLayout->addWidget(createLabel(dieselInfoWidget, QString("W: %1").arg(0),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), Qt::AlignCenter);
		dieselInfoLayout->addWidget(createLabel(dieselInfoWidget, ":/images/diesel.png",
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}, QSize{30, 30}), Qt::AlignCenter);

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
		layout->addWidget(consumersFrameLabel, 0, Qt::AlignCenter);

		QFrame* firstInfoWidget = new QFrame{widget};
		QHBoxLayout* firstInfoLayout = new QHBoxLayout{firstInfoWidget};
		firstInfoWidget->setLayout(firstInfoLayout);
		layout->addWidget(firstInfoWidget);

		firstInfoLayout->addWidget(createLabel(firstInfoWidget, tr("1st group"),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), 10, Qt::AlignCenter);
		firstInfoLayout->addWidget(createLabel(firstInfoWidget, QString("V: %1").arg(0),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), 10, Qt::AlignCenter);
		firstInfoLayout->addWidget(createLabel(firstInfoWidget, QString("A: %1").arg(0),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), 10, Qt::AlignCenter);
		firstInfoLayout->addWidget(createLabel(firstInfoWidget, QString("W: %1").arg(0),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), 10, Qt::AlignCenter);
		firstInfoLayout->addWidget(createLabel(firstInfoWidget, ":/images/solar-power.png",
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}, QSize{30, 30}), 10, Qt::AlignCenter);

		QFrame* secondInfoWidget = new QFrame{widget};
		QHBoxLayout* secondInfoLayout = new QHBoxLayout{secondInfoWidget};
		secondInfoWidget->setLayout(secondInfoLayout);
		layout->addWidget(secondInfoWidget, Qt::AlignCenter);

		secondInfoLayout->addWidget(createLabel(secondInfoWidget, tr("2nd group"),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), 10, Qt::AlignCenter);
		secondInfoLayout->addWidget(createLabel(secondInfoWidget, QString("V: %1").arg(0),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), 10, Qt::AlignCenter);
		secondInfoLayout->addWidget(createLabel(secondInfoWidget, QString("A: %1").arg(0),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), 10, Qt::AlignCenter);
		secondInfoLayout->addWidget(createLabel(secondInfoWidget, QString("W: %1").arg(0),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), 10, Qt::AlignCenter);
		secondInfoLayout->addWidget(createLabel(secondInfoWidget, ":/images/solar-power.png",
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}, QSize{30, 30}), 10, Qt::AlignCenter);

		QFrame* thirdInfoWidget = new QFrame{widget};
		QHBoxLayout* thirdInfoLayout = new QHBoxLayout{thirdInfoWidget};
		thirdInfoWidget->setLayout(thirdInfoLayout);
		layout->addWidget(thirdInfoWidget);

		thirdInfoLayout->addWidget(createLabel(thirdInfoWidget, tr("3rd group"),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), 10, Qt::AlignCenter);
		thirdInfoLayout->addWidget(createLabel(thirdInfoWidget, QString("V: %1").arg(0),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), 10, Qt::AlignCenter);
		thirdInfoLayout->addWidget(createLabel(thirdInfoWidget, QString("A: %1").arg(0),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), 10, Qt::AlignCenter);
		thirdInfoLayout->addWidget(createLabel(thirdInfoWidget, QString("W: %1").arg(0),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), 10, Qt::AlignCenter);
		thirdInfoLayout->addWidget(createLabel(thirdInfoWidget, ":/images/solar-power.png",
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}, QSize{30, 30}), 10, Qt::AlignCenter);

		break;
	}
	case TabWidget::WorkMode: {
		widget = new QFrame{parent};
		widget->setFrameShape(QFrame::Box);

		QVBoxLayout* layout = new QVBoxLayout{widget};
		widget->setLayout(layout);

		QLabel* modeLabel = new QLabel{widget};
		modeLabel->setText(tr("Work Mode"));
		modeLabel->setAlignment(Qt::AlignCenter);
		modeLabel->setFrameShape(QFrame::Box);
		modeLabel->setSizePolicy(QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});
		modeLabel->setMaximumSize(QSize{150, 40});
		layout->addWidget(modeLabel, 0, Qt::AlignCenter);

		QFrame* manualModeWidget = new QFrame{widget};
		QHBoxLayout* manualModeWidgetLayout = new QHBoxLayout{manualModeWidget};
		manualModeWidget->setLayout(manualModeWidgetLayout);
		layout->addWidget(manualModeWidget, 20, Qt::AlignCenter);

		QRadioButton* manualModeButton = new QRadioButton{manualModeWidget};
		manualModeButton->setMinimumSize(20, 20);
		manualModeWidgetLayout->addWidget(manualModeButton, 20, Qt::AlignCenter);

		QFrame* manualModeDescriptionWidget = new QFrame{manualModeWidget};
		QVBoxLayout* manualModeDescriptionLayout = new QVBoxLayout{manualModeDescriptionWidget};
		manualModeDescriptionWidget->setLayout(manualModeDescriptionLayout);
		manualModeWidgetLayout->addWidget(manualModeDescriptionWidget, 20, Qt::AlignCenter);

		QLabel* manualModeWidgetName = new QLabel{manualModeDescriptionWidget};
		manualModeWidgetName->setText(tr("Manual mode"));
		manualModeDescriptionLayout->addWidget(manualModeWidgetName);

		QFrame* manualModeDescriptionDelimeter = new QFrame{manualModeDescriptionWidget};
		manualModeDescriptionDelimeter->setFrameShape(QFrame::HLine);
		manualModeDescriptionLayout->addWidget(manualModeDescriptionDelimeter);

		QLabel* manualModeWidgetDescription = new QLabel{manualModeDescriptionWidget};
		manualModeWidgetDescription->setText(tr("Manual mode description"));
		manualModeDescriptionLayout->addWidget(manualModeWidgetDescription);

		QFrame* autoModeWidget = new QFrame{widget};
		QHBoxLayout* autoModeWidgetLayout = new QHBoxLayout{autoModeWidget};
		autoModeWidget->setLayout(autoModeWidgetLayout);
		layout->addWidget(autoModeWidget, 20, Qt::AlignCenter);

		QRadioButton* autoModeButton = new QRadioButton{autoModeWidget};
		autoModeButton->setMinimumSize(20, 20);
		autoModeWidgetLayout->addWidget(autoModeButton, 20, Qt::AlignCenter);

		QFrame* autoModeDescriptionWidget = new QFrame{autoModeWidget};
		QVBoxLayout* autoModeDescriptionLayout = new QVBoxLayout{autoModeDescriptionWidget};
		autoModeDescriptionWidget->setLayout(autoModeDescriptionLayout);
		autoModeWidgetLayout->addWidget(autoModeDescriptionWidget, 20, Qt::AlignCenter);

		QLabel* autoModeWidgetName = new QLabel{autoModeDescriptionWidget};
		autoModeWidgetName->setText(tr("Automatic mode"));
		autoModeDescriptionLayout->addWidget(autoModeWidgetName);

		QFrame* autoModeDescriptionDelimeter = new QFrame{autoModeDescriptionWidget};
		autoModeDescriptionDelimeter->setFrameShape(QFrame::HLine);
		autoModeDescriptionLayout->addWidget(autoModeDescriptionDelimeter);

		QLabel* autoModeWidgetDescription = new QLabel{autoModeDescriptionWidget};
		autoModeWidgetDescription->setText(tr("Automatic mode description"));
		autoModeDescriptionLayout->addWidget(autoModeWidgetDescription);

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

		QProgressBar* batteryChargeIndicator = new QProgressBar{widget};
		batteryChargeIndicator->setOrientation(Qt::Vertical);
		batteryChargeIndicator->setValue(40);
		batteryChargeIndicator->setMinimumSize(60, 100);
		batteryStatusLayout->addWidget(batteryChargeIndicator);

		QFrame* batteryStatusInformation = new QFrame{widget};
		batteryStatusLayout->addWidget(batteryStatusInformation);
		QVBoxLayout* batteryStatusInformationLayout = new QVBoxLayout{batteryStatusInformation};
		batteryStatusInformation->setLayout(batteryStatusInformationLayout);

		QLabel* batteryCurrentCapacity = new QLabel{batteryStatusInformation};
		batteryCurrentCapacity->setText(QString{tr("%1 W*h")}.arg(0));
		batteryStatusInformationLayout->addWidget(batteryCurrentCapacity);

		QFrame* batteryInformationDelimeter = new QFrame{batteryStatusInformation};
		batteryInformationDelimeter->setFrameShape(QFrame::HLine);
		batteryStatusInformationLayout->addWidget(batteryInformationDelimeter);

		QLabel* batteryCurrentStatus = new QLabel{batteryStatusInformation};
		batteryCurrentStatus->setText(QString{tr("Charging...")});
		batteryStatusInformationLayout->addWidget(batteryCurrentStatus);

		break;
	}
	default:
		widget = nullptr;
		break;
	}

	return widget;
}
