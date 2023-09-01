#pragma once

#include <QFont>
#include <QGraphicsItem>

QT_FORWARD_DECLARE_CLASS(QChart)
QT_FORWARD_DECLARE_CLASS(QGraphicsSceneMouseEvent)

class Callout : public QGraphicsItem {
public:
	Callout(QChart* parent);

	void setText(const QString& text);
    void setAnchor(QPointF point);
    void updateGeometry();

    QRectF boundingRect() const override;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

private:
	QString mText;
	QRectF mTextRect;
	QRectF mRect;
	QPointF mAnchor;
	QFont mFont;
	QChart* mChart = nullptr;
};
