#include "scene.h"
#include "GraphicsHandler.h"
Scene::Scene(QObject* parent): QGraphicsScene(parent)
{
    sceneMode = NoMode;
    circleToDraw = 0;
    rectItem = 0;
    dotToDraw = 0;
    //init axes
    QVector<QLineF> axis = {
        {0,0,200,0},
        {0,0,0,200},
        {200,0,180, 10},
        {200,0,180,-10},
        {0,200,10,180},
        {0,200,-10,180}
    };
    for (auto i = 0; i < axis.length(); i++) {
        itemToDraw = new QGraphicsLineItem;
        this->addItem(itemToDraw);
        itemToDraw->setLine(axis.at(i));
        itemToDraw = 0;
    }
    textItem = new QGraphicsSimpleTextItem;
    this->addItem(textItem);
    textItem->setPos(5,0);
    textItem->setText("0");
    textItem = 0;
    textItem = new QGraphicsSimpleTextItem;
    this->addItem(textItem);
    textItem->setPos(0,200);
    textItem->setText("x");
    textItem=0;
    textItem = new QGraphicsSimpleTextItem;
    this->addItem(textItem);
    textItem->setPos(200,0);
    textItem->setText("y");
    textItem = 0;
    //init axes _end

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
    }
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event){
    if(sceneMode == DrawLine)
        origPoint = event->scenePos();
    else if(sceneMode == DrawCircle)
        origPoint = event->scenePos();
    else if (sceneMode == DrawDot)
        origPoint = event->scenePos();
    QGraphicsScene::mousePressEvent(event);
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
            rectItem->setPen(QPen(Qt::gray, 2, Qt::DashLine));
            rectItem->setPos(origPoint);
            }
            //show rect _end
            this->addItem(circleToDraw);
            circleToDraw->setPen(QPen(Qt::black, 3, Qt::SolidLine));
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
    QGraphicsScene::mouseReleaseEvent(event);
}

void Scene::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Delete)
        foreach(QGraphicsItem* item, selectedItems()){
            removeItem(item);
            delete item;
        }
    else if (event->key() == Qt::Key_2){ //ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
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
             QList<line> collided = area.CyrusBeckClip(Lines); //находим линии отсекаемые областью
             if (collided.isEmpty()){
                 qDebug() << "empty area";
             }
             else
                 foreach (class::line Line , collided){
                     if(!itemToDraw){
                         itemToDraw = new QGraphicsLineItem;
                         this->addItem(itemToDraw);
                         itemToDraw->setPen(QPen(Qt::red, 2, Qt::SolidLine));
                         //itemToDraw->setPos(Line.graphicsPtr->pos());
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
         else
            QGraphicsScene::keyReleaseEvent(event);
     }
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
