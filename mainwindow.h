#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "NewCanvasDialog.h"
#include "LineDialog.h"
#include "EllipseDialog.h"
#include "PolygonDialog.h"
#include "ColorDialog.h"
#include "TranslateDialog.h"
#include "RotateDialog.h"
#include "ScaleDialog.h"
#include "ClipDialog.h"
#include "CurveDialog.h"
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
#include <QTimer>

#define TYPE_NOTHING 0
#define TYPE_LINE 1
#define TYPE_POLYGON 2
#define TYPE_ELLIPSE 3
#define TYPE_CURVE_BSPLINE 4
#define TYPE_CURVE_BEZIER 5
#define TYPE_SCALE 6
#define TYPE_ROTATE 7

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

    //--------non-Gui interface-----------
    void execNonGui(QString fileName, QString savingDir);
    //------------------------------------

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
    TranslateDialog *translateDialog;
    RotateDialog *rotateDialog;
    ScaleDialog *scaleDialog;
    ClipDialog *clipDialog;
    CurveDialog *curveDialog;
    //------------------------------------

    //---------Drag mouse to draw---------
    int selectedDrawEvent;
    int clickTimes;
    int selectedX0,selectedY0,selectedX1,selectedY1;
    int startX, startY;
    int allocatedId;
    int selectedId;
    int selectedIndex;
    int centralX, centralY;
    //return allocated id
    //when type == TYPE_POLYGON, must input id(id ==0 when first save Polygon)
    void drawCenter(QPainter *thisPainter);
    int saveDragGraphicUnit(int x0, int y0, int x1, int y1, int type, int id);
    int selectGraphicUnit(int nx, int ny); //return 0 if none
    //------------------------------------

    //------structure of Graphic Unit-----
    struct GraphicUnit {
        int id;
        int type;
        vector<int> para;
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
    void mouseDoubleClickEvent(QMouseEvent *e);
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

    //------画笔颜色-------
    QColor *qColor;
    //--------------------

    //------计时器---------
    QTimer *qTimer;
    //--------------------

    //----------各类事件的处理函数-------------------
    void createNewCanvas(int width, int height);
    bool isIdExist(int id); //判断图元id是否存在
    void drawLineDDA(float x0, float y0, float x1, float y1, QPainter *thisPainter);
    void drawLineBresenham(float x0, float y0, float x1, float y1, QPainter *thisPainter);
    void drawEllipse(int x, int y, int rx, int ry, QPainter *thisPainter);
    void drawPolygon(vector<int> v, int type, QPainter *thisPainter);
    void setColor(int R, int G, int B, QPainter *thisPainter);
    void createTempPixmapExceptId(int id); //绘制除id以外的所有图元
    void doTranslate(int id, int x, int y, QPainter *thisPainter);
    void doRotate(int id, int cx, int cy, int angle, QPainter *thisPainter);
    void doScale(int id, int cx, int cy, float scale, QPainter *thisPainter);
    void doClipCohenSutherland(int id, int x1, int y1, int x2, int y2, QPainter *thisPainter);
    void doClipLiangBarsky(int id, int x1, int y1, int x2, int y2, QPainter *thisPainter);
    void drawCurveBspline(vector<int> v, QPainter *thisPainter);
    struct Point
    {
        double x;
        double y;
        void operator =(const Point& B)
        {
            x = B.x;
            y = B.y;
        }
    };
    Point getFinalPoint(double t, int n);
    Point *controlPoint;
    Point *workPoint;
    void drawCurveBezier(vector<int> v, QPainter *thisPainter);
    //--------------------------------------------

private slots:

    //--------------File栏--------------------
    void on_actionNewCanvas_triggered();
    //void on_actionOpen_triggered();
    //void on_actionSave_triggered();
    void on_actionSaveAs_triggered();
    void on_actionImportFromFile_triggered();
    void doImportFromFile(QString fileName, QString savingDir);
    //----------------------------------------

    //--------------Edit栏--------------------
    void on_actionResetCanvas_triggered();
    void on_actionColor_triggered();
    void on_actionLine_triggered();
    void on_actionEllipse_triggered();
    void on_actionPolygon_triggered();
    void on_actionTranslate_triggered();
    void on_actionRotate_triggered();
    void on_actionScale_triggered();
    void on_actionClip_triggered();
    void on_actionCurve_triggered();
    //----------------------------------------

    //--------------Toolbar-------------------
    void cancelSelectedIcon();
    void on_actionLineIcon_triggered();
    void on_actionEllipseIcon_triggered();
    void on_actionPolygonIcon_triggered();
    void on_actionCurveIcon_triggered();
    void on_actionScaleIcon_triggered();
    void on_actionRotateIcon_triggered();
    void on_actionPointerIcon_triggered();
    //----------------------------------------

    //-------------------------Signals from child dialog-------------------------------
    void onReceive_NewCanvasDialogAcceptedEvent(int width, int height);
    void onReceive_DrawLine(int id, float x0, float y0, float x1, float y1, int type);
    void onReceive_DrawEllipse(int id, int x, int y, int rx, int ry);
    void onReceive_DrawPolygon(int id, int n, vector<int> v, int type);
    void onReceive_SetColor(int r, int g, int b);
    void onReceive_Translate(int id, int dx, int dy);
    void onReceive_Rotate(int id, int cx, int cy, int angle);
    void onReceive_Scale(int id, int cx, int cy, float scale);
    void onReceive_Clip(int id, int x1, int y1, int x2, int y2, int type);
    void onReceive_DrawCurve(int id, int n, vector<int> v, int type);
    //---------------------------------------------------------------------------------
};

#endif // MAINWINDOW_H
