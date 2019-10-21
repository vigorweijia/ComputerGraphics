#ifndef ELLIPSEDIALOG_H
#define ELLIPSEDIALOG_H

#include <QDialog>

namespace Ui {
class EllipseDialog;
}

class EllipseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EllipseDialog(QWidget *parent = 0);
    ~EllipseDialog();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

signals:
    void drawEllipseEvent(int id, int x, int y, int rx, int ry);

private:
    Ui::EllipseDialog *ui;
};

#endif // ELLIPSEDIALOG_H
