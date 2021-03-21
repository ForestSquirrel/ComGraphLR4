#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>
#include <QAction>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QVector>
#include <QDebug>
#include "IntersectionHandler.h"
#include "SplineHandler.h"
#include "QGraphicsControlPointItem.h"
class Scene : public QGraphicsScene
{
public:
    enum Mode {NoMode, SelectObject, DrawLine, DrawCircle, DrawDot, DrawSpline};
    Scene(QObject* parent = 0);
    void setMode(Mode mode);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void wheelEvent (QGraphicsSceneWheelEvent * event);
private:
    Mode sceneMode;
    BezierSpline spline;
    QList<line> Lines;
    QPointF origPoint;
    QGraphicsLineItem* itemToDraw;
    QGraphicsEllipseItem* circleToDraw;
    QGraphicsEllipseItem* dotToDraw;
    QGraphicsSimpleTextItem* textItem;
    QGraphicsRectItem *rectItem;
    QGraphicsControlPointItem* controlPoint;
    QString data;
    GraphicsArea area;
    void makeItemsControllable(bool areControllable);
};

#endif // SCENE_H
