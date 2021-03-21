#include "mainwindow.h"

MainWindow::MainWindow()
{
    scene = new Scene(this);
    scene->setSceneRect(0,0,200,200);
    view = new QGraphicsView(scene);
    view->setRenderHints(QPainter::Antialiasing);
    setCentralWidget(view);

    createActions();
    createConnections();
    createToolBar();
}

void MainWindow::createActions(){
    lineAction = new QAction("Draw line", this);
    lineAction->setData(int(Scene::DrawLine));
    lineAction->setIcon(QIcon(":/icons/line.png"));
    lineAction->setCheckable(true);

    selectAction = new QAction("Select object", this);
    selectAction->setData(int(Scene::SelectObject));
    selectAction->setIcon(QIcon(":/icons/select.png"));
    selectAction->setCheckable(true);

    circleAction = new QAction("Draw Circle", this);
    circleAction->setData(int(Scene::DrawCircle));
    circleAction->setIcon(QIcon(":/icons/circle.png"));
    circleAction->setCheckable(true);

    dotAction = new QAction("Draw Dot", this);
    dotAction->setData(int(Scene::DrawDot));
    dotAction->setIcon(QIcon(":/icons/dot.png"));
    dotAction->setCheckable(true);

    splineAction = new QAction("Draw Spline", this);
    splineAction->setData(int(Scene::DrawSpline));
    splineAction->setIcon(QIcon(":/icons/Spline.png"));
    splineAction->setCheckable(true);

    actionGroup = new QActionGroup(this);
    actionGroup->setExclusive(true);
    actionGroup->addAction(lineAction);
    actionGroup->addAction(selectAction);
    actionGroup->addAction(circleAction);;
    actionGroup->addAction(dotAction);
    actionGroup->addAction(splineAction);
}

void MainWindow::createConnections(){
    connect(actionGroup, SIGNAL(triggered(QAction*)),
            this, SLOT(actionGroupClicked(QAction*)));
}

void MainWindow::actionGroupClicked(QAction *action){
    scene->setMode(Scene::Mode(action->data().toInt()));
}

void MainWindow::createToolBar(){
    drawingToolBar = new QToolBar;
    addToolBar(Qt::TopToolBarArea, drawingToolBar);
    drawingToolBar->addAction(selectAction);
    drawingToolBar->addAction(lineAction);
    drawingToolBar->addAction(circleAction);
    drawingToolBar->addAction(dotAction);
    drawingToolBar->addAction(splineAction);
}
