#include "ClipDialog.h"
#include "ui_ClipDialog.h"

ClipDialog::ClipDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClipDialog)
{
    ui->setupUi(this);
}

ClipDialog::~ClipDialog()
{
    delete ui;
}

void ClipDialog::on_cohenSutherland_clicked()
{
    if(ui->ID->text().isEmpty() || ui->x1->text().isEmpty() || ui->x2->text().isEmpty() || ui->y1->text().isEmpty() || ui->y2->text().isEmpty()) return;
    int id = ui->ID->text().toInt();
    int x1 = ui->x1->text().toInt();
    int x2 = ui->x2->text().toInt();
    int y1 = ui->y1->text().toInt();
    int y2 = ui->y2->text().toInt();
    emit clipEvent(id, x1, y1, x2, y2, 0);
    this->close();
}

void ClipDialog::on_liangBarsky_clicked()
{
    if(ui->ID->text().isEmpty() || ui->x1->text().isEmpty() || ui->x2->text().isEmpty() || ui->y1->text().isEmpty() || ui->y2->text().isEmpty()) return;
    int id = ui->ID->text().toInt();
    int x1 = ui->x1->text().toInt();
    int x2 = ui->x2->text().toInt();
    int y1 = ui->y1->text().toInt();
    int y2 = ui->y2->text().toInt();
    emit clipEvent(id, x1, y1, x2, y2, 1);
    this->close();
}

void ClipDialog::on_cancel_clicked()
{
    this->close();
}
