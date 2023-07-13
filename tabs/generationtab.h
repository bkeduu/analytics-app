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

	virtual void createTabContents() final override;
	virtual void removeTabContents(const QString& text = tr("You need to authorize before starting")) final override;

public slots:
	virtual void onAuthorized() final override;
	virtual void onTabOpened() final override;

private:
	GenerationTab(const QString& tabName, QWidget* parent = nullptr);
	QLayout* layout;

	static GenerationTab* instance;
	static QMutex lock;

	virtual ~GenerationTab() {};
};

