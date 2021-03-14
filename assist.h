#pragma once
#ifndef ASSIST_H
#define ASSIST_H
#include <QPointF>
#include <QVector>
#include <QDebug>
inline QPointF Add(QPointF a, QPointF b){
    return QPointF(a.x() + b.x(),
                   a.y() + b.y());
}

inline QPointF Sub(QPointF a, QPointF b){
    return QPointF(a.x() - b.x(),
                   a.y() - b.y());
}

inline QPointF Mul(QPointF a, float b){
    return QPointF(a.x() * b,
                   a.y() * b);
}

inline float Dot(QPointF a, QPointF b){
    return a.x() * b.x() + a.y() * b.y();
}

inline float Cross(QPointF a, QPointF b){
    return a.x() * b.y() - a.y() * b.x();
}

//Gift wraapping implemenattion
inline int orientation(QPointF a, QPointF b, QPointF c) {
   int v = (b.y() - a.y()) * (c.x() - b.x()) -
           (b.x() - a.x()) * (c.y() - b.y());
   if (v == 0)
      return 0; // colinear
   else
      return (v > 0) ? 1 : 2; // clock or counterclock wise
}

inline QVector<QPointF> giftWrapping(QVector<QPointF> points) {
    if (points.size() < 3)//at least three points required
        qDebug() << "missing points";

    QVector<QPointF> result;

    int l = 0;
    for (int i = 1; i < points.size(); i++)
        if (points.at(i).x() < points.at(l).x())
            l = i;
    int p = l, q;
    do {
        result.push_back(points.at(p));
        q = (p + 1) % points.size();
        for (int i = 0; i < points.size(); i++)
            if (orientation(points.at(p), points.at(i), points.at(q)) == 2)
                q = i;
        p = q;
    } while (p != l);
    return result;
}
//----------------------------------------------------------------
#endif // ASSIST_H
