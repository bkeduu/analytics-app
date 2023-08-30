#include "bcswitch.h"

#include <QStateMachine>
#include <QSignalTransition>
#include <QPropertyAnimation>
#include <QMouseEvent>

BCSwitch::BCSwitch(QWidget *parent) : QAbstractButton(parent), mOn(tr("On")), mOff(tr("Off")) {
	setCheckable(true);
	mOrientation = Qt::Horizontal;
	mHeight = 22;
	mMargin = 3;
	mOffset = 0;
	bool vertical = mOrientation == Qt::Vertical;

	QStateMachine *machine = new QStateMachine;

	QState *left = new QState(machine);
	left->assignProperty(this, "offset", mHeight / 2);

	mRight = new QState(machine);
	mRight->assignProperty(this, "offset", (vertical ? height() : width()) - mHeight);

	QSignalTransition *transition1 = left->addTransition(this,
														 SIGNAL(toggled(bool)), mRight);
	transition1->addAnimation(new QPropertyAnimation(this, "offset"));

	QSignalTransition *transition2 = mRight->addTransition(this,
														   SIGNAL(toggled(bool)), left);
	transition2->addAnimation(new QPropertyAnimation(this, "offset"));

	/* for whatever reason QSignalTransition doesnt trigger when
	 * toggled() signal received but widget not visible */
	machine->setInitialState(left);
	setToolTip(mOff);
	machine->start();
	setChecked(false);
}

BCSwitch::BCSwitch(Qt::Orientation orientation /* = Qt::Horizontal */,
				   const QString &on /* = "On" */, const QString &off /* = "Off" */,
				   bool checked /* = false */, QWidget *parent /* = NULL */) :
	QAbstractButton(parent), mOn(on), mOff(off) {
	setCheckable(true);
	mOrientation = orientation;
	mHeight = 16;
	mMargin = 3;
	mOffset = mHeight / 2;
	bool vertical = mOrientation == Qt::Vertical;

	QStateMachine *machine = new QStateMachine;

	QState *left = new QState(machine);
	left->assignProperty(this, "offset", mHeight / 2);

	mRight = new QState(machine);
	mRight->assignProperty(this, "offset", (vertical ? height() : width()) - mHeight);

	QSignalTransition *transition1 = left->addTransition(this,
														 SIGNAL(toggled(bool)), mRight);
	transition1->addAnimation(new QPropertyAnimation(this, "offset"));

	QSignalTransition *transition2 = mRight->addTransition(this,
														   SIGNAL(toggled(bool)), left);
	transition2->addAnimation(new QPropertyAnimation(this, "offset"));

	/* for whatever reason QSignalTransition doesnt trigger when
	 * toggled() signal received but widget not visible */
	if (checked) {
		machine->setInitialState(mRight);
	}
	else {
		machine->setInitialState(left);
	}
	machine->start();
	setChecked(checked);
}

void BCSwitch::setChecked(bool checked) {
	if (checked != isChecked()) {
		this->click();
	}
}

void BCSwitch::paintEvent(QPaintEvent*) {
	bool vertical = mOrientation == Qt::Vertical;
	QPainter p(this);
	p.setPen(hasFocus() ? Qt::SolidLine : Qt::NoPen);
	p.setRenderHint(QPainter::Antialiasing, true);
	int w = (vertical ? height() : width()) - 2 * mMargin;
	int h = (vertical ? width() : height()) - 2 * mMargin;
	if (vertical) {
		p.translate(width() / 2, height() / 2);
		p.rotate(-90);
		p.translate(- height() / 2, - width() / 2);
	}
	if (isEnabled()) {
		p.setBrush(isChecked() ? QColor("#009688") : Qt::black);
		p.setOpacity(isChecked() ? 0.5 : 0.38);
		p.drawRoundedRect(QRect(mMargin, mMargin, w, h), mHeight / 2, mHeight / 2);
		p.setBrush(isChecked() ? QColor("#009688") : QColor("#d5d5d5"));
	} else {
		p.setBrush(isChecked() ? QColor("#77BCA6") : Qt::black);
		p.setOpacity(isChecked() ? 0.5 : 0.12);
		p.drawRoundedRect(QRect(mMargin, mMargin, w, h), mHeight / 2, mHeight / 2);
		p.setBrush(isChecked() ? QColor("#77BCA6") : QColor("#BDBDBD"));
	}
	p.setOpacity(1.0);
	if (vertical)
		p.drawEllipse(QRectF(offset() - (mHeight / 2), 0.0, width(), width()));
	else {
		if (isChecked())
			p.drawEllipse(QRectF(offset() - (mHeight / 3), 0.0, height(), height()));
		else
			p.drawEllipse(QRectF(offset() - (mHeight / 2), 0.0, height(), height()));
	}
}

void BCSwitch::enterEvent(QEnterEvent* e) {
	setCursor(Qt::PointingHandCursor);
	QAbstractButton::enterEvent(e);
}

void BCSwitch::resizeEvent(QResizeEvent* e) {
	int end = mOrientation == Qt::Vertical ? e->size().height() : e->size().width();
	mRight->assignProperty(this, "offset", end - mHeight);
	if(isChecked()){
		mOffset = end - mHeight;
	}
	QAbstractButton::resizeEvent(e);
}

QSize BCSwitch::sizeHint() const
{
	if (mOrientation == Qt::Vertical)
		return QSize(mHeight + 2 * mMargin, 2 * (mHeight + mMargin));
	else
		return QSize(2 * (mHeight + mMargin), mHeight + 2 * mMargin);
}
