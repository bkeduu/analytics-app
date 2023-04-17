#pragma once
#include <QWidget>
#include <QCheckBox>
#include <QLabel>
#include <QHBoxLayout>

class CustomCheckbox : public QWidget {
	Q_OBJECT
public:
	CustomCheckbox(QWidget *parent = nullptr, const QString& text = "");

signals:
	void checkboxClicked(bool);
};
