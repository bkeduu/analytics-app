#pragma once

#include <QMutex>
#include <QLabel>
#include <QFrame>
#include <QLayout>
#include <QSettings>

class MainWindow;

class ITab : public QFrame {
	Q_OBJECT
public:
	ITab(QWidget* parent, const QString& name);
	QString getName() const;

	virtual void createTabContents() = 0;
	virtual void removeTabContents(const QString& text = tr("You need to authorize before starting")) = 0;

	virtual void load(QSettings&) { };
	virtual void save(QSettings&) { };

	virtual ~ITab() = default;

public slots:
	virtual void onAuthorized() = 0;
	virtual void onTabOpened() = 0;

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

