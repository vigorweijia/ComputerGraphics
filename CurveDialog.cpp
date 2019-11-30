#include "CurveDialog.h"
#include "ui_CurveDialog.h"

CurveDialog::CurveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CurveDialog)
{
    ui->setupUi(this);
}

CurveDialog::~CurveDialog()
{
    delete ui;
}

void CurveDialog::on_Bspline_clicked()
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
    emit drawCurveEvent(id, n, v, 1);
    this->close();
}

void CurveDialog::on_Bezier_clicked()
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
    emit drawCurveEvent(id, n, v, 1);
    this->close();
}

void CurveDialog::on_Cancel_clicked()
{
    this->close();
}
