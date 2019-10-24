#ifndef POLYGONDIALOG_H
#define POLYGONDIALOG_H

#include <QDialog>
#include <vector>
#include <QDebug>
using namespace std;

namespace Ui {
class PolygonDialog;
}

class PolygonDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PolygonDialog(QWidget *parent = 0);
    ~PolygonDialog();

private slots:
    void on_DDA_clicked();
    void on_Bresenham_clicked();
    void on_Cancel_clicked();

signals:
    void drawPolygonEvent(int id, int n, vector<int> v, int type);

private:
    Ui::PolygonDialog *ui;
};

#endif // POLYGONDIALOG_H
