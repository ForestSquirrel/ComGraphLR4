#ifndef MESHWIDGET_H
#define MESHWIDGET_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QSpinBox;
QT_END_NAMESPACE

//! [0]
class MeshDialog : public QDialog
{
    Q_OBJECT

public:
    int first() const;
    int second() const;
    MeshDialog(QWidget *parent = nullptr);
private:
    QSpinBox *firstBox;
    QSpinBox *secondBox;
};

#endif // MESHWIDGET_H
