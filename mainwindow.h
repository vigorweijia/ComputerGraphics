#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "NewCanvasDialog.h"
#include "LineDialog.h"
#include "EllipseDialog.h"
#include "PolygonDialog.h"
#include "ColorDialog.h"
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
#include <QColor>
#include <QPen>
#include <QMouseEvent>

#define TYPE_NOTHING 0
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

    //-----width and height of canvas-----
    int newCanvasWidth;
    int newCanvasHeight;
    //------------------------------------

    //--------kinds of dialogs------------
    NewCanvasDialog *newCanvasDialog;
    LineDialog *lineDialog;
    EllipseDialog *ellipseDialog;
    PolygonDialog *polygonDialog;
    ColorDialog *colorDialog;
    //------------------------------------

    //---------Drag mouse to draw---------
    int selectedDrawEvent;
    int clickTimes;
    int selectedX0,selectedY0,selectedX1,selectedY1;
    //------------------------------------

    //------structure of Graphic Unit-----
    struct GraphicUnit {
        int id;
        int type;
        vector<float> para;
        struct Color{
            char r;
            char g;
            char b;
        }color;
    };
    vector<GraphicUnit> v;
    //------------------------------------

protected:
    //----------mouse drag event--------------
    int baseX,baseY; //用于计算相对于label(Canvas)的坐标
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    //----------------------------------------

private:
    Ui::MainWindow *ui;
    //-------主画笔和画布（置底）--------
    QPainter *qPainter;
    QPixmap *qPixmap;
    //--------------------------------

    //------临时画笔和画布（置顶）-------
    QPainter *tempPainter;
    QPixmap *tempPixmap;
    //--------------------------------

    QColor *qColor;//color of painter

    //----------各类事件的处理函数-------------------
    void createNewCanvas(int width, int height);
    bool isIdExist(int id); //判断图元id是否存在
    void drawLineDDA(float x0, float y0, float x1, float y1, QPainter *thisPainter);
    void drawLineBresenham(float x0, float y0, float x1, float y1, QPainter *thisPainter);
    void drawEllipse(int x, int y, int rx, int ry, QPainter *thisPainter);
    void drawPolygon(int n, vector<int> v, int type, QPainter *thisPainter);
    void setColor(int R, int G, int B, QPainter *thisPainter);
    void createTempPixmapExceptId(int id); //绘制除id以外的所有图元
    //--------------------------------------------

private slots:

    //--------------File栏--------------------
    void on_actionNewCanvas_triggered();
    //void on_actionOpen_triggered();
    //void on_actionSave_triggered();
    void on_actionSaveAs_triggered();
    void on_actionImportFromFile_triggered();
    //----------------------------------------

    //--------------Edit栏--------------------
    void on_actionResetCanvas_triggered();
    void on_actionColor_triggered();
    void on_actionLine_triggered();
    void on_actionEllipse_triggered();
    void on_actionPolygon_triggered();
    //void on_actionTranslate_triggered();
    //void on_actionRotate_triggered();
    //----------------------------------------

    //--------------Toolbar-------------------
    void on_actionLineIcon_triggered();
    void on_actionEllipseIcon_triggered();
    //----------------------------------------

    //-------------------------Signals from child dialog-------------------------------
    void onReceive_NewCanvasDialogAcceptedEvent(int width, int height);
    void onReceive_DrawLine(int id, float x0, float y0, float x1, float y1, int type);
    void onReceive_DrawEllipse(int id, int x, int y, int rx, int ry);
    void onReceive_DrawPolygon(int id, int n, vector<int> v, int type);
    void onReceive_SetColor(int r, int g, int b);
    //---------------------------------------------------------------------------------
};

#endif // MAINWINDOW_H
