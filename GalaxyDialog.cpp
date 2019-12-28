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
