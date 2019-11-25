#include "ScaleDialog.h"
#include "ui_ScaleDialog.h"

ScaleDialog::ScaleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScaleDialog)
{
    ui->setupUi(this);
}

ScaleDialog::~ScaleDialog()
{
    delete ui;
}

void ScaleDialog::on_buttonBox_accepted()
{
    if(ui->ID->text().isEmpty() || ui->cx->text().isEmpty() || ui->cy->text().isEmpty() || ui->scale->text().isEmpty()) return;
    int id = ui->ID->text().toInt();
    int cx = ui->cx->text().toInt();
    int cy = ui->cy->text().toInt();
    float scale = ui->scale->text().toFloat();
    emit scaleEvent(id, cx, cy, scale);
    this->close();
}

void ScaleDialog::on_buttonBox_rejected()
{
    this->close();
}
