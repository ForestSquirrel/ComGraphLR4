#ifndef EDITPOINTDIALOG_H
#define EDITPOINTDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QLineEdit;
QT_END_NAMESPACE

//! [0]
class EditPointDialog : public QDialog
{
    Q_OBJECT

public:
    EditPointDialog(QWidget *parent = nullptr);

    float x() const;
    float y() const;
    float z() const;
    void setFieldText(const float &x, const float &y, const float &z);

private:
    QLineEdit *xText;
    QLineEdit *yText;
    QLineEdit *zText;
};

#endif // EDITPOINTDIALOG_H
