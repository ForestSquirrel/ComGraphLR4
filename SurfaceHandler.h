#ifndef SURFACEHANDLER_H
#define SURFACEHANDLER_H
#include<QVector4D>
#include <QMatrix4x4>
#include<QGraphicsScene>
#include <QtMath>
#include<QGraphicsItem>

inline QVector4D rotAndProj(QVector4D point, float alpha, float beta, float gama){
    QMatrix4x4 to2D(1,0,0,0,
                    0,-1,0,0,
                    0,0,0,0,
                    0,0,0,1);
    QMatrix4x4 rotX( 1, 0,              0,           0,
                     0, qCos(alpha),    qSin(alpha), 0,
                     0, -1*qSin(alpha), qCos(alpha), 0,
                     0, 0,              0,           1);
    QMatrix4x4 rotY( qCos(beta),   0,  -1*qSin(beta), 0,
                     0,            1,  0,             0,
                     qSin(beta),   0,  qCos(beta),    0,
                     0,            0,  0,             1);
    QMatrix4x4 rotZ( qCos(gama),    qSin(gama), 0,  0,
                     -1*qSin(gama), qCos(gama), 0,  0,
                     0,             0,          1,  0,
                     0,             0,          0,  1);
    QMatrix4x4 Rot = rotZ*rotY*rotX;
    QVector4D res = point*Rot;
    res = res*to2D;
    QVector4D norm = res.normalized();
    QVector4D test = res*Rot.inverted()*to2D.inverted();
    return res;
}

inline void drawAxis(QGraphicsScene* scene, QGraphicsLineItem* itemToDraw){
    QVector4D zero(0,0,0,1);
    QVector4D x(200,0,0,1);
    QVector4D y(0,200,0,1);
    QVector4D z(0,0,200,1);
    float a = qDegreesToRadians(35.26439f);
    float b = qDegreesToRadians(45.0f);
    float c = qDegreesToRadians(0.0f);
    QVector4D nZero = rotAndProj(zero,a,b,c);
    QVector4D nX = rotAndProj(x,a,b,c);
    QVector4D nY = rotAndProj(y,a,b,c);
    QVector4D nZ = rotAndProj(z,a,b,c);
    //------------------------------------------------------------------------
    if(!itemToDraw){
        itemToDraw = new QGraphicsLineItem;
        scene->addItem(itemToDraw);
        itemToDraw->setPen(QPen(Qt::green, 1, Qt::SolidLine));
        itemToDraw->setPos(nZero.toPointF());
    }
    itemToDraw->setLine(QLineF(nZero.toPointF(), nX.toPointF()));
    itemToDraw = 0;
    //------------------------------------------------------------------------
    if(!itemToDraw){
        itemToDraw = new QGraphicsLineItem;
        scene->addItem(itemToDraw);
        itemToDraw->setPen(QPen(Qt::red, 2, Qt::SolidLine));
        itemToDraw->setPos(nZero.toPointF());
    }
    itemToDraw->setLine(QLineF(nZero.toPointF(), nY.toPointF()));
    itemToDraw = 0;
    //------------------------------------------------------------------------
    if(!itemToDraw){
        itemToDraw = new QGraphicsLineItem;
        scene->addItem(itemToDraw);
        itemToDraw->setPen(QPen(Qt::blue, 3, Qt::SolidLine));
        itemToDraw->setPos(nZero.toPointF());
    }
    itemToDraw->setLine(QLineF(nZero.toPointF(), nZ.toPointF()));
    itemToDraw = 0;
}
#endif // SURFACEHANDLER_H
