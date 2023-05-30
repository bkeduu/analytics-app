#include "generationtab.h"
#include "etc/customwidgets.h"
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
	removeTabContents();
}

void GenerationTab::onAuthorized() {
	createTabContents();
}

void GenerationTab::createTabContents() {
	clearTab();
	delete this->layout;
	layout = new QGridLayout{this};
	this->setLayout(layout);
	CustomCheckbox* cb = new CustomCheckbox{this, tr("Diesel generator")};
	layout->addWidget(cb);

	connect(cb, &CustomCheckbox::checkboxClicked, cb, [=](bool newState) {
		mParent->onRelayClicked(4, newState);
	});
}

void GenerationTab::removeTabContents(const QString& text) {
	clearTab();
	delete this->layout;
	layout = new QGridLayout{this};
	this->setLayout(layout);
	layout->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

	QLabel* textLabel = new QLabel{this};
	textLabel->setText(text);
	textLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	textLabel->setAlignment(Qt::AlignCenter);

	layout->addWidget(textLabel);
}
