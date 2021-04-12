#ifndef BEZIERSURFACE_H
#define BEZIERSURFACE_H

#include <QPainter>
#include <QGraphicsScene>
#include<QVector4D>
#include<QVector>
#include <QMatrix4x4>
#include <QtMath>
#include<QGraphicsItem>
#include <QDebug>
#include "SurfaceHandler.h"
#include "cp3d.h"
#include "GraphicsHandler.h"

class BezierSurface{
public:
    QVector<QVector<cp3D*>> cpVec;
    QVector<QGraphicsLineItem*> clVec;

    QVector<QVector4D> tempdotVec;
    QVector<QVector<QVector4D>> dotVec;
    QVector<QGraphicsLineItem*> gridVec;

    int sizeM;
    int sizeN;

    struct angles{
        float alpha;
        float beta;
        float gamma;
    };
    angles angles;

    BezierSurface(){
        clear();
    }
    ~BezierSurface(){
        clear();
    }
    BezierSurface(int m, int n){
        sizeM = m;
        sizeN = n;
        for (auto i = 0; i < m; i++){
            QVector<cp3D*> tempVec;
            for (auto j = 0; j < n; j++){
                cp3D *temp = new cp3D;
                temp->dx = 100*i;
                temp->dy = 100*j;
                temp->dz = 0;
                temp->dw = 1;
                tempVec.push_back(temp);
                temp = 0;
            }
            cpVec.push_back(tempVec);
            tempVec.clear();
        }
    }

    void clear(){
        for (auto i = 0; i < cpVec.size(); i++)
            cpVec[i].clear();
        cpVec.clear();
        clVec.clear();
        for (auto i = 0; i < dotVec.size(); i++)
            dotVec[i].clear();
        dotVec.clear();
        /*for (auto i = 0; i < tempdotVec.size(); i++)
            tempdotVec[i].clear();*/
        tempdotVec.clear();
        gridVec.clear();
    }
    QVector4D lerp3D (QVector4D f, QVector4D l, float t) {
      QVector4D temp;
      temp.setX(  f.x() + t * ( l.x() -  f.x()) );
      temp.setY(  f.y() + t * ( l.y() -  f.y()) );
      temp.setZ(  f.z() + t * ( l.z() -  f.z()) );
      temp.setW( 1 );
      return temp;
    }
    void makeCLines() {
        QGraphicsLineItem* temp = 0;
        clVec.clear();
        for (auto i = 0; i < sizeM; i++)
            for (auto j = 0; j < sizeN; j++){
                if(!temp && j != sizeN - 1){
                    temp = new QGraphicsLineItem;
                    temp->setPen(QPen(Qt::green, 1, Qt::DashDotLine));
                    temp->setLine(QLineF(getCenter(cpVec[i][j]->sceneBoundingRect()),
                                               getCenter(cpVec[i][j+1]->sceneBoundingRect())));
                    clVec.push_back(temp);
                }
                temp = 0;
                if(!temp && i != sizeM - 1){
                    temp = new QGraphicsLineItem;
                    temp->setPen(QPen(Qt::green, 1, Qt::DashDotLine));
                    temp->setLine(QLineF(getCenter(cpVec[i][j]->sceneBoundingRect()),
                                               getCenter(cpVec[i+1][j]->sceneBoundingRect())));
                    clVec.push_back(temp);
                }
                temp = 0;
            }
    }

    void makeSurface(){
        dotVec.clear();
        QVector<QVector4D> tempLine;
        QVector<QVector<QVector4D>> temp;
        //make support splines
        for (auto i = 0; i < sizeM; i++){
            tempLine.clear();
            tempLine = points().at(i);
            for (float t = 0; t <= 1; t+= 0.05){
                bezier(tempLine, t);
            }
            temp.push_back(tempdotVec);
            tempdotVec.clear();
        }
        //---------------------------------
        //make surface
        for (auto j = 0; j < temp[0].size(); j++){
            tempLine.clear();
            for (auto i = 0; i < sizeM; i++){
                tempLine.push_back(temp[i][j]);
            }
            for (float t = 0; t <= 1; t+= 0.05){
                bezier(tempLine, t);
            }
            dotVec.push_back(tempdotVec);
            tempdotVec.clear();
        }
        //-------------------------------------
        //draw meshgrid
        QGraphicsLineItem* tempL = 0;
        gridVec.clear();
        for (auto i = 0; i < dotVec.size(); i++)
            for (auto j = 0; j < dotVec[i].size(); j++){
                if(!tempL && j != dotVec[i].size() - 1){
                    tempL = new QGraphicsLineItem;
                    tempL->setPen(QPen(Qt::black, 1, Qt::SolidLine));
                    tempL->setLine(QLineF(
                                       rotAndProjToPoint(dotVec[i][j], angles.alpha, angles.beta, angles.gamma),
                                       rotAndProjToPoint(dotVec[i][j+1], angles.alpha, angles.beta, angles.gamma)
                                   ));
                    gridVec.push_back(tempL);
                }
                tempL = 0;
                if(!tempL && i != dotVec.size() - 1){
                    tempL = new QGraphicsLineItem;
                    tempL->setPen(QPen(Qt::black, 1, Qt::SolidLine));
                    tempL->setLine(QLineF(
                                       rotAndProjToPoint(dotVec[i][j], angles.alpha, angles.beta, angles.gamma),
                                       rotAndProjToPoint(dotVec[i+1][j], angles.alpha, angles.beta, angles.gamma)
                                   ));
                    gridVec.push_back(tempL);
                }
                tempL = 0;
            }
        //-------------------------------------
    }
    void bezier(QVector<QVector4D> cp, float t) {
      QVector<QVector4D> temp;
      for (auto i = 0; i < cp.size() - 1; i++){
          temp.push_back(lerp3D(cp.at(i), cp.at(i+1), t));
      }
      if (cp.size() == 2){
          tempdotVec.push_back(temp.at(0));
      } else{
          bezier(temp, t);
      }
    }
    QVector<QVector<QVector4D>> points(){
        QVector<QVector<QVector4D>> res;
        for (auto i = 0; i < sizeM; i++){
            QVector<QVector4D> tempVec;
            for (auto j = 0; j < sizeN; j++){
                QVector4D temp = cpVec[i][j]->point();
                tempVec.push_back(temp);
            }
            res.push_back(tempVec);
            tempVec.clear();
        }
        return res;
    }

};

#endif // BEZIERSURFACE_H
