#pragma once
#ifndef SPLINEHANDLER_H
#define SPLINEHANDLER_H
#include <QGraphicsItem>
#include <QVector>
#include <QGraphicsScene>
#include <QDebug>
#include "QGraphicsControlPointItem.h"
#include "GraphicsHandler.h"
class BezierSpline{
public:
    QVector<QGraphicsControlPointItem*> controlPoints;
    QVector<QGraphicsLineItem*> basicLines;
    QVector<QPointF> bezierSpline;
    QVector<QGraphicsLineItem*> bezierLines;
    float step = 0.05;
    BezierSpline(){
        clear();
    }
    ~BezierSpline(){
        clear();
    }

    void updateSpline(QGraphicsScene* scene, QGraphicsLineItem* itemToDraw){
        if (controlPoints.size() >= 2){
            foreach(QGraphicsItem* item, basicLines){
                scene->removeItem(item);
            }
            for (auto i = 0; i < controlPoints.size() - 1; i++){
                if(!itemToDraw){
                    itemToDraw = new QGraphicsLineItem;
                    scene->addItem(itemToDraw);
                    itemToDraw->setPen(QPen(Qt::green, 2, Qt::DashDotLine));
                }
                itemToDraw->setLine(QLineF(getCenter(controlPoints.at(i)->sceneBoundingRect()),
                                           getCenter(controlPoints.at(i + 1)->sceneBoundingRect())));
                basicLines.push_back(itemToDraw);
                itemToDraw = 0;
            }

            //----------------------------------------------------------------------------------------------------------------//
            QVector<QPointF> start = toPoints();
            bezierSpline.clear();
            for(float t = 0; t <= 1; t+=step){
                bezier(start, t);
                qDebug() << "Computed " << t << "out of 1";
            }
            if (bezierLines.size() >= 2){
                foreach(QGraphicsItem* item, bezierLines){
                    scene->removeItem(item);
                }
            }
            for (auto i = 0; i < bezierSpline.size() - 1; i++){
                if(!itemToDraw){
                    itemToDraw = new QGraphicsLineItem;
                    scene->addItem(itemToDraw);
                    itemToDraw->setPen(QPen(Qt::red, 2, Qt::SolidLine));
                }
                QLineF temp = QLineF(bezierSpline.at(i),
                                     bezierSpline.at(i+1));
                itemToDraw->setLine(temp);
                bezierLines.push_back(itemToDraw);
                itemToDraw = 0;
            }
        }
        else{
            qDebug() << "not enough control points";

        }
    }
    QVector<QPointF> toPoints(){
        QVector<QPointF> res;
        QPointF temp;
        foreach (QGraphicsControlPointItem* item, controlPoints){
            temp = getCenter(item->sceneBoundingRect());
            res.push_back(temp);
        }
        return res;
    }
    QPointF lerp (QPointF f, QPointF l, float t) {
      QPointF temp;
      temp.setX(  f.x() + t * ( l.x() -  f.x()) );
      temp.setY(  f.y() + t * ( l.y() -  f.y()) );
      return temp;
    }

    void bezier(QVector<QPointF> cp, float t) {
      QVector<QPointF> temp;
      for (auto i = 0; i < cp.size() - 1; i++){
          temp.push_back(lerp(cp.at(i), cp.at(i+1), t));
      }
      if (cp.size() == 2){
          bezierSpline.push_back(temp.at(0));
      } else{
          bezier(temp, t);
      }
    }

    void clear(){
        controlPoints.clear();
        basicLines.clear();
        bezierSpline.clear();
    }



};

#endif // SPLINEHANDLER_H
