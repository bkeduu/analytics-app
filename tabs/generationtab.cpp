#include "generationtab.h"

GenerationTab* GenerationTab::instance = nullptr;
QMutex GenerationTab::lock{};

GenerationTab* GenerationTab::getWidget(const QString& tabName, QWidget* parent) {
	QMutexLocker locker{&GenerationTab::lock};
		if (!instance)
			instance = new GenerationTab{tabName, parent};
		return instance;
}

GenerationTab::GenerationTab(const QString& tabName, QWidget* parent) : ITab{parent, tabName}, layout{nullptr} {
	layout = new QGridLayout{this};

}
