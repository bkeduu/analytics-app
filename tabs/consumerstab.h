#pragma once

#include "itab.h"
#include "etc/customwidgets.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QSlider>
#include <QVector>
#include <QString>

class MainWindow;

struct Consumer {
	int id;
	QString name;
	bool is_on;
	QWidget* widget;
};

class ConsumersTab final : public ITab {
	Q_OBJECT
public:
	ConsumersTab(const QString& tabName, QWidget* parent = nullptr);

	virtual void lock() final override;
	virtual void unlock() final override;

	void onConsumersReceived(const QJsonObject&);
	void onSensorsDataReceived(const QJsonObject&);

	virtual ~ConsumersTab() final override { };

private:
	virtual void createTabContents() final override;
	virtual void removeTabContents(const QString& text = tr("You need to authorize before starting")) final override;

	QLayout* layout;
	QVector<QMap<int, Consumer>> consumersGroups;
	QVector<CustomCheckBox*> groupRelays;
	QVector<QFrame*> groupWidgets;

	bool mConsumersReceived;
};
