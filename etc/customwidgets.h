#pragma once

#include <QCheckBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QLineEdit>

class CustomCheckbox : public QWidget {
	Q_OBJECT

public:
	CustomCheckbox(QWidget *parent = nullptr, const QString& text = "");
	void setCheckboxStatus(bool newStatus);

protected:
	void mousePressEvent(QMouseEvent* event) final override;

signals:
	void checkboxClicked(bool);

private:
	QCheckBox* checkbox;
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
