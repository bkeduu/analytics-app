#pragma once

#include "itab.h"
#include "etc/customwidgets.h"

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QString>

class GenerationTab final : public ITab {
	Q_OBJECT
public:
	GenerationTab(const QString& tabName, QWidget* parent = nullptr);

	virtual void createTabContents() final override;
	virtual void removeTabContents(const QString& text = tr("You need to authorize before starting")) final override;

	virtual void lock() final override;
	virtual void unlock() final override;

	void onSensorsDataReceived(const QJsonObject&);

	virtual ~GenerationTab() final override {};

private:
	QLayout* mLayout;
	CustomCheckBox* mCheckBox;
	bool mTabCreated;
};

