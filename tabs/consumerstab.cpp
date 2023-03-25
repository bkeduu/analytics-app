#include "consumerstab.h"

ConsumersTab* ConsumersTab::instance = nullptr;
QMutex ConsumersTab::lock{};

ConsumersTab* ConsumersTab::getWidget(const QString& tabName, QWidget* parent) {
	QMutexLocker locker{&ConsumersTab::lock};
	if (!instance)
		instance = new ConsumersTab{tabName, parent};
	return instance;
}

ConsumersTab::ConsumersTab(const QString& tabName, QWidget* parent) : ITab{parent, tabName}, layout{nullptr} {
	layout = new QGridLayout{this};


}
