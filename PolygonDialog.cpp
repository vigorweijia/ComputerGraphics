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

void PolygonDialog::on_DDA_clicked()
{
    if(ui->ID->text().isEmpty() || ui->number->text().isEmpty() || ui->points->text().isEmpty()) return;
    int id = ui->ID->text().toInt();
    int n = ui->number->text().toInt();
    QStringList strList = ui->points->text().split(" ");
    if(strList.size() != n) return;
    vector<int> v;
    for(int i = 0; i < n; i++)
    {
        v.push_back(strList[i*2].toInt());
        v.push_back(strList[i*2+1].toInt());
    }
    emit drawPolygonEvent(id, n, v, 0);
    this->close();
}

void PolygonDialog::on_Bresenham_clicked()
{
    if(ui->ID->text().isEmpty() || ui->number->text().isEmpty() || ui->points->text().isEmpty()) return;
    int id = ui->ID->text().toInt();
    int n = ui->number->text().toInt();
    QStringList strList = ui->points->text().split(" ");
    if(strList.size() != 2*n) return;
    vector<int> v;
    for(int i = 0; i < n; i++)
    {
        v.push_back(strList[i*2].toInt());
        v.push_back(strList[i*2+1].toInt());
    }
    emit drawPolygonEvent(id, n, v, 1);
    this->close();
}

void PolygonDialog::on_Cancel_clicked()
{
    this->close();
}
