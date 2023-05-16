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

	virtual void onAuthorize() { }

	virtual void load(QSettings& settings) { }
	virtual void save(QSettings& settings) { }

	virtual ~ITab() = default;

private:
	const QString tabName;

protected:
	MainWindow* mParent;
};

