#include "calloutview.h"
#include "calloutwidget.h"

CalloutWidget::CalloutWidget(QWidget *parent) : ContentWidget(parent) {
    // To make mouse tracking work, we need to subclass the QGraphicsView instead of just having
    // it as member of this class
	mView = new CalloutView(this);
	mView->resize(size());
}

void CalloutWidget::resizeEvent(QResizeEvent *) {
	mView->resize(size());
}
