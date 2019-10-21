#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "NewCanvasDialog.h"
#include "LineDialog.h"
#include "EllipseDialog.h"
#include <vector>
#include <QDebug>
#include <QPainter>
#include <QPixmap>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QBuffer>
#include <QDialog>

#define TYPE_LINE 1
#define TYPE_POLYGON 2
#define TYPE_ELLIPSE 3
#define TYPE_CURVE 4

using namespace std;

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
    LineDialog *lineDialog;
    EllipseDialog *ellipseDialog;


    struct GraphicUnit {
        int id;
        int type;
        vector<float> para;
    };
    vector<GraphicUnit> v;

private:
    Ui::MainWindow *ui;
    QPainter *qPainter;
    QPixmap *qPixmap;
    bool isIdExist(int id);
    void drawLineDDA(float x0, float y0, float x1, float y1);
    void drawLineBresenham(float x0, float y0, float x1, float y1);
    void drawEllipse(int x, int y, int rx, int ry);

private slots:
    void on_actionNewCanvas_triggered();
    //void on_actionOpen_triggered();
    //void on_actionSave_triggered();
    void on_actionSaveAs_triggered();
    //void on_actionImportFromFile_triggered();
    //void on_actionResetCanvas_triggered();
    //void on_actionColor_triggered();
    void on_actionLine_triggered();
    void onReceive_NewCanvasDialogAcceptedEvent(int width, int height);
    void onReceive_DrawLine(int id, float x0, float y0, float x1, float y1, int type);
    void onReceive_DrawEllipse(int id, int x, int y, int rx, int ry);
};

#endif // MAINWINDOW_H
