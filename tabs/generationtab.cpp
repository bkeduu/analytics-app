#include "generationtab.h"
#include "etc/customwidgets.h"
#include "mainwindow.h"

GenerationTab::GenerationTab(const QString& tabName, QWidget* parent) : ITab{parent, tabName}, layout{nullptr} {
	createTabContents();
}

void GenerationTab::onAuthorized() {
	createTabContents();
}

void GenerationTab::onTabOpened() {

}

void GenerationTab::onDataReceived(const QJsonObject&) {

}

void GenerationTab::createTabContents() {
	clearTab();
	delete this->layout;
	layout = new QGridLayout{this};
	this->setLayout(layout);
	CustomCheckbox* cb = new CustomCheckbox{this, tr("Diesel generator")};
	layout->addWidget(cb);

	connect(cb, &CustomCheckbox::checkboxClicked, cb, [=](bool newState) {
		mParent->onRelayClicked(3, newState);
	});
}

void GenerationTab::removeTabContents(const QString& text) {
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
