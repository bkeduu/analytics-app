#pragma once

#include <QAbstractButton>
#include <QString>
#include <QPainter>
#include <QState>

class BCSwitch: public QAbstractButton {
	Q_OBJECT
	Q_PROPERTY(int offset READ offset WRITE setOffset)

public:
	BCSwitch(QWidget *parent = nullptr);
	BCSwitch(Qt::Orientation orientation = Qt::Horizontal, const QString &on = tr("On"),
			 const QString &off = tr("Off"), bool checked = false, QWidget *parent = NULL);
	QSize sizeHint() const;

	int offset() const {
		return mOffset;
	}
	void setOffset(int o) {
		mOffset = o;
		update();
	}
public slots:
	void setChecked(bool checked);
protected:
	void paintEvent(QPaintEvent *e);
	void enterEvent(QEnterEvent *e);
	void resizeEvent(QResizeEvent *e);
private:
	QString mOn;
	QString mOff;
	int mOffset;
	int mHeight;
	int mMargin;
	QState *mRight;
	Qt::Orientation mOrientation;
};
