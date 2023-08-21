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

void CustomCheckbox::setCheckboxStatus(bool newStatus) {
	checkbox->setChecked(newStatus);
}

void CustomCheckbox::mousePressEvent(QMouseEvent* /*event*/) {
	checkbox->animateClick();
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

	updateWidgetsGeometry();
}

void CustomLayout::updateWidgetsGeometry() {
	foreach (QWidget* widget, widgets) {
		QRect geometry = widget->geometry();
		geometry.setX(0);
		geometry.setY(0);
		widget->setGeometry(geometry);
		widget->move(0 - widget->x(), 0 - widget->y());
	}
}
