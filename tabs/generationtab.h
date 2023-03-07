#pragma once

#include "itab.h"

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QString>

class GenerationTab : public ITab {
	Q_OBJECT
public:
	static GenerationTab* getWidget(const QString& tabName, QWidget* parent = nullptr);

private:
	GenerationTab(const QString& tabName, QWidget* parent = nullptr);
	QGridLayout* layout;

	static GenerationTab* instance;
	static QMutex lock;

	virtual ~GenerationTab() {};
};

