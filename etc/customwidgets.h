#pragma once

#include <QCheckBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QLineEdit>

class CustomCheckbox : public QWidget {
	Q_OBJECT

public:
	CustomCheckbox(QWidget *parent = nullptr, const QString& text = "");

protected:
	void mousePressEvent(QMouseEvent* event) final override;

signals:
	void checkboxClicked(bool);

private:
	QCheckBox* checkbox;
};


class CustomLoginField : public QWidget {
	Q_OBJECT

public:
	CustomLoginField(const QString& labelText, const QString& placeholder = "", QWidget* parent = nullptr);
	void setPasswordMode(bool state);
	QString text() const;

private:
	QLabel* textLabel;
	QLineEdit* inputWidget;
};
