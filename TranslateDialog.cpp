#include "TranslateDialog.h"
#include "ui_TranslateDialog.h"

TranslateDialog::TranslateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TranslateDialog)
{
    ui->setupUi(this);
}

TranslateDialog::~TranslateDialog()
{
    delete ui;
}

void TranslateDialog::on_buttonBox_accepted()
{
    if(ui->ID->text().isEmpty() || ui->dx->text().isEmpty() || ui->dy->text().isEmpty()) return;
    int id = ui->ID->text().toInt();
    int dx = ui->dx->text().toInt();
    int dy = ui->dy->text().toInt();
    emit translateEvent(id, dx, dy);
    this->close();
}

void TranslateDialog::on_buttonBox_rejected()
{
    this->close();
}
