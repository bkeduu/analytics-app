#include "callout.h"
#include "calloutview.h"

#include <QChart>
#include <QGraphicsLayout>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <QLineSeries>
#include <QMouseEvent>
#include <QSplineSeries>
#include <QResizeEvent>

CalloutView::CalloutView(QWidget* parent) : QGraphicsView{new QGraphicsScene, parent} {
    setDragMode(QGraphicsView::NoDrag);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFrameShape(QFrame::NoFrame);
    setBackgroundRole(QPalette::Window);
    setRenderHint(QPainter::Antialiasing);
    setMouseTracking(true);

	mChart = new QChart;

	QLineSeries* series = new QLineSeries;
    series->append(1, 3);
    series->append(4, 5);
    series->append(5, 4.5);
    series->append(7, 1);
    series->append(11, 2);
	mChart->addSeries(series);

	QSplineSeries* series2 = new QSplineSeries;
    series2->append(1.6, 1.4);
    series2->append(2.4, 3.5);
    series2->append(3.7, 2.5);
    series2->append(7, 4);
    series2->append(10, 2);
	mChart->addSeries(series2);

	mChart->createDefaultAxes();
	mChart->setAcceptHoverEvents(true);

	scene()->addItem(mChart);
}

CalloutView::~CalloutView() {
    delete scene();
}

void CalloutView::resizeEvent(QResizeEvent* event) {
    if (scene()) {
        scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
		mChart->resize(event->size());
		const auto callouts = mCallouts;
        for (Callout *callout : callouts)
            callout->updateGeometry();
    }

    resize(size());
}
