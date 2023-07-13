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

public slots:
	void setJSONDocument(const QJsonObject& document);
	virtual void onAuthorized() final override;
	virtual void onTabOpened() final override;

private:
	ConsumersTab(const QString& tabName, QWidget* parent = nullptr);

	virtual void createTabContents() final override;
	virtual void removeTabContents(const QString& text = tr("You need to authorize before starting")) final override;

	QLayout* layout;

	static ConsumersTab* instance;
	static QMutex lock;

	QVector<QMap<int, Consumer>> consumersGroups;

	QVector<CustomCheckbox*> groupRelays;
	QVector<QFrame*> groupWidgets;

	virtual ~ConsumersTab() {};
};
