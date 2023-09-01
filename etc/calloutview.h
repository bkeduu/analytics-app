#pragma once

#include <QGraphicsView>

QT_FORWARD_DECLARE_CLASS(QChart)
QT_FORWARD_DECLARE_CLASS(QGraphicsScene)
QT_FORWARD_DECLARE_CLASS(QGraphicsSimpleTextItem)
QT_FORWARD_DECLARE_CLASS(QGraphicsView)
QT_FORWARD_DECLARE_CLASS(QMouseEvent)
QT_FORWARD_DECLARE_CLASS(QResizeEvent)

class Callout;

class CalloutView : public QGraphicsView {
    Q_OBJECT
public:
	CalloutView(QWidget* parent = nullptr);
    ~CalloutView();

protected:
	void resizeEvent(QResizeEvent* event) override;

private:
	QChart* mChart = nullptr;
	QList<Callout*> mCallouts;
};
