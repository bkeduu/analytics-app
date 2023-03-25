#pragma once

#include <QMutex>
#include <QLabel>
#include <QFrame>
#include <QLayout>

class ITab : public QFrame {
	Q_OBJECT
public:
	ITab(QWidget* parent, const QString& name);
	QString getName() const;

	virtual ~ITab() = default;

private:
	const QString tabName;
};

