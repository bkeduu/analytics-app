#pragma once

#include "utils.h"

#include <QFrame>
#include <QSettings>
#include <QJsonObject>

class MainWindow;

class ITab : public QFrame {
	Q_OBJECT
public:
	ITab(QWidget* parent, const QString& name);
	QString getName() const;

	virtual void createTabContents() = 0;
	virtual void removeTabContents(const QString& text = "") = 0;

	virtual void load(QSettings&) { };
	virtual void save(QSettings&) { };

	virtual void lock() = 0;
	virtual void unlock() = 0;

	virtual ~ITab() = default;

private:
	const QString tabName;

protected:
	MainWindow* mParent;

	void clearTab() {
		while (QWidget* w = findChild<QWidget*>()) {
			delete w;
		}
	}
};

