#include "GalaxyDialog.h"
#include "ui_GalaxyDialog.h"

GalaxyDialog::GalaxyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GalaxyDialog)
{
    ui->setupUi(this);
}

GalaxyDialog::~GalaxyDialog()
{
    delete ui;
}

void GalaxyDialog::nomarlizedAngle(int *angle)
{

}

void GalaxyDialog::setXRotation(int angle)
{

}

void GalaxyDialog::setYRotation(int angle)
{

}

void GalaxyDialog::setZRotation(int angle)
{

}

void GalaxyDialog::mousePressEvent(QMouseEvent *e)
{

}

void GalaxyDialog::mouseMoveEvent(QMouseEvent *e)
{

}
