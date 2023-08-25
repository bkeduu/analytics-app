#include "generationtab.h"
#include "mainwindow.h"

GenerationTab::GenerationTab(const QString& tabName, QWidget* parent) : ITab{parent, tabName}, mLayout{nullptr} {
	createTabContents();
}

void GenerationTab::lock() {
	mCheckBox->lock();
}

void GenerationTab::unlock() {
	mCheckBox->unlock();
}

void GenerationTab::onSensorsDataReceived(const QJsonObject& data) {
	mCheckBox->blockSignals(true);
	mCheckBox->setCheckboxStatus(data.value("4").toArray().at(4).toInt());
	mCheckBox->blockSignals(false);
}

void GenerationTab::createTabContents() {
	clearTab();
	delete mLayout;
	mLayout = new QGridLayout{this};
	setLayout(mLayout);
	mCheckBox = new CustomCheckBox{this, tr("Diesel generator")};
	mLayout->addWidget(mCheckBox);

	connect(mCheckBox, &CustomCheckBox::checkboxClicked, mCheckBox, [=](bool newState) {
		mParent->onRelayClicked(3, newState);
	});
}

void GenerationTab::removeTabContents(const QString& text) {
	clearTab();
	delete mLayout;
	mLayout = new QGridLayout{this};
	setLayout(mLayout);
	mLayout->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

	QLabel* textLabel = new QLabel{this};
	textLabel->setProperty("class", "tab-standalone-text");
	textLabel->setText(text);
	textLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	textLabel->setAlignment(Qt::AlignCenter);

	mLayout->addWidget(textLabel);
}
