#include "consumerstab.h"
#include "mainwindow.h"

ConsumersTab::ConsumersTab(const QString& tabName, QWidget* parent) : ITab{parent, tabName},
	layout{nullptr}, consumersGroups{3, {}}, mConsumersReceived{false} {
	removeTabContents(tr("Waiting for consumers data from server"));
}

void ConsumersTab::lock() {
	foreach (CustomCheckBox* cb, groupRelays)
		cb->lock();
}

void ConsumersTab::unlock() {
	foreach (CustomCheckBox* cb, groupRelays)
		cb->unlock();
}

void ConsumersTab::onConsumersReceived(const QJsonObject& data) {
	if (!data.contains("G1") || !data.contains("G2") || !data.contains("G3"))
		throw InternalErrorException{tr("Data structure with wrong value received at %1. The app will be closed.").arg(FLF)};

	for (int i = 0; i < 3; ++i) {
		if (!data.contains(QString{"G%1"}.arg(i + 1)))
			throw InternalErrorException{tr("Data structure with wrong value received at %1. The app will be closed.").arg(FLF)};

		QJsonArray group = data.value(QString{"G%1"}.arg(i + 1)).toArray();
		for (auto it = group.begin(); it != group.end(); ++it) {
			QJsonObject consumer = (*it).toObject();

			if (!consumer.contains("id") || !consumer.contains("name") || !consumer.contains("status"))
				throw InternalErrorException{tr("Data structure with wrong value received at %1. The app will be closed.").arg(FLF)};

			consumersGroups[i][consumer.value("id").toInt()] = {
				consumer.value("id").toInt(),
				consumer.value("name").toString(),
				bool(consumer.value("status").toInt()),
				nullptr
			};
		}
	}

	createTabContents();
	mConsumersReceived = true;
}

void ConsumersTab::onSensorsDataReceived(const QJsonObject& data) {
	if (!data.contains("1") || !data.contains("2") || !data.contains("3"))
		throw InternalErrorException{tr("Data structure with wrong value received at %1. The app will be closed.").arg(FLF)};

	if (mConsumersReceived) {
		for (int i = 0; i < 3; ++i) {
			if (!data.contains(QString{"%1"}.arg(i + 1)))
				throw InternalErrorException{tr("Data structure with wrong value received at %1. The app will be closed.").arg(FLF)};
			groupRelays[i]->setCheckboxStatus(data.value(QString{"%1"}.arg(i + 1)).toArray().at(4).toInt());
		}
	}
}

void ConsumersTab::createTabContents() {
	clearTab();
	delete this->layout;
	layout = new QHBoxLayout{this};
	this->setLayout(layout);
	layout->setSpacing(30);

	groupWidgets.push_back(new QFrame{this});
	groupWidgets.push_back(new QFrame{this});
	groupWidgets.push_back(new QFrame{this});

	for (int i = 0; i < groupWidgets.size(); ++i) {
		layout->addWidget(groupWidgets[i]);
		groupWidgets[i]->setFrameShape(QFrame::Box);

		QVBoxLayout* verticalLayout = new QVBoxLayout{groupWidgets[i]};
		verticalLayout->setAlignment(Qt::AlignLeft);
		groupWidgets[i]->setLayout(verticalLayout);

		CustomCheckBox* groupCheckbox = new CustomCheckBox{groupWidgets[i], tr("Group %1").arg(i + 1)};
		verticalLayout->addWidget(groupCheckbox);
		groupRelays.push_back(groupCheckbox);

		foreach (int consumerId, consumersGroups[i].keys()) {
			QWidget* consumerWidget = new CustomCheckBox{groupWidgets[i], consumersGroups[i][consumerId].name};
			verticalLayout->addWidget(consumerWidget);
			consumersGroups[i][consumerId].widget = consumerWidget;
		}
	}

	for (int i = 0; i < groupRelays.size(); ++i)
		connect(groupRelays[i], &CustomCheckBox::checkboxClicked, groupRelays[i], [=](bool newState) {
			mParent->onRelayClicked(i, newState);
		});
}

void ConsumersTab::removeTabContents(const QString& text) {
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
