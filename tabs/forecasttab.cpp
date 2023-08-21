#include "forecasttab.h"

ForecastTab::ForecastTab(const QString& tabName, QWidget* parent) : ITab{parent, tabName}, layout{nullptr} {
	removeTabContents(tr("In development"));
}

void ForecastTab::onAuthorized() {

}

void ForecastTab::onDataReceived(const QJsonObject&) {

}

void ForecastTab::createTabContents() {
  // todo
}

void ForecastTab::removeTabContents(const QString& text) {
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
