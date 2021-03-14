#pragma once
#ifndef INTERSECTIONHANDLER_H
#define INTERSECTIONHANDLER_H
#include <QGraphicsLineItem>
#include <QVector>
#include <QPair>
#include <QtMath>
#include <QDebug>
#include "assist.h"
#include "line.h"
class GraphicsArea : public QList<QPointF>{
public:
    GraphicsArea(): QList<QPointF>() {}

    bool isConvex(){
        if (this->size() >= 3){
            for (int a = this->size() - 2, b = this->size() - 1, c = 0; c < this->size(); a = b, b = c, ++c) {
                if (!line(this->at(a), this->at(b)).OnLeft(this->at(b))){
                    return false;
                }
            }
        }
        return true;
    }

    QList<line> Edges() {
        QList<line> temp;
        if (this->size() >= 2) {
            for (int a = this->size() - 1, b = 0; b < this->size(); a = b, ++b){
                temp.push_back(line(this->at(a), this->at(b)));
            }
        }
        return temp;
    }

    bool CyrusBeckClip(line subject){
        QPointF subjDir = subject.getDirection();
        float tA = 0.0f;
        float tB = 1.0f;
        float t;
        foreach (auto edge, Edges()){
            if ( Dot(edge.getNormal(), subjDir) < 0 ){
                t = subject.IntersectionParameter(edge);
                if (t > tA){
                    tA = t;
                }
            }
            else if (Dot(edge.getNormal(), subjDir) > 0){
                t = subject.IntersectionParameter(edge);
                if (t < tB){
                    tB = t;
                }
            }
            else {
                if (!edge.OnLeft(subject.p1())){
                    return false;
                }
            }
        }
        if (tA > tB){
            return false;
        }
        subject = subject.Morph(tA, tB);
        return true;
    }

    QList<line> CyrusBeckClip(QList<line> subjects){
        if (!this->isConvex()){
            qDebug() << "not convex";
        }

        QList<line> clippedSubjects;
        foreach (line subject, subjects){
            auto clippedSubject = subject;
            if (CyrusBeckClip(clippedSubject)){
                clippedSubjects.push_back(clippedSubject);
            }
        }
        return clippedSubjects;
    }

};



#endif // INTERSECTIONHANDLER_H
