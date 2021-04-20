#ifndef SURFACEHANDLER_H
#define SURFACEHANDLER_H
#include<QVector4D>
#include <QMatrix4x4>
#include<QGraphicsScene>
#include <QtMath>
#include<QGraphicsItem>
#include <QDebug>

inline QVector4D rotAndProj(QPointF point, float alpha, float beta, float gama){
    QVector4D temp(point);
    temp.setZ(1);
    temp.setW(1);
    QMatrix4x4 to2D(1,0,0,0,
                    0,1,0,0,
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
    QVector4D res = temp*Rot;
    res = res*to2D;
    return res;
}
inline QVector4D rot(QVector4D point, float alpha, float beta, float gama){
    QVector4D temp(point);
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
    QVector4D res = temp*Rot;
    return res;
}
inline QVector4D rotAndProj(QVector4D point, float alpha, float beta, float gama){
    QMatrix4x4 to2D(1,0,0,0,
                    0,1,0,0,
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
    return res;
}

inline QPointF rotAndProjToPoint(QVector4D point, float alpha, float beta, float gama){
    QMatrix4x4 to2D(1,0,0,0,
                    0,1,0,0,
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
    return res.toPointF();
}

inline QVector4D rotBack(QVector4D point, float alpha, float beta, float gama){
    QMatrix4x4 to2D(1,0,0,0,
                    0,1,0,0,
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
    QVector4D res = point*Rot.inverted();
    return res;
}
inline void drawAxis(QGraphicsLineItem* xAxis,  QGraphicsLineItem* yAxis,  QGraphicsLineItem* zAxis, float a, float b, float c){
    QVector4D zero(0,0,0,1);
    QVector4D x(200,0,0,1);
    QVector4D y(0,200,0,1);
    QVector4D z(0,0,200,1);
    QVector4D nZero = rotAndProj(zero,a,b,c);
    QVector4D nX = rotAndProj(x,a,b,c);
    QVector4D nY = rotAndProj(y,a,b,c);
    QVector4D nZ = rotAndProj(z,a,b,c);
    //------------------------------------------------------------------------
    xAxis->setPos(nZero.toPointF());
    xAxis->setLine(QLineF(nZero.toPointF(), nX.toPointF()));
    //------------------------------------------------------------------------
    yAxis->setPos(nZero.toPointF());
    yAxis->setLine(QLineF(nZero.toPointF(), nY.toPointF()));
    //------------------------------------------------------------------------
    zAxis->setPos(nZero.toPointF());
    zAxis->setLine(QLineF(nZero.toPointF(), nZ.toPointF()));
}
#endif // SURFACEHANDLER_H
