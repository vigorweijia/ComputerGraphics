#ifndef ROTATEDIALOG_H
#define ROTATEDIALOG_H

#include <QDialog>

namespace Ui {
class RotateDialog;
}

class RotateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RotateDialog(QWidget *parent = 0);
    ~RotateDialog();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

signals:
    void rotateEvent(int id,int cx,int cy,int angle);

private:
    Ui::RotateDialog *ui;
};

#endif // ROTATEDIALOG_H
