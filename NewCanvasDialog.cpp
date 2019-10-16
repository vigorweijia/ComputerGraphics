#include "NewCanvasDialog.h"
#include "ui_NewCanvasDialog.h"

NewCanvasDialog::NewCanvasDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewCanvasDialog)
{
    ui->setupUi(this);
}

NewCanvasDialog::~NewCanvasDialog()
{
    delete ui;
}

void NewCanvasDialog::on_buttonBox_accepted()
{
    int w = ui->lineEdit->text().toInt();
    int d = ui->lineEdit_2->text().toInt();
    emit newCanvasDialogAcceptedEvent(w, d);
}

void NewCanvasDialog::on_buttonBox_rejected()
{
    this->close();
}
