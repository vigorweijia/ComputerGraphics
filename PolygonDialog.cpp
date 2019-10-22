#include "PolygonDialog.h"
#include "ui_PolygonDialog.h"

PolygonDialog::PolygonDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PolygonDialog)
{
    ui->setupUi(this);
}

PolygonDialog::~PolygonDialog()
{
    delete ui;
}
