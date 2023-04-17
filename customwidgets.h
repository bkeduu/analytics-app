#pragma once
#include <QWidget>
#include <QCheckBox>
#include <QLabel>
#include <QHBoxLayout>

class CustomSlider : public QWidget {
	Q_OBJECT
public:
	CustomSlider(QWidget *parent = nullptr, const QString& text = "");
};
