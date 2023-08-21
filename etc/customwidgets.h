#pragma once

#include <QAbstractButton>
#include <QLabel>
#include <QCheckBox>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QLineEdit>

class CustomCheckBox : public QWidget {
	Q_OBJECT

public:
	CustomCheckBox(QWidget *parent = nullptr, const QString& text = "", const QString& hint = "");
	void setCheckboxStatus(bool newStatus);

protected:
	void mousePressEvent(QMouseEvent* event);

signals:
	void checkboxClicked(bool);

private:
	QCheckBox* button;
};


class CustomRadioButton : public QWidget {
	Q_OBJECT

public:
	CustomRadioButton(QWidget *parent = nullptr, const QString& text = "", const QString& hint = "");
	void setButtonStatus(bool newStatus);
	QRadioButton* getButton() const { return button; }

protected:
	void mousePressEvent(QMouseEvent* event);

signals:
	void buttonClicked(bool);

private:
	QRadioButton* button;
};


class CustomLineEdit : public QWidget {
	Q_OBJECT

public:
	CustomLineEdit(const QString& labelText, const QString& placeholder = "", QWidget* parent = nullptr);
	void setPasswordMode(bool state);
	void setText(const QString& text);
	QString text() const;

private:
	QLabel* textLabel;
	QLineEdit* inputWidget;
};

class CustomLayout final : public QGridLayout {
	Q_OBJECT

public:
	CustomLayout(QWidget* parent = nullptr);
	virtual void setGeometry(const QRect& rect) final override;

	void addWidget(QWidget* widget, int row = 0, int column = 0, Qt::Alignment = Qt::Alignment());

private:
	void updateWidgetsGeometry();

	QList<QWidget*> widgets;
};
