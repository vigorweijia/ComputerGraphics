#include "LineDialog.h"
#include "ui_LineDialog.h"

LineDialog::LineDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LineDialog)
{
    ui->setupUi(this);
}

LineDialog::~LineDialog()
{
    delete ui;
}

void LineDialog::on_Cancel_clicked() {
    this->close();
}

void LineDialog::on_DDA_clicked() {
    if(ui->ID->text().isEmpty() || ui->x0->text().isEmpty() || ui->y0->text().isEmpty() || ui->x1->text().isEmpty() || ui->y1->text().isEmpty()) return;
    int id = ui->ID->text().toInt();
    float x0 = ui->x0->text().toFloat();
    float y0 = ui->y0->text().toFloat();
    float x1 = ui->x1->text().toFloat();
    float y1 = ui->y1->text().toFloat();
    emit drawLineEvent(id, x0, y0, x1, y1, 1);
    this->close();
}

void LineDialog::on_Bresenham_clicked() {
    if(ui->ID->text().isEmpty() || ui->x0->text().isEmpty() || ui->y0->text().isEmpty() || ui->x1->text().isEmpty() || ui->y1->text().isEmpty()) return;
    int id = ui->ID->text().toInt();
    float x0 = ui->x0->text().toFloat();
    float y0 = ui->y0->text().toFloat();
    float x1 = ui->x1->text().toFloat();
    float y1 = ui->y1->text().toFloat();
    emit drawLineEvent(id, x0, y0, x1, y1, 1);
    this->close();
}
