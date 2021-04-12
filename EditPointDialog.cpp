#include "EditPointDialog.h"

#include <QtWidgets>

//! [0]
EditPointDialog::EditPointDialog(QWidget *parent)
    : QDialog(parent),
      xText(new QLineEdit),
      yText(new QLineEdit),
      zText(new QLineEdit)
{
    auto xLabel = new QLabel(tr("X"));
    auto yLabel = new QLabel(tr("Y"));
    auto zLabel = new QLabel(tr("Z"));
    auto okButton = new QPushButton(tr("OK"));
    auto cancelButton = new QPushButton(tr("Cancel"));

    auto gLayout = new QGridLayout;
    gLayout->setColumnStretch(1, 2);
    gLayout->addWidget(xLabel, 0, 0);
    gLayout->addWidget(xText, 0, 1);

    gLayout->addWidget(yLabel, 1, 0);
    gLayout->addWidget(yText, 1, 1);

    gLayout->addWidget(zLabel, 2, 0);
    gLayout->addWidget(zText, 2, 1);

    auto buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    gLayout->addLayout(buttonLayout, 3, 1, Qt::AlignRight);

    auto mainLayout = new QVBoxLayout;
    mainLayout->addLayout(gLayout);
    setLayout(mainLayout);
    connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);

    setWindowTitle(tr("Edit point"));
}
float EditPointDialog::x() const{
    return xText->text().toFloat();
}

float EditPointDialog::y() const{
    return yText->text().toFloat();
}

float EditPointDialog::z() const{
    return zText->text().toFloat();
}

void EditPointDialog::setFieldText(const float &x, const float &y, const float &z){
    xText->setText(QString::number(x));
    yText->setText(QString::number(y));
    zText->setText(QString::number(z));
}
