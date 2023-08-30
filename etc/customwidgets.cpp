#include "customwidgets.h"

CustomCheckBox::CustomCheckBox(QWidget* parent, const QString& text, const QString& hint) : QWidget{parent} {
	QHBoxLayout* layout = new QHBoxLayout{this};
	layout->setAlignment(Qt::AlignLeft);

	button = new BCSwitch{this};
	QLabel* label = new QLabel{this};
	label->setText(text);

	this->setLayout(layout);
	layout->addWidget(button);
	layout->addWidget(label);

	layout->setSpacing(5);
	layout->setContentsMargins(0, 0, 0, 0);

	if (hint.length())
		setToolTip(hint);

	connect(button, &QCheckBox::clicked, button, [&](bool newState) {
		emit checkboxClicked(newState);
	});
}

void CustomCheckBox::setCheckboxStatus(bool newStatus) {
	button->setChecked(newStatus);
}

void CustomCheckBox::mousePressEvent(QMouseEvent* /*event*/) {
	button->animateClick();
}


CustomRadioButton::CustomRadioButton(QWidget* parent, const QString& text, const QString& hint) : QWidget{parent} {
	QHBoxLayout* layout = new QHBoxLayout{this};
	layout->setAlignment(Qt::AlignLeft);

	button = new QRadioButton{this};
	QLabel* label = new QLabel{this};
	label->setText(text);

	this->setLayout(layout);
	layout->addWidget(button);
	layout->addWidget(label);

	layout->setSpacing(5);
	layout->setContentsMargins(0, 0, 0, 0);

	if (hint.length())
		setToolTip(hint);

	connect(button, &QRadioButton::clicked, button, [&](bool newState) {
		emit buttonClicked(newState);
	});
}

void CustomRadioButton::setButtonStatus(bool newStatus) {
	button->setChecked(newStatus);
}

void CustomRadioButton::mousePressEvent(QMouseEvent* /*event*/) {
	button->animateClick();
}


CustomLineEdit::CustomLineEdit(const QString& labelText, const QString& placeholder, QWidget* parent) : QWidget{parent} {
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

void CustomLineEdit::setPasswordMode(bool state) {
	if (state) {
		inputWidget->setEchoMode(QLineEdit::Password);
	}
	else {
		inputWidget->setEchoMode(QLineEdit::Normal);
	}
}

void CustomLineEdit::setText(const QString& text) {
	inputWidget->setText(text);
}

QString CustomLineEdit::text() const {
	return inputWidget->text();
}

CustomLayout::CustomLayout(QWidget* parent) : QGridLayout{parent} { }

void CustomLayout::setGeometry(const QRect& rect) {
	QRect copy{rect};
	copy.setX(0);
	copy.setY(0);
	QGridLayout::setGeometry(copy);

	updateWidgetsGeometry();
}

void CustomLayout::addWidget(QWidget* widget, int row, int column, Qt::Alignment alignment) {
	widgets.push_back(widget);
	QGridLayout::addWidget(widget, row, column, alignment);

	widget->adjustSize();

	updateWidgetsGeometry();
}

void CustomLayout::updateWidgetsGeometry() {
	foreach (QWidget* widget, widgets) {
		QRect geometry = widget->geometry();
		geometry.setX(0);
		geometry.setY(0);

		widget->move(0, 0);
	}
}
