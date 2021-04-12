#include "MeshWidget.h"

#include <QtWidgets>

//! [0]
MeshDialog::MeshDialog(QWidget *parent)
    : QDialog(parent),
      firstBox(new QSpinBox),
      secondBox(new QSpinBox)
{
    auto xLabel = new QLabel(tr("x"));
    auto okButton = new QPushButton(tr("OK"));
    auto cancelButton = new QPushButton(tr("Cancel"));
    firstBox->setRange(3, 5);
    secondBox->setRange(3, 5);
    firstBox->setSingleStep(1);
    secondBox->setSingleStep(1);
    firstBox->setFixedWidth(100);
    secondBox->setFixedWidth(100);
    auto gLayout = new QGridLayout;
    gLayout->addWidget(xLabel, 0, 1, Qt::AlignCenter);

    gLayout->addWidget(firstBox, 0, 0, Qt::AlignRight);
    gLayout->addWidget(secondBox, 0, 2, Qt::AlignLeft);

    auto buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    gLayout->addLayout(buttonLayout, 2, 1, Qt::AlignHCenter);

    auto mainLayout = new QVBoxLayout;
    mainLayout->addLayout(gLayout);
    setLayout(mainLayout);
    connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);

    setWindowTitle(tr("Grid Editor"));
}
int MeshDialog::first() const{
    return firstBox->value();
}

int MeshDialog::second() const{
    return secondBox->value();
}
