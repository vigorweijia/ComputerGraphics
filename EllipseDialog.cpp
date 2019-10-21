#include "EllipseDialog.h"
#include "ui_EllipseDialog.h"

EllipseDialog::EllipseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EllipseDialog)
{
    ui->setupUi(this);
}

EllipseDialog::~EllipseDialog()
{
    delete ui;
}

void EllipseDialog::on_buttonBox_accepted()
{
    if(ui->ID->text().isEmpty() || ui->x->text().isEmpty() || ui->y->text().isEmpty() || ui->rx->text().isEmpty() || ui->ry->text().isEmpty()) return;
    int ID = ui->ID->text().toInt();
    int x = ui->x->text().toInt();
    int y = ui->y->text().toInt();
    int rx = ui->rx->text().toInt();
    int ry = ui->ry->text().toInt();
    emit drawEllipseEvent(ID, x, y, rx, ry);
    this->close();
}

void EllipseDialog::on_buttonBox_rejected()
{
    this->close();
}
