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
#include "SurfaceHandler.h"
#include "cp3d.h"
#include "BezierSurface.h"
#include "MeshWidget.h"
class Scene : public QGraphicsScene
{
public:
    enum Mode {NoMode, SelectObject, DrawLine, DrawCircle, DrawDot, DrawSpline, DrawSurface};
    Scene(QObject* parent = 0);
    void setMode(Mode mode);
    void updateSurface();
    void updateCLines();
    void updateItem(cp3D* item);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void wheelEvent (QGraphicsSceneWheelEvent * event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e) override;
private:
    struct angles{
        float alpha;
        float beta;
        float gamma;
    };
    MeshDialog *gd;
    BezierSurface surface;
    angles angles;
    Mode sceneMode;
    BezierSpline spline;
    QList<line> Lines;
    QVector<cp3D*> cp3dvec;
    QPointF origPoint;
    QGraphicsLineItem* xAxis;
    QGraphicsLineItem* yAxis;
    QGraphicsLineItem* zAxis;
    QGraphicsLineItem* itemToDraw;
    QGraphicsEllipseItem* circleToDraw;
    QGraphicsEllipseItem* dotToDraw;
    QGraphicsSimpleTextItem* textItem;
    QGraphicsRectItem *rectItem;
    QGraphicsControlPointItem* controlPoint;
    cp3D* cp3d;
    QString data;
    GraphicsArea area;
    void makeItemsControllable(bool areControllable);
};

#endif // SCENE_H
