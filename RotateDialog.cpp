#include "RotateDialog.h"
#include "ui_RotateDialog.h"

RotateDialog::RotateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RotateDialog)
{
    ui->setupUi(this);
}

RotateDialog::~RotateDialog()
{
    delete ui;
}

void RotateDialog::on_buttonBox_accepted()
{
    if(ui->ID->text().isEmpty() || ui->cx->text().isEmpty() || ui->cy->text().isEmpty() || ui->angle->text().isEmpty()) return;
    int id = ui->ID->text().toInt();
    int cx = ui->cx->text().toInt();
    int cy = ui->cy->text().toInt();
    int angle = ui->angle->text().toInt();
    emit rotateEvent(id, cx, cy, angle);
    this->close();
}

void RotateDialog::on_buttonBox_rejected()
{
    this->close();
}
