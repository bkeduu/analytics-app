#include "statustab.h"
#include "customwidgets.h"
#include "client.h"

#include <QJsonArray>
#include <QGroupBox>
#include <QButtonGroup>
#include <QResizeEvent>

StatusTab::StatusTab(const QString& tabName, QWidget* parent) : ITab{parent, tabName}, mLayout{nullptr}, mButtonGroup{nullptr} {
	createTabContents();
}

void StatusTab::lock() {
	if (mButtonGroup)
		for (int i = int(Manual); i <= int(MaximumEconomy); ++i)
			dynamic_cast<QRadioButton*>(mButtonGroup->button(i))->setEnabled(false);
}

void StatusTab::unlock() {
	if (mButtonGroup)
		for (int i = int(Manual); i <= int(MaximumEconomy); ++i)
			dynamic_cast<QRadioButton*>(mButtonGroup->button(i))->setEnabled(true);
}

void StatusTab::createTabContents() {
	clearTab();
	delete mLayout;
	QGridLayout* gridLayout = new QGridLayout{this};
	gridLayout->setAlignment(Qt::AlignCenter);
	gridLayout->setContentsMargins(5, 5, 5, 5);
	gridLayout->setSpacing(10);
	mLayout = gridLayout;
	this->setLayout(gridLayout);

	mGenerators = createWidget(TabWidget::Generation, this);
	mGenerators->setSizePolicy(QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});
	mConsumers = createWidget(TabWidget::Consumers, this);
	mConsumers->setSizePolicy(QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});
	mModes = createWidget(TabWidget::WorkMode, this);
	mModes->setSizePolicy(QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});
	mBattery = createWidget(TabWidget::BatteryStatus, this);
	mBattery->setSizePolicy(QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});

	gridLayout->addWidget(mGenerators, 0, 0);
	gridLayout->addWidget(mModes, 0, 1);
	gridLayout->addWidget(mConsumers, 1, 0);
	gridLayout->addWidget(mBattery, 1, 1);

	gridLayout->setColumnStretch(0, 2);
	gridLayout->setColumnStretch(1, 1);
}

void StatusTab::removeTabContents(const QString& text) {
	clearTab();
	delete mLayout;
	mLayout = new QGridLayout{this};
	this->setLayout(mLayout);
	mLayout->setAlignment(Qt::AlignCenter);

	QLabel* textLabel = new QLabel{this};
	textLabel->setProperty("class", "tab-standalone-text");
	textLabel->setText(text);
	textLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	textLabel->setAlignment(Qt::AlignCenter);

	mLayout->addWidget(textLabel);
}

QLabel* StatusTab::createLabel(QWidget* parent, const QString& text) const {
	QLabel* result = new QLabel{parent};
	result->setText(text);
	result->setSizePolicy(QSizePolicy{QSizePolicy::Minimum, QSizePolicy::Minimum});
	result->setMargin(0);
	result->setIndent(0);
	result->setContentsMargins(0, 0, 0, 0);
	result->setAlignment(Qt::AlignLeading);
	result->setMaximumSize(QSize{90, 40});
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
	result->setMaximumSize(QSize{90, 40});
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

void StatusTab::onSensorsDataReceived(const QJsonObject& dataObject) {
	if (!dataObject.contains("solar") || !dataObject.contains("wind") || !dataObject.contains("gen"))
		throw InternalErrorException{tr("Data structure with wrong value received at %1. The app will be closed.").arg(FLF)};

	QJsonArray solarArray = dataObject.value("solar").toArray();
	if (solarArray.isEmpty() || solarArray.size() < 4)
		throw InternalErrorException{tr("Data structure with wrong value received at %1. The app will be closed.").arg(FLF)};

	dynamic_cast<QLabel*>(mWidgetLocator["solar-voltage-label"])->setText(tr("V: %1").arg(solarArray[0].toDouble()));
	dynamic_cast<QLabel*>(mWidgetLocator["solar-current-label"])->setText(tr("A: %1").arg(solarArray[1].toDouble()));
	dynamic_cast<QLabel*>(mWidgetLocator["solar-power-label"])->setText(tr("W: %1").arg(solarArray[2].toDouble()));
	dynamic_cast<QProgressBar*>(mWidgetLocator["solar-progressbar"])->setValue(solarArray[3].toInt());

	QJsonArray windArray = dataObject.value("wind").toArray();
	if (windArray.isEmpty() || windArray.size() < 4)
		throw InternalErrorException{tr("Data structure with wrong value received at %1. The app will be closed.").arg(FLF)};

	dynamic_cast<QLabel*>(mWidgetLocator["wind-voltage-label"])->setText(tr("V: %1").arg(windArray[0].toDouble()));
	dynamic_cast<QLabel*>(mWidgetLocator["wind-current-label"])->setText(tr("A: %1").arg(windArray[1].toDouble()));
	dynamic_cast<QLabel*>(mWidgetLocator["wind-power-label"])->setText(tr("W: %1").arg(windArray[2].toDouble()));
	dynamic_cast<QProgressBar*>(mWidgetLocator["wind-progressbar"])->setValue(windArray[3].toInt());

	QJsonArray dieselArray = dataObject.value("gen").toArray();
	if (dieselArray.isEmpty() || dieselArray.size() < 5)
		throw InternalErrorException{tr("Data structure with wrong value received at %1. The app will be closed.").arg(FLF)};

	if (dieselArray[4].toInt()) {
		dynamic_cast<QLabel*>(mWidgetLocator["diesel-voltage-label"])->setText(tr("V: %1").arg(dieselArray[0].toDouble()));
		dynamic_cast<QLabel*>(mWidgetLocator["diesel-current-label"])->setText(tr("A: %1").arg(dieselArray[1].toDouble()));
		dynamic_cast<QLabel*>(mWidgetLocator["diesel-power-label"])->setText(tr("W: %1").arg(dieselArray[2].toDouble()));
		dynamic_cast<QProgressBar*>(mWidgetLocator["diesel-progressbar"])->setValue(dieselArray[3].toInt());
	}
	else {
		dynamic_cast<QLabel*>(mWidgetLocator["diesel-voltage-label"])->setText(tr("V: -"));
		dynamic_cast<QLabel*>(mWidgetLocator["diesel-current-label"])->setText(tr("A: -"));
		dynamic_cast<QLabel*>(mWidgetLocator["diesel-power-label"])->setText(tr("W: -"));
		dynamic_cast<QProgressBar*>(mWidgetLocator["diesel-progressbar"])->setValue(0);
	}

	QJsonArray batteryArray = dataObject.value("bat").toArray();
	dynamic_cast<QLabel*>(mWidgetLocator["battery-voltage-label"])->setText(QString{"%1 V"}.arg(batteryArray[0].toDouble()));
	int status = batteryArray[4].toInt();

	switch (BatteryInformation(status)) {
	case Charging:
		dynamic_cast<QLabel*>(mWidgetLocator["battery-status-label"])->setText(tr("Charging..."));
		break;
	case Discharging:
		dynamic_cast<QLabel*>(mWidgetLocator["battery-status-label"])->setText(tr("Discharging..."));
		break;
	default:
		dynamic_cast<QLabel*>(mWidgetLocator["battery-status-label"])->setText(tr("Unknown..."));
		break;
	}

	dynamic_cast<QProgressBar*>(mWidgetLocator["battery-progressbar"])->setValue(batteryArray[1].toInt());

	QJsonArray firstArray = dataObject.value("1").toArray();
	if (firstArray.isEmpty() || firstArray.size() < 5)
		throw InternalErrorException{tr("Data structure with wrong value received at %1. The app will be closed.").arg(FLF)};

	if (firstArray[4].toInt()) {
		dynamic_cast<QLabel*>(mWidgetLocator["first-voltage-label"])->setText(tr("V: %1").arg(firstArray[0].toDouble()));
		dynamic_cast<QLabel*>(mWidgetLocator["first-current-label"])->setText(tr("A: %1").arg(firstArray[1].toDouble()));
		dynamic_cast<QLabel*>(mWidgetLocator["first-power-label"])->setText(tr("W: %1").arg(firstArray[2].toDouble()));
		dynamic_cast<QProgressBar*>(mWidgetLocator["first-progressbar"])->setValue(firstArray[3].toInt());
	}
	else {
		dynamic_cast<QLabel*>(mWidgetLocator["first-voltage-label"])->setText(tr("V: -"));
		dynamic_cast<QLabel*>(mWidgetLocator["first-current-label"])->setText(tr("A: -"));
		dynamic_cast<QLabel*>(mWidgetLocator["first-power-label"])->setText(tr("W: -"));
		dynamic_cast<QProgressBar*>(mWidgetLocator["first-progressbar"])->setValue(0);
	}

	QJsonArray secondArray = dataObject.value("2").toArray();
	if (secondArray.isEmpty() || secondArray.size() < 5)
		throw InternalErrorException{tr("Data structure with wrong value received at %1. The app will be closed.").arg(FLF)};

	if (secondArray[4].toInt()) {
		dynamic_cast<QLabel*>(mWidgetLocator["second-voltage-label"])->setText(tr("V: %1").arg(secondArray[0].toDouble()));
		dynamic_cast<QLabel*>(mWidgetLocator["second-current-label"])->setText(tr("A: %1").arg(secondArray[1].toDouble()));
		dynamic_cast<QLabel*>(mWidgetLocator["second-power-label"])->setText(tr("W: %1").arg(secondArray[2].toDouble()));
		dynamic_cast<QProgressBar*>(mWidgetLocator["second-progressbar"])->setValue(secondArray[3].toInt());
	}
	else {
		dynamic_cast<QLabel*>(mWidgetLocator["second-voltage-label"])->setText(tr("V: -"));
		dynamic_cast<QLabel*>(mWidgetLocator["second-current-label"])->setText(tr("A: -"));
		dynamic_cast<QLabel*>(mWidgetLocator["second-power-label"])->setText(tr("W: -"));
		dynamic_cast<QProgressBar*>(mWidgetLocator["second-progressbar"])->setValue(0);
	}

	QJsonArray thirdArray = dataObject.value("3").toArray();
	if (thirdArray.isEmpty() || thirdArray.size() < 5)
		throw InternalErrorException{tr("Data structure with wrong value received at %1. The app will be closed.").arg(FLF)};

	if (thirdArray[4].toInt()) {
		dynamic_cast<QLabel*>(mWidgetLocator["third-voltage-label"])->setText(tr("V: %1").arg(thirdArray[0].toDouble()));
		dynamic_cast<QLabel*>(mWidgetLocator["third-current-label"])->setText(tr("A: %1").arg(thirdArray[1].toDouble()));
		dynamic_cast<QLabel*>(mWidgetLocator["third-power-label"])->setText(tr("W: %1").arg(thirdArray[2].toDouble()));
		dynamic_cast<QProgressBar*>(mWidgetLocator["third-progressbar"])->setValue(thirdArray[3].toInt());
	}
	else {
		dynamic_cast<QLabel*>(mWidgetLocator["third-voltage-label"])->setText(tr("V: -"));
		dynamic_cast<QLabel*>(mWidgetLocator["third-current-label"])->setText(tr("A: -"));
		dynamic_cast<QLabel*>(mWidgetLocator["third-power-label"])->setText(tr("W: -"));
		dynamic_cast<QProgressBar*>(mWidgetLocator["third-progressbar"])->setValue(0);
	}
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
		widget->setTitle(tr("Generators"));
		widget->setStyleSheet(groupBoxStylesheet);

		QVBoxLayout* layout = new QVBoxLayout{widget};
		layout->setContentsMargins(0, 15, 0, 15);
		widget->setLayout(layout);

		QFrame* solarInfoWidget = new QFrame{widget};
		QHBoxLayout* solarInfoLayout = new QHBoxLayout{solarInfoWidget};
		solarInfoWidget->setLayout(solarInfoLayout);
		layout->addWidget(solarInfoWidget, Qt::AlignCenter);

		solarInfoLayout->addWidget(createLabel(solarInfoWidget, ":/static/images/solar-power.png",
											   QSize{40, 40}), 10, Qt::AlignCenter);

		mWidgetLocator.insert("solar-voltage-label", createLabel(solarInfoWidget, QString{"V: -"}));
		solarInfoLayout->addWidget(mWidgetLocator["solar-voltage-label"], 10, Qt::AlignCenter);

		mWidgetLocator.insert("solar-current-label", createLabel(solarInfoWidget, QString{"A: -"}));
		solarInfoLayout->addWidget(mWidgetLocator["solar-current-label"], 10, Qt::AlignCenter);

		mWidgetLocator.insert("solar-power-label", createLabel(solarInfoWidget, QString{"W: -"}));
		solarInfoLayout->addWidget(mWidgetLocator["solar-power-label"], 10, Qt::AlignCenter);

		mWidgetLocator.insert("solar-progressbar", createProgressBar(solarInfoWidget, QSize{70, 40}));
		solarInfoLayout->addWidget(mWidgetLocator["solar-progressbar"], 10, Qt::AlignCenter);

		QFrame* windInfoWidget = new QFrame{widget};
		QHBoxLayout* windInfoLayout = new QHBoxLayout{windInfoWidget};
		windInfoWidget->setLayout(windInfoLayout);
		layout->addWidget(windInfoWidget, Qt::AlignCenter);

		windInfoLayout->addWidget(createLabel(windInfoWidget, ":/static/images/wind-turbine.png",
											  QSize{40, 40}), 10, Qt::AlignCenter);

		mWidgetLocator.insert("wind-voltage-label", createLabel(windInfoWidget, QString{"V: -"}));
		windInfoLayout->addWidget(mWidgetLocator["wind-voltage-label"], 10, Qt::AlignCenter);

		mWidgetLocator.insert("wind-current-label", createLabel(windInfoWidget, QString{"A: -"}));
		windInfoLayout->addWidget(mWidgetLocator["wind-current-label"], 10, Qt::AlignCenter);

		mWidgetLocator.insert("wind-power-label", createLabel(windInfoWidget, QString{"W: -"}));
		windInfoLayout->addWidget(mWidgetLocator["wind-power-label"], 10, Qt::AlignCenter);

		mWidgetLocator.insert("wind-progressbar", createProgressBar(windInfoWidget, QSize{70, 40}));
		windInfoLayout->addWidget(mWidgetLocator["wind-progressbar"], 10, Qt::AlignCenter);

		QFrame* dieselInfoWidget = new QFrame{widget};
		QHBoxLayout* dieselInfoLayout = new QHBoxLayout{dieselInfoWidget};
		dieselInfoWidget->setLayout(dieselInfoLayout);
		layout->addWidget(dieselInfoWidget, Qt::AlignCenter);

		dieselInfoLayout->addWidget(createLabel(dieselInfoWidget, ":/static/images/diesel.png",
												QSize{40, 40}), 10, Qt::AlignCenter);

		mWidgetLocator.insert("diesel-voltage-label", createLabel(dieselInfoWidget, QString{"V: -"}));
		dieselInfoLayout->addWidget(mWidgetLocator["diesel-voltage-label"], 10, Qt::AlignCenter);

		mWidgetLocator.insert("diesel-current-label", createLabel(dieselInfoWidget, QString{"A: -"}));
		dieselInfoLayout->addWidget(mWidgetLocator["diesel-current-label"], 10, Qt::AlignCenter);

		mWidgetLocator.insert("diesel-power-label", createLabel(dieselInfoWidget, QString{"W: -"}));
		dieselInfoLayout->addWidget(mWidgetLocator["diesel-power-label"], 10, Qt::AlignCenter);

		mWidgetLocator.insert("diesel-progressbar", createProgressBar(dieselInfoWidget, QSize{70, 40}));
		dieselInfoLayout->addWidget(mWidgetLocator["diesel-progressbar"], 10, Qt::AlignCenter);

		break;
	}
	case TabWidget::Consumers: {
		widget = new QGroupBox{parent};
		widget->setTitle(tr("Consumers"));
		widget->setStyleSheet(groupBoxStylesheet);

		QVBoxLayout* layout = new QVBoxLayout{widget};
		layout->setContentsMargins(0, 15, 0, 15);
		widget->setLayout(layout);

		QFrame* firstInfoWidget = new QFrame{widget};
		QHBoxLayout* firstInfoLayout = new QHBoxLayout{firstInfoWidget};
		firstInfoWidget->setLayout(firstInfoLayout);
		layout->addWidget(firstInfoWidget);

		firstInfoLayout->addWidget(createLabel(firstInfoWidget, tr("1st group")), 10, Qt::AlignCenter);

		mWidgetLocator.insert("first-voltage-label", createLabel(firstInfoWidget, QString{"V: -"}));
		firstInfoLayout->addWidget(mWidgetLocator["first-voltage-label"], 10, Qt::AlignCenter);

		mWidgetLocator.insert("first-current-label", createLabel(firstInfoWidget, QString{"A: -"}));
		firstInfoLayout->addWidget(mWidgetLocator["first-current-label"], 10, Qt::AlignCenter);

		mWidgetLocator.insert("first-power-label", createLabel(firstInfoWidget, QString{"W: -"}));
		firstInfoLayout->addWidget(mWidgetLocator["first-power-label"], 10, Qt::AlignCenter);

		mWidgetLocator.insert("first-progressbar", createProgressBar(firstInfoWidget, QSize{70, 40}));
		firstInfoLayout->addWidget(mWidgetLocator["first-progressbar"], 10, Qt::AlignCenter);

		QFrame* secondInfoWidget = new QFrame{widget};
		QHBoxLayout* secondInfoLayout = new QHBoxLayout{secondInfoWidget};
		secondInfoWidget->setLayout(secondInfoLayout);
		layout->addWidget(secondInfoWidget, Qt::AlignCenter);

		secondInfoLayout->addWidget(createLabel(secondInfoWidget, tr("2nd group")), 10, Qt::AlignCenter);

		mWidgetLocator.insert("second-voltage-label", createLabel(secondInfoWidget, QString{"V: -"}));
		secondInfoLayout->addWidget(mWidgetLocator["second-voltage-label"], 10, Qt::AlignCenter);

		mWidgetLocator.insert("second-current-label", createLabel(secondInfoWidget, QString{"A: -"}));
		secondInfoLayout->addWidget(mWidgetLocator["second-current-label"], 10, Qt::AlignCenter);

		mWidgetLocator.insert("second-power-label", createLabel(secondInfoWidget, QString{"W: -"}));
		secondInfoLayout->addWidget(mWidgetLocator["second-power-label"], 10, Qt::AlignCenter);

		mWidgetLocator.insert("second-progressbar", createProgressBar(secondInfoWidget, QSize{70, 40}));
		secondInfoLayout->addWidget(mWidgetLocator["second-progressbar"], 10, Qt::AlignCenter);

		QFrame* thirdInfoWidget = new QFrame{widget};
		QHBoxLayout* thirdInfoLayout = new QHBoxLayout{thirdInfoWidget};
		thirdInfoWidget->setLayout(thirdInfoLayout);
		layout->addWidget(thirdInfoWidget);

		thirdInfoLayout->addWidget(createLabel(thirdInfoWidget, tr("3rd group")), 10, Qt::AlignCenter);

		mWidgetLocator.insert("third-voltage-label", createLabel(thirdInfoWidget, QString{"V: -"}));
		thirdInfoLayout->addWidget(mWidgetLocator["third-voltage-label"], 10, Qt::AlignCenter);

		mWidgetLocator.insert("third-current-label", createLabel(thirdInfoWidget, QString{"A: -"}));
		thirdInfoLayout->addWidget(mWidgetLocator["third-current-label"], 10, Qt::AlignCenter);

		mWidgetLocator.insert("third-power-label", createLabel(thirdInfoWidget, QString{"W: -"}));
		thirdInfoLayout->addWidget(mWidgetLocator["third-power-label"], 10, Qt::AlignCenter);

		mWidgetLocator.insert("third-progressbar", createProgressBar(thirdInfoWidget, QSize{70, 40}));
		thirdInfoLayout->addWidget(mWidgetLocator["third-progressbar"], 10, Qt::AlignCenter);

		break;
	}
	case TabWidget::WorkMode: {
		widget = new QGroupBox{parent};
		widget->setTitle(tr("Work Mode"));
		widget->setStyleSheet(groupBoxStylesheet);

		QVBoxLayout* layout = new QVBoxLayout{widget};
		widget->setLayout(layout);

		CustomRadioButton* manualModeButton = new CustomRadioButton{widget, tr("Manual mode"), tr("Description of manual mode")};
		CustomRadioButton* maximumReliabilityModeButton = new CustomRadioButton{widget, tr("Maximum reliability mode"), tr("Description of maximum reliability mode")};
		CustomRadioButton* maximumPowerModeButton = new CustomRadioButton{widget, tr("Maximum power mode"), tr("Description of maximum power mode")};
		CustomRadioButton* maximumEcologyModeButton = new CustomRadioButton{widget, tr("Maximum ecology mode"), tr("Description of maximum ecology mode")};
		CustomRadioButton* maximumEconomyModeButton = new CustomRadioButton{widget, tr("Maximum economy mode"), tr("Description of maximum economy mode")};

		layout->addWidget(manualModeButton);
		layout->addWidget(maximumReliabilityModeButton);
		layout->addWidget(maximumPowerModeButton);
		layout->addWidget(maximumEcologyModeButton);
		layout->addWidget(maximumEconomyModeButton);

		manualModeButton->getButton()->click();

		mButtonGroup = new QButtonGroup{widget};
		mButtonGroup->setExclusive(true);

		mButtonGroup->addButton(manualModeButton->getButton(), int(Manual));
		mButtonGroup->addButton(maximumReliabilityModeButton->getButton(), int(MaximumReliability));
		mButtonGroup->addButton(maximumPowerModeButton->getButton(), int(MaximumPower));
		mButtonGroup->addButton(maximumEcologyModeButton->getButton(), int(MaximumEcology));
		mButtonGroup->addButton(maximumEconomyModeButton->getButton(), int(MaximumEconomy));

		connect(mButtonGroup, SIGNAL(idClicked(int)), this, SLOT(onButtonModeChange(int)));

		break;
	}
	case TabWidget::BatteryStatus: {
		widget = new QGroupBox{parent};
		widget->setTitle(tr("Battery"));
		widget->setStyleSheet(groupBoxStylesheet);

		QVBoxLayout* layout = new QVBoxLayout{widget};
		widget->setLayout(layout);

		QFrame* batteryStatusWidget = new QFrame{widget};
		layout->addWidget(batteryStatusWidget);
		QHBoxLayout* batteryStatusLayout = new QHBoxLayout{batteryStatusWidget};
		batteryStatusWidget->setLayout(batteryStatusLayout);

		QProgressBar* batteryChargeIndicator = createProgressBar(batteryStatusWidget, QSize{60, 100});
		batteryChargeIndicator->setValue(0);
		batteryChargeIndicator->setOrientation(Qt::Vertical);
		mWidgetLocator.insert("battery-progressbar", batteryChargeIndicator);
		batteryStatusLayout->addWidget(batteryChargeIndicator);

		QFrame* batteryStatusInformation = new QFrame{widget};
		batteryStatusLayout->addWidget(batteryStatusInformation);
		QVBoxLayout* batteryStatusInformationLayout = new QVBoxLayout{batteryStatusInformation};
		batteryStatusInformation->setLayout(batteryStatusInformationLayout);

		QLabel* batteryCurrentVoltage = new QLabel{batteryStatusInformation};
		batteryCurrentVoltage->setText(QString{tr("- V")});
		mWidgetLocator.insert("battery-voltage-label", batteryCurrentVoltage);
		batteryStatusInformationLayout->addWidget(batteryCurrentVoltage);

		QFrame* batteryInformationDelimeter = new QFrame{batteryStatusInformation};
		batteryInformationDelimeter->setFrameShape(QFrame::HLine);
		batteryStatusInformationLayout->addWidget(batteryInformationDelimeter);

		QLabel* batteryCurrentStatus = new QLabel{batteryStatusInformation};
		batteryCurrentStatus->setText(QString{tr("Charging...")});
		mWidgetLocator.insert("battery-status-label", batteryCurrentStatus);
		batteryStatusInformationLayout->addWidget(batteryCurrentStatus);

		break;
	}
	default:
		throw InternalErrorException{tr("Data structure with wrong value received at %1. The app will be closed.").arg(FLF)};
		break;
	}

	return widget;
}

void StatusTab::switchMode(int newMode) {
	QAbstractButton* button = mButtonGroup->button(newMode);

	button->blockSignals(true);
	button->click();
	button->blockSignals(false);
}

void StatusTab::onButtonModeChange(int mode) {
	emit modeChanged(mode);
}
