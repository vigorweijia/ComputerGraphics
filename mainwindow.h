#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "NewCanvasDialog.h"
#include <QPainter>
#include <QPixmap>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int newCanvasWidth;
    int newCanvasHeight;
    NewCanvasDialog *newCanvasDialog;


private:
    Ui::MainWindow *ui;
    QPainter *qPainter;
    QPixmap *qPixmap;

private slots:
    void on_actionNewCanvas_triggered();
    void onReceive_NewCanvasDialogAcceptedEvent(int width, int height);
};

#endif // MAINWINDOW_H
