#include "consumerstab.h"
#include "mainwindow.h"
#include "calloutwidget.h"

ConsumersTab::ConsumersTab(const QString& tabName, QWidget* parent) : ITab{parent, tabName},
	mLayout{nullptr}, mConsumersGroups{3, {}}, mIsGroupSwitched{false, false, false}, mConsumersReceived{false} {
	createTabContents();
}

void ConsumersTab::lock() {
	foreach (CustomCheckBox* cb, mGroupRelays)
		cb->lock();
}

void ConsumersTab::unlock() {
	foreach (CustomCheckBox* cb, mGroupRelays)
		cb->unlock();
}

void ConsumersTab::onConsumersReceived(const QJsonObject& data) {
//	if (!data.contains("G1") || !data.contains("G2") || !data.contains("G3"))
//		throw InternalErrorException{tr("Data structure with wrong value received at %1. The app will be closed.").arg(FLF)};

//	for (int i = 0; i < 3; ++i) {
//		if (!data.contains(QString{"G%1"}.arg(i + 1)))
//			throw InternalErrorException{tr("Data structure with wrong value received at %1. The app will be closed.").arg(FLF)};

//		QJsonArray group = data.value(QString{"G%1"}.arg(i + 1)).toArray();
//		for (auto it = group.begin(); it != group.end(); ++it) {
//			QJsonObject consumer = (*it).toObject();

//			if (!consumer.contains("id") || !consumer.contains("name") || !consumer.contains("status"))
//				throw InternalErrorException{tr("Data structure with wrong value received at %1. The app will be closed.").arg(FLF)};

//			mConsumersGroups[i][consumer.value("id").toInt()] = {
//				consumer.value("id").toInt(),
//				consumer.value("name").toString(),
//				bool(consumer.value("status").toInt()),
//				nullptr
//			};
//		}
//	}

	mConsumersReceived = true;
}

void ConsumersTab::onSensorsDataReceived(const QJsonObject& data) {
	if (!data.contains("1") || !data.contains("2") || !data.contains("3"))
		throw InternalErrorException{tr("Data structure with wrong value received at %1. The app will be closed.").arg(FLF)};

	for (int i = 0; i < 3; ++i) {
		if (!data.contains(QString{"%1"}.arg(i + 1)))
			throw InternalErrorException{tr("Data structure with wrong value received at %1. The app will be closed.").arg(FLF)};

		if (mIsGroupSwitched[i]) {
			mIsGroupSwitched[i] = false;
		}
		else {
			mGroupRelays[i]->setCheckboxStatus(data.value(QString{"%1"}.arg(i + 1)).toArray().at(4).toInt());
		}
	}
}

void ConsumersTab::onGraphicsDataReceived(const QJsonObject& data) {
	if (!mPlot)
		createTabContents();

	if (!data.contains("prediction") || !data.contains("reality"))
		throw InternalErrorException{tr("Data structure with wrong value received at %1. The app will be closed.").arg(FLF)};

	QJsonArray realityData = data.value("reality").toArray();

	static const QStringList plotLabels{tr("1\'st group"), tr("2\'nd group"), tr("3\'rd group")};
	QStringList labels;

	QVector<QVector<double>> consumersValues{3, {}};

	for (auto it = realityData.begin(); it != realityData.end(); ++it) {
		QJsonArray value = (*it).toArray();

		labels << value.at(0).toString("");
		consumersValues[0] << value.at(4).toDouble(.0);
		consumersValues[1] << value.at(5).toDouble(.0);
		consumersValues[2] << value.at(6).toDouble(.0);
	}

	mPlot->onGraphicsData(labels, plotLabels, {}, consumersValues);
	mPlot->resize(size());
}

void ConsumersTab::createTabContents() {
	clearTab();
	delete mLayout;
	QVBoxLayout* layout = new QVBoxLayout{this};
	mLayout = layout;
	layout->setSpacing(30);

	mPlot = new StackedBarWithLinesWidget{this, "", "", "kW"};
	layout->addWidget(mPlot, 15);

	QWidget* groupsWidget = new QWidget{this};
	QHBoxLayout* groupsWidgetLayout = new QHBoxLayout{groupsWidget};
	groupsWidget->setLayout(groupsWidgetLayout);
	layout->addWidget(groupsWidget, 1);

	for (int i = 0; i < 3; ++i) {
		CustomCheckBox* groupCheckbox = new CustomCheckBox{groupsWidget, tr("Group %1").arg(i + 1)};
		mGroupRelays.push_back(groupCheckbox);
		groupsWidgetLayout->addWidget(groupCheckbox, 1, Qt::AlignCenter);
	}

	for (int i = 0; i < mGroupRelays.size(); ++i)
		connect(mGroupRelays[i], &CustomCheckBox::checkboxClicked, mGroupRelays[i], [this, i](bool newState) {
			mIsGroupSwitched[i] = true;
			mParent->onRelayClicked(i + 1, newState);
		});

	setLayout(mLayout);
}

void ConsumersTab::removeTabContents(const QString& text) {
	clearTab();
	delete mLayout;
	mLayout = new QGridLayout{this};
	this->setLayout(mLayout);
	mLayout->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

	QLabel* textLabel = new QLabel{this};
	textLabel->setProperty("class", "tab-standalone-text");
	textLabel->setText(text);
	textLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	textLabel->setAlignment(Qt::AlignCenter);

	mLayout->addWidget(textLabel);
}
