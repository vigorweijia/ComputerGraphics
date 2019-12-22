#ifndef GALAXYDIALOG_H
#define GALAXYDIALOG_H

#include <QDialog>
#include <QKeyEvent>
#include "GalaxyWidget.h"

namespace Ui {
class GalaxyDialog;
}

class GalaxyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GalaxyDialog(QWidget *parent = 0);
    ~GalaxyDialog();

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

private:
    Ui::GalaxyDialog *ui;
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void nomarlizedAngle(int *angle);
    GalaxyWidget *galaxyWidget;
    QPoint lastPos;
    int xRot;
    int yRot;
    int zRot;
};

#endif // GALAXYDIALOG_H
