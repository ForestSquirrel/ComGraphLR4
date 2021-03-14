#pragma once
#ifndef GRAPHICSHANDLER_H
#define GRAPHICSHANDLER_H
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>
#include <QAction>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QVector>
#include <QPair>
QPointF getCenter(QRectF rect){
    QPointF end = {0,0};
    end.setX( rect.x() + rect.width()/2 );
    end.setY( rect.y() + rect.height()/2 );
    return end;
}

qreal getRad(QRectF rect){
    return rect.x() + rect.width() - getCenter(rect).x();
}

QPair<QPointF, QPointF> GetTangentPointsAtCircle(QPointF center, QPointF dot, qreal R){
      QPair<QPointF, QPointF> rst;
      qreal YT0, YT1, nx, ny, xy, tx0, tx1, D;
      nx = (dot.x() - center.x()) / R; //shift and scale
      ny = (dot.y() - center.y()) / R;
      xy = nx * nx + ny * ny;

      //common case, two tangents
      D = ny * sqrt(xy - 1);
      tx0 = (nx - D) / xy;
      tx1 = (nx + D) / xy;
      if (ny != 0){
        YT0 = center.y() + R * (1 - tx0 * nx) / ny;
        YT1 = center.y() + R * (1 - tx1 * nx) / ny;
      }
      else { //point at the center horizontal, Y=0
        D = R * sqrt(1 - tx0 * tx0);
        YT0 = center.y() + D;
        YT1 = center.y() - D;
      }
      auto XT0 = center.x() + R * tx0; //restore scale and position
      auto XT1 = center.x() + R * tx1;
    rst.first.setX(XT0);
    rst.second.setX(XT1);
    rst.first.setY(YT0);
    rst.second.setY(YT1);
    return rst;
}
#endif // GRAPHICSHANDLER_H
