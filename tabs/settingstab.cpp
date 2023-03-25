#include "settingstab.h"

SettingsTab* SettingsTab::instance = nullptr;
QMutex SettingsTab::lock{};

SettingsTab* SettingsTab::getWidget(const QString& tabName, QWidget* parent) {
	QMutexLocker locker{&SettingsTab::lock};
		if (!instance)
			instance = new SettingsTab{tabName, parent};
		return instance;
}

SettingsTab::SettingsTab(const QString& tabName, QWidget* parent) : ITab{parent, tabName}, layout{nullptr} {
	layout = new QGridLayout{this};


}
