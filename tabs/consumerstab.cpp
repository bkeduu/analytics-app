#include "consumerstab.h"
#include "mainwindow.h"

ConsumersTab* ConsumersTab::instance = nullptr;
QMutex ConsumersTab::lock{};

ConsumersTab* ConsumersTab::getWidget(const QString& tabName, QWidget* parent) {
	QMutexLocker locker{&ConsumersTab::lock};
	if (!instance)
		instance = new ConsumersTab{tabName, parent};
	return instance;
}

ConsumersTab::ConsumersTab(const QString& tabName, QWidget* parent) : ITab{parent, tabName},
	layout{nullptr}, consumersGroups{3} { }

void ConsumersTab::setJSONDocument(const QJsonObject& object) {

	for (int i = 0; i < 3; ++i) {
		QJsonArray group = object.value(QString{"G%1"}.arg(i)).toArray();
		for (auto it = group.begin(); it != group.end(); ++it) {

			QJsonObject consumer = (*it).toObject();

			Consumer c {
				consumer.value("id").toInt(),
				consumer.value("name").toString(),
				bool(consumer.value("status").toInt()),
				nullptr
			};

			consumersGroups[i][c.id] = c;
		}
	}

	buildInterface();
}

void ConsumersTab::buildInterface() {
	layout = new QHBoxLayout{this};
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

		CustomCheckbox* groupCheckbox = new CustomCheckbox{groupWidgets[i], tr("Group %1").arg(i)};
		verticalLayout->addWidget(groupCheckbox);
		groupRelays.push_back(groupCheckbox);

		foreach (int consumerId, consumersGroups[i].keys()) {
			QWidget* consumerWidget = new CustomCheckbox{groupWidgets[i], consumersGroups[i][consumerId].name};
			verticalLayout->addWidget(consumerWidget);
			consumersGroups[i][consumerId].widget = consumerWidget;
		}
	}

	for (int i = 0; i < groupRelays.size(); ++i) {

		connect(groupRelays[i], &CustomCheckbox::checkboxClicked, groupRelays[i], [=](bool newState) {
			mParent->onRelayClicked(i, newState);
		});

	}

}
