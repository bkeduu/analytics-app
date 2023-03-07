#include "consumerstab.h"

ConsumersTab* ConsumersTab::instance = nullptr;
QMutex ConsumersTab::lock{};

ConsumersTab* ConsumersTab::getWidget(const QString& tabName, QWidget* parent) {
	QMutexLocker locker(&ConsumersTab::lock);
	if (!instance)
		instance = new ConsumersTab(tabName, parent);
	return instance;
}

ConsumersTab::ConsumersTab(const QString& tabName, QWidget* parent) : ITab(parent, tabName), layout(nullptr) {
	layout = new QGridLayout(this);

	this->setMinimumHeight(100);

	layout->addWidget(new QPushButton("1", this), 0, 1);
	layout->addWidget(new QCheckBox("2", this), 0, 2);
	layout->addWidget(new QCheckBox("3", this), 0, 3);
	layout->addWidget(new QPushButton("4", this), 0, 4);
}
