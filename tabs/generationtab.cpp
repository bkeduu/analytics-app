#include "generationtab.h"
#include "mainwindow.h"

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
	CustomCheckbox* cb = new CustomCheckbox{this, tr("Diesel generator")};
	layout->addWidget(cb);

	connect(cb, &CustomCheckbox::checkboxClicked, cb, [=](bool newState) {
		mParent->onRelayClicked(4, newState);
	});
}
