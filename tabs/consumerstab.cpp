#include "consumerstab.h"
#include "customwidgets.h"

ConsumersTab* ConsumersTab::instance = nullptr;
QMutex ConsumersTab::lock{};

ConsumersTab* ConsumersTab::getWidget(const QString& tabName, QWidget* parent) {
	QMutexLocker locker{&ConsumersTab::lock};
	if (!instance)
		instance = new ConsumersTab{tabName, parent};
	return instance;
}

ConsumersTab::ConsumersTab(const QString& tabName, QWidget* parent) : ITab{parent, tabName}, layout{nullptr} {
	group_widgets.reserve(3);
}

void ConsumersTab::setJSONDocument(const QJsonDocument& document) {
	config = document;

	QJsonObject object = config.object();
	QJsonObject groups = object.value("consumers").toObject();

	QJsonArray group = groups["group_1"].toArray();
	for (auto it = group.begin(); it != group.end(); ++it)
		consumers_group1[(*it).toString()] = nullptr;

	group = groups["group_2"].toArray();
	for (auto it = group.begin(); it != group.end(); ++it)
		consumers_group2[(*it).toString()] = nullptr;

	group = groups["group_3"].toArray();
	for (auto it = group.begin(); it != group.end(); ++it)
		consumers_group3[(*it).toString()] = nullptr;

	buildInterface();
}

void ConsumersTab::buildInterface() {
	layout = new QHBoxLayout{this};
	layout->setSpacing(100);

	group_widgets.push_back(new QWidget{this});
	group_widgets.push_back(new QWidget{this});
	group_widgets.push_back(new QWidget{this});

	layout->addWidget(group_widgets[0]);
	layout->addWidget(group_widgets[1]);
	layout->addWidget(group_widgets[2]);

	QVBoxLayout* verticalLayout = new QVBoxLayout{group_widgets[0]};
	verticalLayout->setAlignment(Qt::AlignLeft);
	group_widgets[0]->setLayout(verticalLayout);

	foreach (QString consumer, consumers_group1.keys()) {
		QWidget* consumerWidget = new CustomSlider{group_widgets[0], consumer};
		verticalLayout->addWidget(consumerWidget);
		consumers_group1[consumer] = consumerWidget;
	}

	verticalLayout = new QVBoxLayout{group_widgets[1]};
	verticalLayout->setAlignment(Qt::AlignLeft);
	group_widgets[0]->setLayout(verticalLayout);

	foreach (QString consumer, consumers_group2.keys()) {
		QWidget* consumerWidget = new CustomSlider{group_widgets[1], consumer};
		verticalLayout->addWidget(consumerWidget);
		consumers_group2[consumer] = consumerWidget;
	}

	verticalLayout = new QVBoxLayout{group_widgets[2]};
	verticalLayout->setAlignment(Qt::AlignLeft);
	group_widgets[0]->setLayout(verticalLayout);

	foreach (QString consumer, consumers_group3.keys()) {
		QWidget* consumerWidget = new CustomSlider{group_widgets[2], consumer};
		verticalLayout->addWidget(consumerWidget);
		consumers_group3[consumer] = consumerWidget;
	}

}
