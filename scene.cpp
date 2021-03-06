#include "scene.h"
#include "GraphicsHandler.h"

Scene::Scene(QObject* parent): QGraphicsScene(parent)
{
    sceneMode = NoMode;
    gd = new MeshDialog;
    itemToDraw = 0;
    circleToDraw = 0;
    rectItem = 0;
    dotToDraw = 0;
    controlPoint = 0;
    cp3d = 0;

    xAxis = new QGraphicsLineItem;
    this->addItem(xAxis);
    xAxis->setPen(QPen(Qt::green, 1, Qt::SolidLine));

    yAxis = new QGraphicsLineItem;
    this->addItem(yAxis);
    yAxis->setPen(QPen(Qt::red, 2, Qt::SolidLine));

    zAxis = new QGraphicsLineItem;
    this->addItem(zAxis);
    zAxis->setPen(QPen(Qt::blue, 3, Qt::SolidLine));

    angles.alpha = 0;
    angles.beta = 0;
    angles.gamma = 0;
    drawAxis(xAxis, yAxis, zAxis, angles.alpha, angles.beta, angles.gamma);
}

void Scene::setMode(Mode mode){
    sceneMode = mode;
    QGraphicsView::DragMode vMode =
            QGraphicsView::NoDrag;
    if(mode == DrawLine){
        makeItemsControllable(false);
        vMode = QGraphicsView::NoDrag;
    }
    else if(mode == SelectObject){
        makeItemsControllable(true);
        vMode = QGraphicsView::RubberBandDrag;
    }
    else if(mode == DrawCircle){
        makeItemsControllable(false);
        vMode = QGraphicsView::NoDrag;
    }
    else if (mode == DrawDot){
        makeItemsControllable(false);
        vMode = QGraphicsView::NoDrag;
    }
    else if (mode == DrawSpline){
        makeItemsControllable(false);
        vMode = QGraphicsView::NoDrag;
    }
    else if (mode == DrawSurface){
        makeItemsControllable(false);
        vMode = QGraphicsView::NoDrag;
    }
    QGraphicsView* mView = views().at(0);
    if(mView)
        mView->setDragMode(vMode);
}

void Scene::makeItemsControllable(bool areControllable){
    foreach(QGraphicsItem* item, items()){
        item->setFlag(QGraphicsItem::ItemIsSelectable,
                      areControllable);
        item->setFlag(QGraphicsItem::ItemIsMovable,
                      areControllable);
        item->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    }
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event){
    if(sceneMode == DrawLine)
        origPoint = event->scenePos();
    else if(sceneMode == DrawCircle)
        origPoint = event->scenePos();
    else if (sceneMode == DrawDot)
        origPoint = event->scenePos();
    else if (sceneMode == DrawSpline)
        origPoint = event->scenePos();
    /*else if (sceneMode == DrawSurface)
        origPoint = event->scenePos();*/ //deprecated
    QGraphicsScene::mousePressEvent(event);
}
void Scene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e){
    QGraphicsScene::mouseDoubleClickEvent(e);
    updateSurface();
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    if(sceneMode == DrawLine){
        if(!itemToDraw){
            itemToDraw = new QGraphicsLineItem;
            this->addItem(itemToDraw);
            itemToDraw->setPen(QPen(Qt::black, 3, Qt::SolidLine));
            itemToDraw->setPos(origPoint);
        }
        itemToDraw->setLine(0,0,
                            event->scenePos().x() - origPoint.x(),
                            event->scenePos().y() - origPoint.y()
                            );
    }
    else if(sceneMode == DrawCircle){
        if(!circleToDraw){
            circleToDraw = new QGraphicsEllipseItem;
            //show rect
            if (!rectItem){
            rectItem = new QGraphicsRectItem;
            this->addItem(rectItem);
            rectItem->setPen(QPen(Qt::gray, 2, Qt::SolidLine));
            rectItem->setPos(origPoint);
            }
            //show rect _end
            this->addItem(circleToDraw);
            circleToDraw->setPen(QPen(Qt::black, 2, Qt::SolidLine));
            circleToDraw->setPos(origPoint);
        }
        circleToDraw->setRect(0,0,
                              event->scenePos().x() - origPoint.x(),
                              event->scenePos().x() - origPoint.x()
                              );
        rectItem->setRect(0,0,
                           event->scenePos().x() - origPoint.x(),
                           event->scenePos().x() - origPoint.x()
                           );
        data.clear();
    }
    /*else if (sceneMode == DrawSurface){
        if (!circleToDraw){
            circleToDraw =  new QGraphicsEllipseItem;
            this->addItem(circleToDraw);
            circleToDraw->setPen(QPen(Qt::red, 2, Qt::SolidLine));
            circleToDraw->setRect(0,0,
                                  15,15);
        }
        circleToDraw->setPos(rotAndProjToPoint(QVector4D(event->scenePos().x(), event->scenePos().y(), 0, 1), angles.alpha, angles.beta, angles.gamma));
    }*/ //deprecated
    else
        QGraphicsScene::mouseMoveEvent(event);
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if (sceneMode == DrawLine){
        Lines.push_back(line(origPoint.x(), origPoint.y(),
                              event->scenePos().x(),  event->scenePos().y(),
                        itemToDraw));
        itemToDraw = 0;
        textItem = 0;

    }
    else if (sceneMode == DrawCircle){
        circleToDraw = 0;
        textItem = 0;
        this->removeItem(rectItem);
        rectItem = 0;
    }
    else if (sceneMode == DrawDot){
        if (!dotToDraw){
            dotToDraw = new QGraphicsEllipseItem;
            this->addItem(dotToDraw);
            dotToDraw->setPen(QPen(Qt::black, 2, Qt::SolidLine));
            dotToDraw->setPos(origPoint);
        }
        dotToDraw->setRect(0,0,
                           2,2);
        dotToDraw = 0;
    }
    else if (sceneMode == DrawSpline){
        if (!controlPoint){
            controlPoint = new QGraphicsControlPointItem;
            this->addItem(controlPoint);
            controlPoint->setPos(origPoint);
        }
        spline.controlPoints.push_back(controlPoint);
        controlPoint = 0;
        if (spline.controlPoints.size() > 1){
            spline.updateSpline(this, itemToDraw);
        }
    }
    /*else if (sceneMode == DrawSurface){
        this->removeItem(circleToDraw);
        circleToDraw = 0;
        cp3d = 0;
        if (!cp3d){
            cp3d = new cp3D;
            this->addItem(cp3d);
            cp3d->angles.alpha = angles.alpha;
            cp3d->angles.beta = angles.beta;
            cp3d->angles.gamma = angles.gamma;
            cp3d->dx = event->scenePos().x();
            cp3d->dy = event->scenePos().y();
            cp3d->setPos(rotAndProjToPoint(cp3d->point(),angles.alpha, angles.beta, angles.gamma));
        }
        cp3dvec.push_back(cp3d);
        cp3d = 0;
    }*/ //deprecated
    else if (sceneMode == SelectObject){
        bool updatable = true;
        if (selectedItems().size() >= 1){
            foreach(QGraphicsItem* item, selectedItems()){
                if (item->type() != QAbstractGraphicsShapeItem::UserType + 1){
                    updatable = false;
                    break;
                }
            }
        }
        else {
            updatable = false;
        }
        if (updatable){
            spline.updateSpline(this, itemToDraw);
        }
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

void Scene::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Delete)
        foreach(QGraphicsItem* item, selectedItems()){
            if (item->type() == QAbstractGraphicsShapeItem::UserType + 1){
                foreach (QGraphicsControlPointItem *item1, spline.controlPoints){
                    if (item1->sceneBoundingRect() == item->sceneBoundingRect()){
                        spline.controlPoints.removeOne(item1);
                    }
                }
                spline.updateSpline(this, itemToDraw);
            }
            removeItem(item);
            delete item;
        }
    else if (event->key() == Qt::Key_2){
        QVector<QPointF> dots;
        area.clear();
        if (selectedItems().size() >= 2){
             for (auto i = 0 ; i < selectedItems().size(); i++){                           //create vector of dots
                 dots.push_back(getCenter( selectedItems().at(i)->sceneBoundingRect()));
             }
             dots = giftWrapping(dots);
             for (auto j = 0 ; j < dots.size() - 1; j++){
                 if(!itemToDraw){
                     itemToDraw = new QGraphicsLineItem;
                     this->addItem(itemToDraw);
                     itemToDraw->setPen(QPen(Qt::green, 2, Qt::SolidLine));
                     itemToDraw->setLine(dots.at(j).x(),   dots.at(j).y(),
                                         dots.at(j+1).x(), dots.at(j+1).y());
                     area.push_back(dots.at(j));
                 }
                 itemToDraw = 0;
             }
             if(!itemToDraw){
                 itemToDraw = new QGraphicsLineItem;
                 this->addItem(itemToDraw);
                 itemToDraw->setPen(QPen(Qt::green, 2, Qt::SolidLine));
                 itemToDraw->setLine(dots.at(dots.size() - 1).x(),   dots.at(dots.size() - 1).y(),
                                     dots.at(0).x(),                 dots.at(0).y());
                 area.push_back(dots.at(dots.size() - 1));
             }
             itemToDraw = 0;

             //----------------match intersections----------------------------
             QList<line> collided = area.CyrusBeckClip(Lines);
             if (collided.isEmpty()){
                 qDebug() << "empty area";
             }
             else
                 foreach (class::line Line , collided){
                     if(!itemToDraw){
                         itemToDraw = new QGraphicsLineItem;
                         this->addItem(itemToDraw);
                         itemToDraw->setPen(QPen(Qt::red, 2, Qt::SolidLine));
                         this->removeItem(Line.graphicsPtr);
                     }
                     itemToDraw->setLine(Line);

                     itemToDraw = 0;
                 }
             //---------------------------------------------------------------
        }
        else
            qDebug() << "not enough points";
    }
     else if (event->key() == Qt::Key_3){
        spline.updateSpline(this, itemToDraw);
    }
    else if (event->key() == Qt::Key_Right){
        float tempA = qRadiansToDegrees(angles.alpha);
        tempA+=10;
        angles.alpha = qDegreesToRadians(tempA);
        drawAxis(xAxis, yAxis, zAxis, angles.alpha, angles.beta, angles.gamma);
        updateSurface();
    }
    else if (event->key() == Qt::Key_Down){
        float tempB = qRadiansToDegrees(angles.beta);
        tempB+=10;
        angles.beta = qDegreesToRadians(tempB);
        drawAxis(xAxis, yAxis, zAxis, angles.alpha, angles.beta, angles.gamma);
        updateSurface();
    }
    /*else if (event->key() == Qt::Key_Up){
        float tempG = qRadiansToDegrees(angles.gamma);
        tempG++;
        angles.gamma = qDegreesToRadians(tempG);
        drawAxis(xAxis, yAxis, zAxis, angles.alpha, angles.beta, angles.gamma);
        updateSurface();
    }*/ //deprecated
    else
        QGraphicsScene::keyPressEvent(event);
}
void Scene::keyReleaseEvent(QKeyEvent *event){
    QPointF point, center;
    QPair<QPointF, QPointF> tang;
    qreal r;
     if (event->key() == Qt::Key_1){
         if (selectedItems().size() == 2){
            if (selectedItems().at(0)->sceneBoundingRect().width() < selectedItems().at(1)->sceneBoundingRect().width()){
                center = getCenter( selectedItems().at(1)->sceneBoundingRect());
                point = getCenter( selectedItems().at(0)->sceneBoundingRect());
                r = getRad( selectedItems().at(1)->sceneBoundingRect() );
                tang = GetTangentPointsAtCircle(center, point, r);
            }
            else{
                center = getCenter( selectedItems().at(0)->sceneBoundingRect());
                point = getCenter( selectedItems().at(1)->sceneBoundingRect());
                r = getRad( selectedItems().at(0)->sceneBoundingRect() );
                tang = GetTangentPointsAtCircle(center, point, r);
            }

            if(!itemToDraw){
                itemToDraw = new QGraphicsLineItem;
                this->addItem(itemToDraw);
                itemToDraw->setPen(QPen(Qt::black, 3, Qt::SolidLine));
                itemToDraw->setLine(point.x(),point.y(),
                                    tang.first.x(), tang.first.y()
                                    );
            }
            itemToDraw = 0;
            if(!itemToDraw){
                itemToDraw = new QGraphicsLineItem;
                this->addItem(itemToDraw);
                itemToDraw->setPen(QPen(Qt::black, 3, Qt::SolidLine));
                itemToDraw->setLine(point.x(),point.y(),
                                    tang.second.x(), tang.second.y()
                                    );
            }
            itemToDraw = 0;
         }
     }
     else if (event->key() == Qt::Key_G && sceneMode == DrawSurface){
         gd->setModal(true);
         gd->show();
         if (gd->exec() == QDialog::Accepted){
             surface = BezierSurface(gd->first(), gd->second());
         }
         this->cp3dvec.clear();
         for (auto i = 0; i < surface.sizeM; i++)
             for (auto j = 0; j < surface.sizeN; j++){
                 this->addItem(surface.cpVec[i][j]);
                 this->cp3dvec.push_back(surface.cpVec[i][j]);
             }
         updateSurface();
     }
     else if (event->key() == Qt::Key_C){
         foreach(QGraphicsPolygonItem* p, surface.poly){
             this->removeItem(p);
         }
         surface.handleDirections();
         foreach(QGraphicsPolygonItem* p, surface.poly){
             this->addItem(p);
         }
     }
     else if (event->key() == Qt::Key_D){
         foreach(QGraphicsPolygonItem* p, surface.poly){
             this->removeItem(p);
         }
     }
     else
         QGraphicsScene::keyReleaseEvent(event);
}

void Scene::wheelEvent(QGraphicsSceneWheelEvent *event){
       qreal degrees;
       qreal scaleFactor;
       if (selectedItems().size() == 1){
           scaleFactor = selectedItems().at(0)->scale();
           degrees = event->delta() / 120;
           scaleFactor += degrees / 10;
           qDebug() << event->delta() << degrees << scaleFactor;
           selectedItems().at(0)->setScale(scaleFactor);
       }
       QGraphicsScene::wheelEvent(event);
}

void Scene::updateSurface(){
    foreach (cp3D *item, this->cp3dvec){
        updateItem(item);
    }
    updateCLines();
    foreach (QGraphicsLineItem *item, this->surface.gridVec){
        this->removeItem(item);
    }
    surface.angles.alpha = this->angles.alpha;
    surface.angles.beta = this->angles.beta;
    surface.angles.gamma=this->angles.gamma;
    surface.makeSurface();
    foreach (QGraphicsLineItem *item, this->surface.gridVec){
        this->addItem(item);
    }

}
void Scene::updateItem(cp3D *item){
    item->setPos(rotAndProjToPoint(item->point(), angles.alpha, angles.beta, angles.gamma));
}
void Scene::updateCLines(){
    foreach (QGraphicsLineItem *item, this->surface.clVec){
        this->removeItem(item);
    }
    surface.makeCLines();
    foreach (QGraphicsLineItem *item, this->surface.clVec){
        this->addItem(item);
    }
}
