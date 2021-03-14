#pragma once
#ifndef LINE_H
#define LINE_H
#include <QGraphicsLineItem>
#include <assist.h>
class line : public QLineF {
public:
    QGraphicsLineItem* graphicsPtr;

    line(QGraphicsLineItem* lineItem) : QLineF(lineItem->line()) {
        this->graphicsPtr = lineItem;
    }
    line(QPointF p1, QPointF p2) : QLineF(p1, p2) {
        this->graphicsPtr = NULL;
    }
    line(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsLineItem* ptr) : QLineF(x1, y1, x2, y2) {
        this->graphicsPtr = ptr;
    }

    bool OnLeft(QPointF p){
        QPointF ab = QPointF(this->p2().x() - this->p1().x(),
                             this->p2().y() - this->p1().y());
        QPointF ap = QPointF(p.x() - this->p1().x(),
                             p.y() - this->p1().y());
        return Cross(ab, ap) >= 0;
    }
    QPointF getNormal(){
        return QPointF(this->p2().y() - this->p1().y(),
                       this->p1().x() - this->p2().x());
    }

    QPointF getDirection(){
        return QPointF(this->p2().x() - this->p1().x(),
                       this->p2().y() - this->p1().y());
    }

    float IntersectionParameter(line that) {
        line edge = that;
        QPointF segmentToEdge = Sub(edge.p1(),
                                    this->p1());
        QPointF segmentDir = this->getDirection();
        QPointF edgeDir = edge.getDirection();

        float t = Cross(edgeDir, segmentToEdge) / Cross(edgeDir, segmentDir);

        if (t != t){ //isNaN
            t = 0;
        }

        return t;
    }

    line Morph(float tA, float tB){
        QPointF d = this->getDirection();
        return line(Add(this->p1(), Mul(d, tA)),
                    Add(this->p1(), Mul(d, tB)));
    }
};
#endif // LINE_H
