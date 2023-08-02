#pragma once

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

	virtual ~ITab() = default;

public slots:
	virtual void onAuthorized() = 0;
	virtual void onTabOpened() = 0;
	virtual void onDataReceived(const QJsonObject&) = 0;

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

