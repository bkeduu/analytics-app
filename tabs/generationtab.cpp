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
	if (!data.contains("gen"))
		throw InternalErrorException{tr("Data structure with wrong value received at %1. The app will be closed.").arg(FLF)};

	QJsonArray generatorArray = data.value("gen").toArray();
	if (generatorArray.isEmpty() || generatorArray.size() < 5)
		throw InternalErrorException{tr("Data structure with wrong value received at %1. The app will be closed.").arg(FLF)};

	mCheckBox->setCheckboxStatus(generatorArray[4].toInt());
}

void GenerationTab::createTabContents() {
	clearTab();
	delete mLayout;
	mLayout = new QGridLayout{this};
	setLayout(mLayout);
	mCheckBox = new CustomCheckBox{this, tr("Diesel generator")};
	mLayout->addWidget(mCheckBox);

	connect(mCheckBox, &CustomCheckBox::checkboxClicked, mCheckBox, [=](bool newState) {
		mParent->onRelayClicked(4, newState);
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
