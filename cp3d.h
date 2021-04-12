#ifndef CP3D_H
#define CP3D_H
#include <QPainter>
#include <QGraphicsItem>
#include <QVector4D>
#include <QGraphicsScene>
#include "EditPointDialog.h"
#include "SurfaceHandler.h"
class cp3D :public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    enum { Type = UserType + 2 };
    int type() const override;
    explicit cp3D(QGraphicsEllipseItem *parent = nullptr);
    QPen outlinePen = QPen(Qt::gray, 2, Qt::DashLine);
    QPen pen = QPen(Qt::black, 2, Qt::SolidLine);
    void paint(QPainter * painter,
               const QStyleOptionGraphicsItem * option,
               QWidget * widget) override;
    void setOutlinePen(QPen pen);
    float dx;
    float dy;
    float dz;
    float dw;
    struct angles{
        float alpha;
        float beta;
        float gamma;
    };
    angles angles;
    EditPointDialog* d;
    float z() const;
    float w() const;
    void setdX(const float &x);
    void setdY(const float &y);
    void setdZ(const float &z);
    void setdW(const float &w);
    QVector4D point();
protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e) override;
    void editEntry();
};

#endif // CP3D_H
