#pragma once

#include <QString>
#include <QWidget>
#include <QMutex>

class ITab : public QWidget {
	Q_OBJECT
public:
	ITab(QWidget* parent, const QString& name);
	QString getName() const;

	virtual ~ITab() = default;

private:
	const QString tabName;
};

