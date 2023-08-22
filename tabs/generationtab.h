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

	virtual ~GenerationTab() final override {};

public slots:
	virtual void onAuthorized() final override;
	virtual void onDataReceived(const QJsonObject&) final override;

private:
	QLayout* layout;
	CustomCheckBox* mCheckBox;
};

