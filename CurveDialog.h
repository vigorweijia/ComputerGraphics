#ifndef CURVEDIALOG_H
#define CURVEDIALOG_H

#include <QDialog>
#include <vector>
#include <QDebug>
using namespace std;

namespace Ui {
class CurveDialog;
}

class CurveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CurveDialog(QWidget *parent = 0);
    ~CurveDialog();

private slots:
    void on_Bspline_clicked();
    void on_Bezier_clicked();
    void on_Cancel_clicked();

signals:
    void drawCurveEvent(int id,int n,vector<int> v,int type);

private:
    Ui::CurveDialog *ui;
};

#endif // CURVEDIALOG_H
