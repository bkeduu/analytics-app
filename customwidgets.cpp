#include "customwidgets.h"

CustomSlider::CustomSlider(QWidget *parent, const QString& text) : QWidget{parent} {

	QHBoxLayout* layout = new QHBoxLayout{this};
	layout->setAlignment(Qt::AlignLeft);

	QCheckBox* cb = new QCheckBox{this};
	QLabel* label = new QLabel{this};
	label->setText(text);

	this->setLayout(layout);
	layout->addWidget(cb);
	layout->addWidget(label);

	layout->setSpacing(5);
	layout->setContentsMargins(0, 0, 0, 0);
}
