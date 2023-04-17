#pragma once

#include "itab.h"

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

class ConsumersTab : public ITab {
	Q_OBJECT
public:
	static ConsumersTab* getWidget(const QString& tabName, QWidget* parent = nullptr);
	void setJSONDocument(const QJsonDocument& document);

private:
	ConsumersTab(const QString& tabName, QWidget* parent = nullptr);

	void buildInterface();

	QHBoxLayout* layout;

	QJsonDocument config;

	static ConsumersTab* instance;
	static QMutex lock;

	QMap<QString, QWidget*> consumers_group1;
	QMap<QString, QWidget*> consumers_group2;
	QMap<QString, QWidget*> consumers_group3;

	QVector<QWidget*> group_widgets;

	virtual ~ConsumersTab() {};
};
