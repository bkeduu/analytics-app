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

	static QFile stylesheet_file = QFile{":/stylesheets/progressbar.css"};
	stylesheet_file.open(QFile::ReadOnly, QFile::Text);

	static QString stylesheet = QString::fromStdString(stylesheet_file.readAll().toStdString());

	result->setValue(40);
	result->setTextVisible(true);
	result->setMaximumSize(minSize);
	result->setStyleSheet("QProgressBar::chunk { \
							  background-color: rgb(0, 179, 0); \
						  } \
						  QProgressBar { \
							  border: 1px solid #acacac; \
							  border-radius: 0px; \
							  text-align: center; \
							  background: #dfdfdf; \
						 }");

	return result;
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

		solarInfoLayout->addWidget(createLabel(solarInfoWidget, ":/images/solar-power.png",
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}, QSize{30, 30}), 10, Qt::AlignCenter);
		solarInfoLayout->addWidget(createLabel(solarInfoWidget, QString("V: %1").arg(0),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), 10, Qt::AlignCenter);
		solarInfoLayout->addWidget(createLabel(solarInfoWidget, QString("A: %1").arg(0),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), 10, Qt::AlignCenter);
		solarInfoLayout->addWidget(createLabel(solarInfoWidget, QString("W: %1").arg(0),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), 10, Qt::AlignCenter);
		solarInfoLayout->addWidget(createProgressBar(solarInfoWidget, QSize{70, 30}), 10, Qt::AlignCenter);

		QFrame* windInfoWidget = new QFrame{widget};
		QHBoxLayout* windInfoLayout = new QHBoxLayout{windInfoWidget};
		solarInfoWidget->setLayout(windInfoLayout);
		layout->addWidget(windInfoWidget, Qt::AlignCenter);

		windInfoLayout->addWidget(createLabel(windInfoWidget, ":/images/wind-turbine.png",
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}, QSize{30, 30}), 10, Qt::AlignCenter);
		windInfoLayout->addWidget(createLabel(windInfoWidget, QString("V: %1").arg(0),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), 10, Qt::AlignCenter);
		windInfoLayout->addWidget(createLabel(windInfoWidget, QString("A: %1").arg(0),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), 10, Qt::AlignCenter);
		windInfoLayout->addWidget(createLabel(windInfoWidget, QString("W: %1").arg(0),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), 10, Qt::AlignCenter);
		windInfoLayout->addWidget(createProgressBar(windInfoWidget, QSize{70, 30}), 10, Qt::AlignCenter);

		QFrame* dieselInfoWidget = new QFrame{widget};
		QHBoxLayout* dieselInfoLayout = new QHBoxLayout{dieselInfoWidget};
		dieselInfoWidget->setLayout(dieselInfoLayout);
		layout->addWidget(dieselInfoWidget, Qt::AlignCenter);

		dieselInfoLayout->addWidget(createLabel(dieselInfoWidget, ":/images/diesel.png",
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}, QSize{30, 30}), 10, Qt::AlignCenter);
		dieselInfoLayout->addWidget(createLabel(dieselInfoWidget, QString("V: %1").arg(0),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), 10, Qt::AlignCenter);
		dieselInfoLayout->addWidget(createLabel(dieselInfoWidget, QString("A: %1").arg(0),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), 10, Qt::AlignCenter);
		dieselInfoLayout->addWidget(createLabel(dieselInfoWidget, QString("W: %1").arg(0),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), 10, Qt::AlignCenter);
		dieselInfoLayout->addWidget(createProgressBar(dieselInfoWidget, QSize{70, 30}), 10, Qt::AlignCenter);

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

		firstInfoLayout->addWidget(createLabel(firstInfoWidget, tr("1st group"),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), 10, Qt::AlignCenter);
		firstInfoLayout->addWidget(createLabel(firstInfoWidget, QString("V: %1").arg(0),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), 10, Qt::AlignCenter);
		firstInfoLayout->addWidget(createLabel(firstInfoWidget, QString("A: %1").arg(0),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), 10, Qt::AlignCenter);
		firstInfoLayout->addWidget(createLabel(firstInfoWidget, QString("W: %1").arg(0),
									  QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum}), 10, Qt::AlignCenter);
		firstInfoLayout->addWidget(createProgressBar(firstInfoWidget, QSize{70, 30}), 10, Qt::AlignCenter);

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
		secondInfoLayout->addWidget(createProgressBar(secondInfoWidget, QSize{70, 30}), 10, Qt::AlignCenter);

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
		thirdInfoLayout->addWidget(createProgressBar(thirdInfoWidget, QSize{70, 30}), 10, Qt::AlignCenter);

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
			if (state) {
				autoModeButton->setChecked(!state);
			}
			else {
				manualModeButton->setChecked(!state);
			}
		});

		connect(autoModeButton, &QRadioButton::clicked, autoModeButton, [=](bool state) {
			if (state) {
				manualModeButton->setChecked(!state);
			}
			else {
				autoModeButton->setChecked(!state);
			}
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

		QProgressBar* batteryChargeIndicator = new QProgressBar{widget};

		batteryChargeIndicator->setValue(40);
		batteryChargeIndicator->setMinimumSize(60, 100);

		batteryChargeIndicator->setStyleSheet("QProgressBar::chunk { \
												  background-color: rgb(0, 179, 0); \
											  } \
											  QProgressBar { \
												  border: 1px solid #acacac; \
												  border-radius: 0px; \
												  text-align: center; \
												  background: #dfdfdf; \
											}");

		batteryChargeIndicator->setOrientation(Qt::Vertical);
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
