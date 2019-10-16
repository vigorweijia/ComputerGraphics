#ifndef NEWCANVASDIALOG_H
#define NEWCANVASDIALOG_H

#include <QDialog>

namespace Ui {
class NewCanvasDialog;
}

class NewCanvasDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewCanvasDialog(QWidget *parent = 0);
    ~NewCanvasDialog();

private slots:

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

signals:
    void newCanvasDialogAcceptedEvent(int width, int height);

private:
    Ui::NewCanvasDialog *ui;
};

#endif // NEWCANVASDIALOG_H
