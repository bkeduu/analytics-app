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

class ConsumersTab : public ITab {
	Q_OBJECT
public:
	static ConsumersTab* getWidget(const QString& tabName, QWidget* parent = nullptr);
	void setJSONDocument(QJsonObject document);

private:
	ConsumersTab(const QString& tabName, QWidget* parent = nullptr);

	void buildInterface();

	QHBoxLayout* layout;

	static ConsumersTab* instance;
	static QMutex lock;

	QVector<QMap<int, Consumer>> consumersGroups;

	QVector<CustomCheckbox*> groupRelays;
	QVector<QFrame*> groupWidgets;

	virtual ~ConsumersTab() {};
};
