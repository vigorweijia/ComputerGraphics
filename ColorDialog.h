#ifndef COLORDIALOG_H
#define COLORDIALOG_H

#include <QDialog>
#include <QColorDialog>
#include <QColor>

namespace Ui {
class ColorDialog;
}

class ColorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ColorDialog(QWidget *parent = 0);
    ~ColorDialog();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_buttonPalette_clicked();

signals:
    void setColorEvent(int r, int g, int b);

private:
    Ui::ColorDialog *ui;
};

#endif // COLORDIALOG_H
