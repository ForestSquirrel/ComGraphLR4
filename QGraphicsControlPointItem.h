#ifndef QGRAPHICSCONTROLPOINTITEM_H
#define QGRAPHICSCONTROLPOINTITEM_H

#include <QPainter>
#include <QGraphicsItem>
// class for customization
class QGraphicsControlPointItem :public QGraphicsEllipseItem {
public:
    enum { Type = UserType + 1 };
    int type() const override;
    explicit QGraphicsControlPointItem(QGraphicsEllipseItem *parent = nullptr);
    QPen outlinePen = QPen(Qt::gray, 2, Qt::DashLine);
    QPen pen = QPen(Qt::black, 2, Qt::SolidLine);
    void paint(QPainter * painter,
               const QStyleOptionGraphicsItem * option,
               QWidget * widget) override;
};

#endif // QGRAPHICSCONTROLPOINTITEM_H
