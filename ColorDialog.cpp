#include "ColorDialog.h"
#include "ui_ColorDialog.h"

ColorDialog::ColorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColorDialog)
{
    ui->setupUi(this);
}

ColorDialog::~ColorDialog()
{
    delete ui;
}

void ColorDialog::on_buttonBox_accepted()
{
    if(ui->colorR->text().isEmpty() || ui->colorG->text().isEmpty() || ui->colorB->text().isEmpty()) return;
    emit setColorEvent(ui->colorR->text().toInt(), ui->colorG->text().toInt(), ui->colorB->text().toInt());
}

void ColorDialog::on_buttonBox_rejected()
{
    this->close();
}

void ColorDialog::on_buttonPalette_clicked()
{
    QColor qColor = QColorDialog::getColor(Qt::black, this);
    if(qColor.isValid())
    {
        ui->colorR->setText(QString::number(qColor.red()));
        ui->colorG->setText(QString::number(qColor.green()));
        ui->colorB->setText(QString::number(qColor.blue()));
    }
}
