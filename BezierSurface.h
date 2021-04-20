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
    QVector<QVector<QVector4D>> dotnormVec;
    QVector<QGraphicsLineItem*> gridVec;

    int sizeM;
    int sizeN;

    struct angles{
        float alpha;
        float beta;
        float gamma;
    } angles;

    QVector<QGraphicsPolygonItem*> poly;

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
        //draw grid
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
    bool handleRotation(){
        QVector4D n = QVector4D(0, 0, 0, 1);
        QVector4D n1 = QVector4D(0, 0, 1, 1);
        if (rot(n1, angles.alpha, angles.beta, angles.gamma).z() > rot(n, angles.alpha, angles.beta, angles.gamma).z())
            return true;
        else
            return false;
    }
    void handleDirections(){
        QGraphicsPolygonItem* tempL = 0;
        QPolygonF tempP;
        bool siderot = handleRotation();
        bool sidevecX = true;
        bool sidevecY = true;
        bool side = true;
        QVector<QPointF> tempV;
        poly.clear();
        for (auto i = 0; i < dotVec.size(); i++)
            for (auto j = 0; j < dotVec[i].size(); j++){
                if(!tempL && j != dotVec[i].size() - 1 && i != dotVec.size() - 1){
                        qDebug() << i <<' '<< j;
                    if ( rot(dotVec[i][j], angles.alpha, angles.beta, angles.gamma).x() <= rot(dotVec[i][j+1], angles.alpha, angles.beta, angles.gamma).x() &&
                         rot(dotVec[i+1][j], angles.alpha, angles.beta, angles.gamma).x() <= rot(dotVec[i+1][j+1], angles.alpha, angles.beta, angles.gamma).x()){
                        sidevecX = true;
                    } else{
                        sidevecX = false;
                    }
                    if (rot(dotVec[i][j], angles.alpha, angles.beta, angles.gamma).y() <= rot(dotVec[i+1][j], angles.alpha, angles.beta, angles.gamma).y() &&
                            rot(dotVec[i][j+1], angles.alpha, angles.beta, angles.gamma).y() <= rot(dotVec[i+1][j+1], angles.alpha, angles.beta, angles.gamma).y()){
                        sidevecY = true;
                    } else{
                        sidevecY = false;
                    }
                    if (sidevecX == true && sidevecY == true && siderot == true)
                        side = true;
                    else if (sidevecX == false && sidevecY == false && siderot == false)
                        side = true;
                    //---------------------------------------------------------
                    else if (sidevecX == false && sidevecY == false && siderot == true)
                        side = false;
                    else if (sidevecX == true && sidevecY == true && siderot == false)
                        side = false;
                    //----------------------------------------------------------
                    else if (sidevecX == true && sidevecY == false && siderot == false)
                        side = false;
                    else if (sidevecX == true && sidevecY == false && siderot == true)
                        side = true;
                    //------------------------------------------------------------
                    else if (sidevecX == false && sidevecY == true && siderot == true)
                        side = false;
                    else if (sidevecX == false && sidevecY == true && siderot == false)
                        side = true;
                    qDebug() << dotVec[i][j] << '|' << dotVec[i][j+1] << '\n' << dotVec[i+1][j] << '|' << dotVec[i+1][j+1] << '\n' << sidevecX << sidevecY <<siderot <<'|'<< side<< '\n' <<"------------------" << '\n';
                    tempV.push_back(rotAndProjToPoint(dotVec[i][j], angles.alpha, angles.beta, angles.gamma));
                    tempV.push_back(rotAndProjToPoint(dotVec[i][j+1], angles.alpha, angles.beta, angles.gamma));
                    tempV.push_back(rotAndProjToPoint(dotVec[i+1][j+1], angles.alpha, angles.beta, angles.gamma));
                    tempV.push_back(rotAndProjToPoint(dotVec[i+1][j], angles.alpha, angles.beta, angles.gamma));
                    tempV.push_back(rotAndProjToPoint(dotVec[i][j], angles.alpha, angles.beta, angles.gamma));
                    tempP = QPolygonF(tempV);
                    tempL = new QGraphicsPolygonItem;
                    tempL->setPolygon(tempP);
                    if (side){
                        tempL->setBrush(QBrush(QColor::fromRgbF(0, 128, 128, 0.33)));
                    } else{
                        tempL->setBrush(QBrush(QColor::fromRgbF(128, 0, 0, 0.33)));
                    }
                    poly.push_back(tempL);
                    tempL = 0;
                    tempV.clear();
                }
            }
    }
};

#endif // BEZIERSURFACE_H
