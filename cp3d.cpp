#include "cp3d.h"

cp3D::cp3D(QGraphicsEllipseItem *parent) : QObject(), QGraphicsEllipseItem(parent) {
    QRectF temprect(0, 0, 15, 15);
    setRect(temprect);
    dz = 0;
    dw = 1;
    d = new EditPointDialog;
}
void cp3D::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    QRectF rect = boundingRect();
    painter->setPen(this->outlinePen);
    painter->drawEllipse(rect);

    QRectF dotRect(rect.x() + 7, rect.y() + 7,
                   2,            2);
    painter->setPen(pen);
    painter->drawEllipse(dotRect);

    Q_UNUSED(option);
    Q_UNUSED(widget);

}
void cp3D::setOutlinePen(QPen pen){
    outlinePen = pen;
}
int cp3D::type() const{
    return Type;
}
void cp3D::setdX(const float &x) {
    dx = x;
}
void cp3D::setdY(const float &y) {
    dy = y;
}
float cp3D::z()  const{
    return dz;
}
void cp3D::setdZ(const float &z) {
    dz = z;
}
float cp3D::w() const{
    return dw;
}
void cp3D::setdW(const float &w) {
    dw = w;
}
QVector4D cp3D::point(){
    return QVector4D(dx, dy, dz, dw);
}
void cp3D::editEntry(){
    d->setModal(true);
    d->setFieldText(dx, dy, dz);
    d->show();
    if (d->exec() == QDialog::Accepted){
      setdX(d->x());
      setdY(d->y());
      setdZ(d->z());
      setdW(1);
      setPos(rotAndProjToPoint(point(), angles.alpha, angles.beta, angles.gamma));
    } else
        return;

}
void cp3D::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e){
    editEntry();
    Q_UNUSED(e);
}
