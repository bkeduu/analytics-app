#include "statustab.h"
#include "customwidgets.h"
#include "client.h"

#include <QJsonArray>
#include <QGroupBox>
#include <QButtonGroup>
#include <QResizeEvent>

StatusTab::StatusTab(const QString& tabName, QWidget* parent) : ITab{parent, tabName},
	generators{nullptr}, consumers{nullptr}, layout{nullptr} {
	createTabContents();
}

void StatusTab::onAuthorized() {

}

void StatusTab::createTabContents() {
	clearTab();
	delete this->layout;
	QGridLayout* gridLayout = new QGridLayout{this};
	gridLayout->setAlignment(Qt::AlignCenter);
	gridLayout->setContentsMargins(5, 5, 5, 5);
	gridLayout->setSpacing(10);
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
	layout->setAlignment(Qt::AlignCenter);

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

void StatusTab::onDataReceived(const QJsonObject& dataObject) {

	QJsonArray solarArray = dataObject.value("solar").toArray();
	dynamic_cast<QLabel*>(widgetLocator["solar-voltage-label"])->setText(tr("V: %1").arg(solarArray[0].toDouble()));
	dynamic_cast<QLabel*>(widgetLocator["solar-current-label"])->setText(tr("A: %1").arg(solarArray[1].toDouble()));
	dynamic_cast<QLabel*>(widgetLocator["solar-power-label"])->setText(tr("W: %1").arg(solarArray[2].toDouble()));
	dynamic_cast<QProgressBar*>(widgetLocator["solar-progressbar"])->setValue(solarArray[3].toInt());

	QJsonArray windArray = dataObject.value("wind").toArray();
	dynamic_cast<QLabel*>(widgetLocator["wind-voltage-label"])->setText(tr("V: %1").arg(windArray[0].toDouble()));
	dynamic_cast<QLabel*>(widgetLocator["wind-current-label"])->setText(tr("A: %1").arg(windArray[1].toDouble()));
	dynamic_cast<QLabel*>(widgetLocator["wind-power-label"])->setText(tr("W: %1").arg(windArray[2].toDouble()));
	dynamic_cast<QProgressBar*>(widgetLocator["wind-progressbar"])->setValue(windArray[3].toInt());

	QJsonArray dieselArray = dataObject.value("gen").toArray();
	dynamic_cast<QLabel*>(widgetLocator["diesel-voltage-label"])->setText(tr("V: %1").arg(dieselArray[0].toDouble()));
	dynamic_cast<QLabel*>(widgetLocator["diesel-current-label"])->setText(tr("A: %1").arg(dieselArray[1].toDouble()));
	dynamic_cast<QLabel*>(widgetLocator["diesel-power-label"])->setText(tr("W: %1").arg(dieselArray[2].toDouble()));
	dynamic_cast<QProgressBar*>(widgetLocator["diesel-progressbar"])->setValue(dieselArray[3].toInt());

	QJsonArray batteryArray = dataObject.value("bat").toArray();
	dynamic_cast<QLabel*>(widgetLocator["battery-voltage-label"])->setText(QString{"%1 V"}.arg(batteryArray[0].toDouble()));
	int status = batteryArray[4].toInt();

	switch (BatteryInformation(status)) {
	case Charging:
		dynamic_cast<QLabel*>(widgetLocator["battery-status-label"])->setText(tr("Charging..."));
		break;
	case Discharging:
		dynamic_cast<QLabel*>(widgetLocator["battery-status-label"])->setText(tr("Discharging..."));
		break;
	default:
		dynamic_cast<QLabel*>(widgetLocator["battery-status-label"])->setText(tr("Unknown..."));
		break;
	}

	dynamic_cast<QProgressBar*>(widgetLocator["battery-progressbar"])->setValue(batteryArray[1].toInt());

	QJsonArray firstArray = dataObject.value("1").toArray();
	dynamic_cast<QLabel*>(widgetLocator["first-voltage-label"])->setText(tr("V: %1").arg(firstArray[0].toDouble()));
	dynamic_cast<QLabel*>(widgetLocator["first-current-label"])->setText(tr("A: %1").arg(firstArray[1].toDouble()));
	dynamic_cast<QLabel*>(widgetLocator["first-power-label"])->setText(tr("W: %1").arg(firstArray[2].toDouble()));
	dynamic_cast<QProgressBar*>(widgetLocator["first-progressbar"])->setValue(firstArray[3].toInt());

	QJsonArray secondArray = dataObject.value("2").toArray();
	dynamic_cast<QLabel*>(widgetLocator["second-voltage-label"])->setText(tr("V: %1").arg(secondArray[0].toDouble()));
	dynamic_cast<QLabel*>(widgetLocator["second-current-label"])->setText(tr("A: %1").arg(secondArray[1].toDouble()));
	dynamic_cast<QLabel*>(widgetLocator["second-power-label"])->setText(tr("W: %1").arg(secondArray[2].toDouble()));
	dynamic_cast<QProgressBar*>(widgetLocator["second-progressbar"])->setValue(secondArray[3].toInt());

	QJsonArray thirdArray = dataObject.value("3").toArray();
	dynamic_cast<QLabel*>(widgetLocator["third-voltage-label"])->setText(tr("V: %1").arg(thirdArray[0].toDouble()));
	dynamic_cast<QLabel*>(widgetLocator["third-current-label"])->setText(tr("A: %1").arg(thirdArray[1].toDouble()));
	dynamic_cast<QLabel*>(widgetLocator["third-power-label"])->setText(tr("W: %1").arg(thirdArray[2].toDouble()));
	dynamic_cast<QProgressBar*>(widgetLocator["third-progressbar"])->setValue(thirdArray[3].toInt());
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

		widgetLocator.insert("solar-voltage-label", createLabel(solarInfoWidget, QString{"V: -"}));
		solarInfoLayout->addWidget(widgetLocator["solar-voltage-label"], 10, Qt::AlignCenter);

		widgetLocator.insert("solar-current-label", createLabel(solarInfoWidget, QString{"A: -"}));
		solarInfoLayout->addWidget(widgetLocator["solar-current-label"], 10, Qt::AlignCenter);

		widgetLocator.insert("solar-power-label", createLabel(solarInfoWidget, QString{"W: -"}));
		solarInfoLayout->addWidget(widgetLocator["solar-power-label"], 10, Qt::AlignCenter);

		widgetLocator.insert("solar-progressbar", createProgressBar(solarInfoWidget, QSize{70, 40}));
		solarInfoLayout->addWidget(widgetLocator["solar-progressbar"], 10, Qt::AlignCenter);

		QFrame* windInfoWidget = new QFrame{widget};
		QHBoxLayout* windInfoLayout = new QHBoxLayout{windInfoWidget};
		windInfoWidget->setLayout(windInfoLayout);
		layout->addWidget(windInfoWidget, Qt::AlignCenter);

		windInfoLayout->addWidget(createLabel(windInfoWidget, ":/static/images/wind-turbine.png",
											  QSize{40, 40}), 10, Qt::AlignCenter);

		widgetLocator.insert("wind-voltage-label", createLabel(windInfoWidget, QString{"V: -"}));
		windInfoLayout->addWidget(widgetLocator["wind-voltage-label"], 10, Qt::AlignCenter);

		widgetLocator.insert("wind-current-label", createLabel(windInfoWidget, QString{"A: -"}));
		windInfoLayout->addWidget(widgetLocator["wind-current-label"], 10, Qt::AlignCenter);

		widgetLocator.insert("wind-power-label", createLabel(windInfoWidget, QString{"W: -"}));
		windInfoLayout->addWidget(widgetLocator["wind-power-label"], 10, Qt::AlignCenter);

		widgetLocator.insert("wind-progressbar", createProgressBar(windInfoWidget, QSize{70, 40}));
		windInfoLayout->addWidget(widgetLocator["wind-progressbar"], 10, Qt::AlignCenter);

		QFrame* dieselInfoWidget = new QFrame{widget};
		QHBoxLayout* dieselInfoLayout = new QHBoxLayout{dieselInfoWidget};
		dieselInfoWidget->setLayout(dieselInfoLayout);
		layout->addWidget(dieselInfoWidget, Qt::AlignCenter);

		dieselInfoLayout->addWidget(createLabel(dieselInfoWidget, ":/static/images/diesel.png",
												QSize{40, 40}), 10, Qt::AlignCenter);

		widgetLocator.insert("diesel-voltage-label", createLabel(dieselInfoWidget, QString{"V: -"}));
		dieselInfoLayout->addWidget(widgetLocator["diesel-voltage-label"], 10, Qt::AlignCenter);

		widgetLocator.insert("diesel-current-label", createLabel(dieselInfoWidget, QString{"A: -"}));
		dieselInfoLayout->addWidget(widgetLocator["diesel-current-label"], 10, Qt::AlignCenter);

		widgetLocator.insert("diesel-power-label", createLabel(dieselInfoWidget, QString{"W: -"}));
		dieselInfoLayout->addWidget(widgetLocator["diesel-power-label"], 10, Qt::AlignCenter);

		widgetLocator.insert("diesel-progressbar", createProgressBar(dieselInfoWidget, QSize{70, 40}));
		dieselInfoLayout->addWidget(widgetLocator["diesel-progressbar"], 10, Qt::AlignCenter);

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

		widgetLocator.insert("first-voltage-label", createLabel(firstInfoWidget, QString{"V: -"}));
		firstInfoLayout->addWidget(widgetLocator["first-voltage-label"], 10, Qt::AlignCenter);

		widgetLocator.insert("first-current-label", createLabel(firstInfoWidget, QString{"A: -"}));
		firstInfoLayout->addWidget(widgetLocator["first-current-label"], 10, Qt::AlignCenter);

		widgetLocator.insert("first-power-label", createLabel(firstInfoWidget, QString{"W: -"}));
		firstInfoLayout->addWidget(widgetLocator["first-power-label"], 10, Qt::AlignCenter);

		widgetLocator.insert("first-progressbar", createProgressBar(firstInfoWidget, QSize{70, 40}));
		firstInfoLayout->addWidget(widgetLocator["first-progressbar"], 10, Qt::AlignCenter);

		QFrame* secondInfoWidget = new QFrame{widget};
		QHBoxLayout* secondInfoLayout = new QHBoxLayout{secondInfoWidget};
		secondInfoWidget->setLayout(secondInfoLayout);
		layout->addWidget(secondInfoWidget, Qt::AlignCenter);

		secondInfoLayout->addWidget(createLabel(secondInfoWidget, tr("2nd group")), 10, Qt::AlignCenter);

		widgetLocator.insert("second-voltage-label", createLabel(secondInfoWidget, QString{"V: -"}));
		secondInfoLayout->addWidget(widgetLocator["second-voltage-label"], 10, Qt::AlignCenter);

		widgetLocator.insert("second-current-label", createLabel(secondInfoWidget, QString{"A: -"}));
		secondInfoLayout->addWidget(widgetLocator["second-current-label"], 10, Qt::AlignCenter);

		widgetLocator.insert("second-power-label", createLabel(secondInfoWidget, QString{"W: -"}));
		secondInfoLayout->addWidget(widgetLocator["second-power-label"], 10, Qt::AlignCenter);

		widgetLocator.insert("second-progressbar", createProgressBar(secondInfoWidget, QSize{70, 40}));
		secondInfoLayout->addWidget(widgetLocator["second-progressbar"], 10, Qt::AlignCenter);

		QFrame* thirdInfoWidget = new QFrame{widget};
		QHBoxLayout* thirdInfoLayout = new QHBoxLayout{thirdInfoWidget};
		thirdInfoWidget->setLayout(thirdInfoLayout);
		layout->addWidget(thirdInfoWidget);

		thirdInfoLayout->addWidget(createLabel(thirdInfoWidget, tr("3rd group")), 10, Qt::AlignCenter);

		widgetLocator.insert("third-voltage-label", createLabel(thirdInfoWidget, QString{"V: -"}));
		thirdInfoLayout->addWidget(widgetLocator["third-voltage-label"], 10, Qt::AlignCenter);

		widgetLocator.insert("third-current-label", createLabel(thirdInfoWidget, QString{"A: -"}));
		thirdInfoLayout->addWidget(widgetLocator["third-current-label"], 10, Qt::AlignCenter);

		widgetLocator.insert("third-power-label", createLabel(thirdInfoWidget, QString{"W: -"}));
		thirdInfoLayout->addWidget(widgetLocator["third-power-label"], 10, Qt::AlignCenter);

		widgetLocator.insert("third-progressbar", createProgressBar(thirdInfoWidget, QSize{70, 40}));
		thirdInfoLayout->addWidget(widgetLocator["third-progressbar"], 10, Qt::AlignCenter);

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

		QButtonGroup* buttonGroup = new QButtonGroup{widget};
		buttonGroup->setExclusive(true);

		buttonGroup->addButton(manualModeButton->getButton(), int(Manual));
		buttonGroup->addButton(maximumReliabilityModeButton->getButton(), int(MaximumReliability));
		buttonGroup->addButton(maximumPowerModeButton->getButton(), int(MaximumPower));
		buttonGroup->addButton(maximumEcologyModeButton->getButton(), int(MaximumEcology));
		buttonGroup->addButton(maximumEconomyModeButton->getButton(), int(MaximumEconomy));

		connect(buttonGroup, SIGNAL(idClicked(int)), this, SLOT(onButtonModeChange(int)));

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
		widgetLocator.insert("battery-progressbar", batteryChargeIndicator);
		batteryStatusLayout->addWidget(batteryChargeIndicator);

		QFrame* batteryStatusInformation = new QFrame{widget};
		batteryStatusLayout->addWidget(batteryStatusInformation);
		QVBoxLayout* batteryStatusInformationLayout = new QVBoxLayout{batteryStatusInformation};
		batteryStatusInformation->setLayout(batteryStatusInformationLayout);

		QLabel* batteryCurrentVoltage = new QLabel{batteryStatusInformation};
		batteryCurrentVoltage->setText(QString{tr("- V")});
		widgetLocator.insert("battery-voltage-label", batteryCurrentVoltage);
		batteryStatusInformationLayout->addWidget(batteryCurrentVoltage);

		QFrame* batteryInformationDelimeter = new QFrame{batteryStatusInformation};
		batteryInformationDelimeter->setFrameShape(QFrame::HLine);
		batteryStatusInformationLayout->addWidget(batteryInformationDelimeter);

		QLabel* batteryCurrentStatus = new QLabel{batteryStatusInformation};
		batteryCurrentStatus->setText(QString{tr("Charging...")});
		widgetLocator.insert("battery-status-label", batteryCurrentStatus);
		batteryStatusInformationLayout->addWidget(batteryCurrentStatus);

		break;
	}
	default:
		throw InternalErrorException{QString{"Bad internal argument received. The app will be closed."} + FLF};
		break;
	}

	return widget;
}

void StatusTab::onButtonModeChange(int mode) {
	emit modeChanged(mode);
}
