#include "customwidgets.h"

CustomCheckbox::CustomCheckbox(QWidget *parent, const QString& text) : QWidget{parent} {

	QHBoxLayout* layout = new QHBoxLayout{this};
	layout->setAlignment(Qt::AlignLeft);

	checkbox = new QCheckBox{this};
	QLabel* label = new QLabel{this};
	label->setText(text);

	this->setLayout(layout);
	layout->addWidget(checkbox);
	layout->addWidget(label);

	layout->setSpacing(5);
	layout->setContentsMargins(0, 0, 0, 0);

	connect(checkbox, &QCheckBox::clicked, checkbox, [&](bool newState) {
		emit checkboxClicked(newState);
	});
}

void CustomCheckbox::mousePressEvent(QMouseEvent* /*event*/) {
	checkbox->animateClick();
}


CustomLoginField::CustomLoginField(const QString& labelText, const QString& placeholder, QWidget* parent) : QWidget{parent} {
	QHBoxLayout* widgetLayout = new QHBoxLayout{this};
	setLayout(widgetLayout);

	textLabel = new QLabel{this};
	textLabel->setText(labelText);

	widgetLayout->addWidget(textLabel, 1, Qt::AlignLeft);

	inputWidget = new QLineEdit{this};
	inputWidget->setPlaceholderText(placeholder);

	widgetLayout->addWidget(inputWidget, 1, Qt::AlignCenter);

	setMinimumSize(QSize{350, 50});
	setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}

void CustomLoginField::setPasswordMode(bool state) {
	if (state) {
		inputWidget->setEchoMode(QLineEdit::Password);
	}
	else {
		inputWidget->setEchoMode(QLineEdit::Normal);
	}
}

QString CustomLoginField::text() const {
	return inputWidget->text();
}
