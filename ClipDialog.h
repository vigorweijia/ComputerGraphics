#ifndef CLIPDIALOG_H
#define CLIPDIALOG_H

#include <QDialog>

namespace Ui {
class ClipDialog;
}

class ClipDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClipDialog(QWidget *parent = 0);
    ~ClipDialog();

private slots:
    void on_cohenSutherland_clicked();
    void on_liangBarsky_clicked();
    void on_cancel_clicked();

signals:
    void clipEvent(int id, int x1, int x2, int y1, int y2, int type);

private:
    Ui::ClipDialog *ui;
};

#endif // CLIPDIALOG_H
