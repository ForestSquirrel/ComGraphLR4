#include "QGraphicsControlPointItem.h"

QGraphicsControlPointItem::QGraphicsControlPointItem(QGraphicsEllipseItem *parent) : QGraphicsEllipseItem(parent) {
    QRectF temprect(0, 0, 15, 15);
    setRect(temprect);
}
void QGraphicsControlPointItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    QRectF rect = boundingRect();
    painter->setPen(this->outlinePen);
    painter->drawEllipse(rect);

    QRectF dotRect(rect.x() + 7, rect.y() + 7,
                   2,            2);
    painter->setPen(pen);
    painter->drawEllipse(dotRect);

    Q_UNUSED(option);
    Q_UNUSED(widget);

}
   int QGraphicsControlPointItem::type() const{
       return Type;
   }

