#include "generationtab.h"
#include "mainwindow.h"

GenerationTab::GenerationTab(const QString& tabName, QWidget* parent)
	: ITab{parent, tabName}, mLayout{nullptr}, mIsRelaySwitched{false} {
	createTabContents();
}

void GenerationTab::lock() {
	mCheckBox->lock();
}

void GenerationTab::unlock() {
	mCheckBox->unlock();
}

void GenerationTab::onSensorsDataReceived(const QJsonObject& data) {
	if (!data.contains("gen"))
		throw InternalErrorException{tr("Data structure with wrong value received at %1. The app will be closed.").arg(FLF)};

	QJsonArray generatorArray = data.value("gen").toArray();
	if (generatorArray.isEmpty() || generatorArray.size() < 5)
		throw InternalErrorException{tr("Data structure with wrong value received at %1. The app will be closed.").arg(FLF)};

	if (mIsRelaySwitched)
		mIsRelaySwitched = false;
	else
		mCheckBox->setCheckboxStatus(generatorArray[4].toInt());
}

void GenerationTab::onGraphicsDataReceived(const QJsonObject& data) {
	if (!mPlot)
		createTabContents();

	if (!data.contains("prediction") || !data.contains("reality"))
		throw InternalErrorException{tr("Data structure with wrong value received at %1. The app will be closed.").arg(FLF)};

	QJsonArray realityData = data.value("reality").toArray();

	static const QStringList plotLabels{tr("Solar"), tr("Wind"), tr("Diesel")};
	QStringList labels;

	QVector<QVector<double>> generatorsValues{3, {}};

	for (auto it = realityData.begin(); it != realityData.end(); ++it) {
		QJsonArray value = (*it).toArray();

		labels << value.at(0).toString("");

		generatorsValues[0] << value.at(1).toDouble(.0);
		generatorsValues[1] << value.at(2).toDouble(.0);
		generatorsValues[2] << value.at(3).toDouble(.0);
	}

	mPlot->onGraphicsData(labels, plotLabels, generatorsValues, {});
	mPlot->resize(size());
}

void GenerationTab::createTabContents() {
	clearTab();
	delete mLayout;
	QVBoxLayout* layout = new QVBoxLayout{this};
	mLayout = layout;
	layout->setSpacing(30);

	mPlot = new StackedBarWithLinesWidget{this};
	layout->addWidget(mPlot, 15);

	QWidget* relayWidget = new QWidget{this};
	QHBoxLayout* relayWidgetLayout = new QHBoxLayout{relayWidget};
	relayWidget->setLayout(relayWidgetLayout);
	relayWidgetLayout->setAlignment(Qt::AlignCenter);
	layout->addWidget(relayWidget, 1);

	mCheckBox = new CustomCheckBox{this, tr("Diesel generator")};
	relayWidgetLayout->addWidget(mCheckBox);

	connect(mCheckBox, &CustomCheckBox::checkboxClicked, mCheckBox, [=](bool newState) {
		mIsRelaySwitched = true;
		mParent->onRelayClicked(4, newState);
	});

	setLayout(mLayout);
}

void GenerationTab::removeTabContents(const QString& text) {
	clearTab();
	delete mLayout;
	mLayout = new QGridLayout{this};
	setLayout(mLayout);
	mLayout->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

	QLabel* textLabel = new QLabel{this};
	textLabel->setProperty("class", "tab-standalone-text");
	textLabel->setText(text);
	textLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	textLabel->setAlignment(Qt::AlignCenter);

	mLayout->addWidget(textLabel);
}
