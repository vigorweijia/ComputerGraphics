#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//-------------new timer------------------------
    qTimer = new QTimer(this);

//----------------------------------------------

//-------------new canvas dialog----------------
    newCanvasDialog = new NewCanvasDialog();
    connect(newCanvasDialog, SIGNAL(newCanvasDialogAcceptedEvent(int,int)), this, SLOT(onReceive_NewCanvasDialogAcceptedEvent(int,int)));
//----------------------------------------------

//-------------draw line dialog-----------------
    lineDialog = new LineDialog();
    connect(lineDialog, SIGNAL(drawLineEvent(int,float,float,float,float,int)), this, SLOT(onReceive_DrawLine(int,float,float,float,float,int)));
//----------------------------------------------

//-------------draw ellipse dialog--------------
    ellipseDialog = new EllipseDialog();
    connect(ellipseDialog, SIGNAL(drawEllipseEvent(int,int,int,int,int)), this, SLOT(onReceive_DrawEllipse(int,int,int,int,int)));
//----------------------------------------------

//-------------draw polygon dialog--------------
    polygonDialog = new PolygonDialog();
    connect(polygonDialog, SIGNAL(drawPolygonEvent(int,int,vector<int>,int)), this, SLOT(onReceive_DrawPolygon(int,int,vector<int>,int)));
//----------------------------------------------

//-------------the color dialog-----------------
    colorDialog = new ColorDialog();
    connect(colorDialog, SIGNAL(setColorEvent(int,int,int)), this, SLOT(onReceive_SetColor(int,int,int)));
//----------------------------------------------

//-------------Translate Dialog-----------------
    translateDialog = new TranslateDialog();
    connect(translateDialog, SIGNAL(translateEvent(int,int,int)), this, SLOT(onReceive_Translate(int,int,int)));
//----------------------------------------------

//-------------Rotate Dialog--------------------
    rotateDialog = new RotateDialog();
    connect(rotateDialog, SIGNAL(rotateEvent(int,int,int,int)), this, SLOT(onReceive_Rotate(int,int,int,int)));
//----------------------------------------------

//-------------Scale Dialog---------------------
    scaleDialog = new ScaleDialog();
    connect(scaleDialog, SIGNAL(scaleEvent(int,int,int,float)), this, SLOT(onReceive_Scale(int,int,int,float)));
//----------------------------------------------

//-------------Clip Dialog----------------------
    clipDialog = new ClipDialog();
    connect(clipDialog, SIGNAL(clipEvent(int,int,int,int,int,int)), this, SLOT(onReceive_Clip(int,int,int,int,int,int)));
//----------------------------------------------

//-------------Curve Dialog---------------------
    curveDialog = new CurveDialog();
    connect(curveDialog, SIGNAL(drawCurveEvent(int,int,vector<int>,int)), this, SLOT(onReceive_DrawCurve(int,int,vector<int>,int)));
//----------------------------------------------

//-------------Selected Event-------------------
    selectedDrawEvent = TYPE_NOTHING;
    clickTimes = 0;
//----------------------------------------------
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::execNonGui(QString fileName, QString savingDir)
{
    doImportFromFile(fileName, savingDir);
}

bool MainWindow::isIdExist(int id)
{
    for(int i = 0; i < v.size(); i++) {if(v[i].id == id) return true;}
    return false;
}

void MainWindow::createTempPixmapExceptId(int id)
{
    qPainter->fillRect(0,0,newCanvasWidth,newCanvasHeight,Qt::white);
    for(int i = 0; i < v.size(); i++)
    {
        if(v[i].id == id) continue;
        int tempR = qColor->red(), tempG = qColor->green(), tempB = qColor->blue();
        setColor(v[i].color.r&0x000000ff, v[i].color.g&0x000000ff, v[i].color.b&0x000000ff, qPainter);
        switch (v[i].type) {
        case TYPE_LINE: drawLineBresenham((float)v[i].para[0],(float)v[i].para[1],(float)v[i].para[2],(float)v[i].para[3],qPainter); break;
        case TYPE_ELLIPSE: drawEllipse(v[i].para[0],v[i].para[1],v[i].para[2],v[i].para[3],qPainter); break;
        case TYPE_POLYGON: drawPolygon(v[i].para, 1, qPainter); break;
        case TYPE_CURVE: break;
        default: break;
        }
        setColor(tempR, tempG, tempB, qPainter);
    }
}

void MainWindow::on_actionNewCanvas_triggered()
{
    newCanvasDialog->show();
}

void MainWindow::createNewCanvas(int width, int height)
{
    newCanvasWidth = width;
    newCanvasHeight = height;

    //ui->textBrowser->setText(QString::number(newCanvasWidth)+","+QString::number(height));
    qPixmap = new QPixmap(width, height);
    tempPixmap = new QPixmap(width, height);
    tempPixmap->fill(Qt::transparent); //整个pixmap改成透明，不然默认是灰色

    //-------------Create the QPainter object----------------
    qPainter = new QPainter(qPixmap);
    qPainter->fillRect(0,0,width,height,Qt::white);
    tempPainter = new QPainter(tempPixmap);
    //tempPainter->fillRect(30,30,40,40,Qt::blue);
    //-------------------------------------------------------

    //-------------Reset the size of Window------------------
    if(width > this->width() || height > this->height()) {
        this->resize(width+50,height+50);
    }
    //-------------------------------------------------------

    //-------------Reset the label (Canvas)------------------
    if(width > ui->label->width() || height > ui->label->height()) {
        ui->label->setGeometry(10,10,width,height);
        ui->tempLabel->setGeometry(10,10,width,height);
    }
    baseX = ui->label->x() + ui->centralWidget->x();
    baseY = ui->label->y() + ui->centralWidget->y();
    //-------------------------------------------------------

    //-------------Set default color-------------------------
    qColor = new QColor(0,0,0);
    //-------------------------------------------------------

    ui->label->setPixmap(*qPixmap);
    ui->tempLabel->setPixmap(*tempPixmap);
    //qPixmap->save("1.bmp");
}

void MainWindow::onReceive_NewCanvasDialogAcceptedEvent(int width, int height)
{
    createNewCanvas(width, height);
    newCanvasDialog->close();
}

void MainWindow::on_actionSaveAs_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "Open File", "/home", "Image File(*.bmp)");
    if(!filename.isNull()) {
        QFile qFile(filename);
        if(!qFile.open(QFile::WriteOnly|QFile::Text)) {
            QMessageBox::warning(this, "ERROR", tr("Failed to open file!"));
            return;
        }
        QTextStream qTextStream(&qFile);
        QByteArray qByteArray;
        QBuffer qBuffer(&qByteArray);
        qBuffer.open(QIODevice::WriteOnly);
        qPixmap->save(&qBuffer, "BMP");
        qFile.write(qByteArray);
    }
    else {
        qDebug() << "Cancel";
    }
}

void MainWindow::on_actionImportFromFile_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", "/home", "Text File(*.txt)");
    doImportFromFile(fileName, QString(""));
}

void MainWindow::doImportFromFile(QString fileName, QString savingDir)
{
    if(!fileName.isNull())
    {
        QFile qFile(fileName);
        if(!qFile.open(QFile::ReadOnly|QFile::Text))
        {
            QMessageBox::warning(this, "ERROR", tr("Failed to open file!"));
            return;
        }
        QTextStream qTextStream(&qFile);
        qTextStream.seek(0); //move to the begining of the file
        QString textLine;
        while(!qTextStream.atEnd())
        {
            textLine = qTextStream.readLine();
            qDebug() << textLine;
            QStringList strList = textLine.split(" ");
            if(strList[0].compare(QString("resetCanvas")) == 0)
            {
                qDebug() << "now at resetCanvas " << strList.size() << "\n";
                createNewCanvas(strList[1].toInt(), strList[2].toInt());
            }
            else if(strList[0].compare(QString("saveCanvas")) == 0)
            {
                qDebug() << "now at saveCanvas" << strList.size() << "\n";
                if(savingDir.compare(QString("")) == 0)
                    qPixmap->save(strList[1]+".bmp");
                else
                    qPixmap->save(savingDir+strList[1]+".bmp");
            }
            else if(strList[0].compare(QString("setColor")) == 0)
            {
                qDebug() << "now at setColor\n";
                setColor(strList[1].toInt(), strList[2].toInt(), strList[3].toInt(), qPainter);
            }
            else if(strList[0].compare(QString("drawLine")) == 0)
            {
                qDebug() << "now at drawLine " << strList.size() << "\n";
                int id = strList[1].toInt();
                if(isIdExist(id))
                {
                    qDebug() << "id " << id << " exists, draw line failed.\n";
                    return;
                }
                int x0 = strList[2].toInt();
                int y0 = strList[3].toInt();
                int x1 = strList[4].toInt();
                int y1 = strList[5].toInt();
                GraphicUnit g;
                g.id = id;
                g.type = TYPE_LINE;
                g.para.push_back(x0);
                g.para.push_back(y0);
                g.para.push_back(x1);
                g.para.push_back(y1);
                g.color.r = (char)((qColor->red())&0xff);
                g.color.g = (char)((qColor->green())&0xff);
                g.color.b = (char)((qColor->blue())&0xff);
                v.push_back(g);
                if(strList[0].compare(QString("DDA"))) drawLineDDA((float)x0, (float)y0, (float)x1, (float)y1, qPainter);
                if(strList[0].compare(QString("Bresenham"))) drawLineBresenham((float)x0, (float)y0, (float)x1, (float)y1, qPainter);
                ui->label->setPixmap(*qPixmap);
            }
            else if(strList[0].compare(QString("drawPolygon")) == 0)
            {
                qDebug() << "now at drawPolygon.\n";
                GraphicUnit g;
                int id = strList[1].toInt();
                g.id = id;
                g.type = TYPE_POLYGON;
                int n = strList[2].toInt();
                g.para.push_back(n);
                int type = (strList[3].compare(QString("DDA")) == 0) ? 0 : 1;
                textLine = qTextStream.readLine();
                strList = textLine.split(" ");
                for(int i = 0; i < n; i++)
                {
                    g.para.push_back(strList[i*2].toInt());
                    g.para.push_back(strList[i*2+1].toInt());
                }
                g.color.r = (char)((qColor->red())&0xff);
                g.color.g = (char)((qColor->green())&0xff);
                g.color.b = (char)((qColor->blue())&0xff);
                v.push_back(g);
                drawPolygon(g.para, type, qPainter);
                ui->label->setPixmap(*qPixmap);
            }
            else if(strList[0].compare(QString("drawEllipse")) == 0)
            {
                qDebug() << "now at drawEllipse " << strList.size() << "\n";
                int id = strList[1].toInt();
                int x = strList[2].toInt();
                int y = strList[3].toInt();
                int rx = strList[4].toInt();
                int ry = strList[5].toInt();
                GraphicUnit g;
                g.id = id;
                g.type = TYPE_ELLIPSE;
                g.para.push_back(x);
                g.para.push_back(y);
                g.para.push_back(rx);
                g.para.push_back(ry);
                g.color.r = (char)((qColor->red())&0xff);
                g.color.g = (char)((qColor->green())&0xff);
                g.color.b = (char)((qColor->blue())&0xff);
                v.push_back(g);
                drawEllipse(x, y, rx, ry, qPainter);
                ui->label->setPixmap(*qPixmap);
            }
            else if(strList[0].compare(QString("drawCurve")) == 0)
            {
                qDebug() << "not at curve.\n";
                GraphicUnit g;
                int id = strList[1].toInt();
                g.id = id;
                g.type = TYPE_CURVE;
                int n = strList[2].toInt();
                g.para.push_back(n);
                int type = (strList[3].compare(QString("B-spline")) == 0) ? 0 : 1;
                textLine = qTextStream.readLine();
                strList = textLine.split(" ");
                for(int i = 0; i < n; i++)
                {
                    g.para.push_back(strList[i*2].toInt());
                    g.para.push_back(strList[i*2+1].toInt());
                }
                g.color.r = (char)((qColor->red())&0xff);
                g.color.g = (char)((qColor->green())&0xff);
                g.color.b = (char)((qColor->blue())&0xff);
                v.push_back(g);
                if(type == 0) drawCurveBspline(g.para, qPainter);
                else if(type == 1) drawCurveBezier(g.para, qPainter);
                ui->label->setPixmap(*qPixmap);
            }
            else if(strList[0].compare(QString("translate")) == 0)
            {
                qDebug() << "now at translate.\n";
                int id = strList[1].toInt();
                int dx = strList[2].toInt();
                int dy = strList[3].toInt();
                doTranslate(id, dx, dy, qPainter);
            }
            else if(strList[0].compare(QString("rotate")) == 0)
            {
                qDebug() << "now at rotate.\n";
                int id = strList[1].toInt();
                int cx = strList[2].toInt();
                int cy = strList[3].toInt();
                int angle = strList[4].toInt();
                doRotate(id, cx, cy, angle, qPainter);
            }
            else if(strList[0].compare(QString("scale")) == 0)
            {
                qDebug() << "now at scale.\n";
                int id = strList[1].toInt();
                int cx = strList[2].toInt();
                int cy = strList[3].toInt();
                float scale = strList[4].toFloat();
                doScale(id, cx, cy, scale, qPainter);
            }
            else if(strList[0].compare(QString("clip")) == 0)
            {
                qDebug() << "now at clip.\n";
                int id = strList[1].toInt();
                int x1 = strList[2].toInt();
                int y1 = strList[3].toInt();
                int x2 = strList[4].toInt();
                int y2 = strList[5].toInt();
                if(strList[0].compare(QString("Cohen-Sutherland")))
                {
                    doClipCohenSutherland(id, x1, y1, x2, y2, qPainter);
                }
                else
                {
                    doClipLiangBarsky(id, x1, y1, x2, y2, qPainter);
                }

            }
            else
            {
                qDebug() << "unknown command of " << strList[0] << "\n";
            }
        }
    }
    else
    {
        qDebug() << "Cancel\n";
    }
}

void MainWindow::on_actionResetCanvas_triggered() {
    newCanvasDialog->show();
}

void MainWindow::on_actionLine_triggered()
{
    lineDialog->show();
}

void MainWindow::onReceive_DrawLine(int id, float x0, float y0, float x1, float y1, int type)
{
    if(isIdExist(id)) {
        QMessageBox::warning(this, "ERROR", tr("Drawing error! The ID is repeated."));
        return;
    }
    GraphicUnit g;
    g.id = id;
    g.type = TYPE_LINE;
    g.para.push_back((int)x0);
    g.para.push_back((int)y0);
    g.para.push_back((int)x1);
    g.para.push_back((int)y1);
    g.color.r = (char)((qColor->red())&0xff);
    g.color.g = (char)((qColor->green())&0xff);
    g.color.b = (char)((qColor->blue())&0xff);
    v.push_back(g);
    if(type == 0) drawLineDDA(x0, y0, x1, y1, qPainter);
    else if(type == 1) drawLineBresenham(x0, y0, x1, y1, qPainter);
    ui->label->setPixmap(*qPixmap);
}

void MainWindow::drawLineDDA(float x0, float y0, float x1, float y1, QPainter *thisPainter)
{
    if(x1 < x0) {swap(x0,x1); swap(y0,y1);}
    if(x1 == x0) {
        if(y1 < y0) swap(y1, y0);
        for(int yi = (int)y0; yi <= (int)y1; yi++) thisPainter->drawPoint((int)x0, yi);
        return;
    }
    if(y1 == y0) {
        for(int xi = (int)x0; xi <= (int)x1; xi++) thisPainter->drawPoint(xi, (int)y0);
        return;
    }
    float m = (y1 - y0)/(x1 - x0);
    if(abs(m) <= 1) {
        if(x1 < x0) {swap(x0,x1); swap(y0,y1);}
        thisPainter->drawPoint((int)x0, (int)y0);
        float yi = y0;
        for(int xi = (int)x0 + 1; xi <= (int)x1; xi++) {
            yi = (float)yi + m;
            thisPainter->drawPoint(xi, (int)(yi+0.5)); //四舍五入
        }
    }
    else {
        if(y1 < y0) {swap(x0,x1); swap(y0,y1);}
        thisPainter->drawPoint((int)x0, (int)y0);
        float xi = x0;
        for(int yi = (int)y0 + 1; yi <= (int)y1; yi++) {
            xi = (float)xi + (1/m);
            thisPainter->drawPoint((int)(xi+0.5), yi); //四舍五入
        }
    }
}

void MainWindow::drawLineBresenham(float x0, float y0, float x1, float y1, QPainter *thisPainter)
{
    if(x1 < x0) {swap(x0,x1); swap(y0,y1);}
    if(x1 == x0) {
        if(y1 < y0) swap(y1, y0);
        for(int yi = (int)y0; yi <= (int)y1; yi++) thisPainter->drawPoint((int)x0, yi);
        return;
    }
    if(y1 == y0) {
        for(int xi = (int)x0; xi <= (int)x1; xi++) thisPainter->drawPoint(xi, (int)y0);
        return;
    }
    if(x1 - x0 == y1 - y0) {
        for(int xi = (int)x0; xi <= (int)x1; xi++) thisPainter->drawPoint(xi, (int)y0 + xi - (int)x0);
        return;
    }
    float m = (y1 - y0)/(x1 - x0);
    int deltaY = (int)abs(y1-y0), deltaX = (int)abs(x1-x0);
    int p;
    thisPainter->drawPoint((int)x0, (int)y0);
    if(abs(m) < 1) {
        if(x1 < x0) {swap(x0,x1); swap(y0,y1);}
        p = 2*deltaY - deltaX;
        int yi = y0;
        for(int xi = (int)x0 + 1; xi <= (int)x1; xi++) {
            if(p < 0) p = p + 2*deltaY;
            else p = p + 2*deltaY - 2*deltaX;
            if(p >= 0) //边界条件
            {
                if(m > 0) yi = yi + 1;
                else yi = yi - 1;
            }
            thisPainter->drawPoint(xi, yi);
        }
    }
    else {
        if(y1 < y0) {swap(x0,x1); swap(y0,y1);}
        p = 2*deltaX - deltaY;
        int xi = x0;
        for(int yi = (int)y0 + 1; yi <= (int)y1; yi++) {
            if(p < 0) p = p + 2*deltaX;
            else p = p + 2*deltaX - 2*deltaY;
            //qDebug() << "p:" << p;
            if(p >= 0) //边界条件
            {
                if(m > 0) xi = xi + 1;
                else xi = xi - 1;
            }
            //qDebug()  << "xi:" << xi << " yi:" << yi;
            thisPainter->drawPoint(xi, yi);
        }
    }
}

void MainWindow::on_actionEllipse_triggered() {
    ellipseDialog->show();
}

void MainWindow::onReceive_DrawEllipse(int id, int x, int y, int rx, int ry) {
    if(isIdExist(id)) {
        QMessageBox::warning(this, "ERROR", tr("Drawing error! The ID is repeated."));
        return;
    }
    GraphicUnit g;
    g.id = id;
    g.type = TYPE_ELLIPSE;
    g.para.push_back(x);
    g.para.push_back(y);
    g.para.push_back(rx);
    g.para.push_back(ry);
    g.color.r = (char)((qColor->red())&0xff);
    g.color.g = (char)((qColor->green())&0xff);
    g.color.b = (char)((qColor->blue())&0xff);
    v.push_back(g);
    drawEllipse(x, y, rx, ry, qPainter);
    ui->label->setPixmap(*qPixmap);
}

void MainWindow::drawEllipse(int x, int y, int rx, int ry, QPainter *thisPainter) {
    int xi = 0, yi = ry;
    thisPainter->drawPoint(x + xi, y + yi);
    thisPainter->drawPoint(x + xi, y - yi);
    float p1k = ry*ry - rx*rx*ry + (float)(rx*rx)/4;
    int lastX, lastY;
    for(xi = 1; ry*ry*xi < rx*rx*yi; xi++)
    {
        if(p1k < 0)
        {
            p1k = p1k + 2*ry*ry*xi + ry*ry;
            lastX = xi, lastY = yi;
            thisPainter->drawPoint(x + xi, y + yi);
            thisPainter->drawPoint(x - xi, y + yi);
            thisPainter->drawPoint(x + xi, y - yi);
            thisPainter->drawPoint(x - xi, y - yi);
        }
        else
        {
            yi = yi - 1;
            p1k = p1k + 2*ry*ry*xi - 2*rx*rx*yi + ry*ry;
            lastX = xi, lastY = yi;
            thisPainter->drawPoint(x + xi, y + yi);
            thisPainter->drawPoint(x - xi, y + yi);
            thisPainter->drawPoint(x + xi, y - yi);
            thisPainter->drawPoint(x - xi, y - yi);
        }
        //qDebug() << xi << " " << yi << "\n";
    }
    /*xi = lastX, yi = lastY;
    float p2k = (float)ry*(float)ry*((float)xi+0.5f)*((float)xi+0.5f) + (float)rx*rx*(yi-1) - (float)rx*rx*ry*ry;
    for(yi = yi - 1; yi >= 0; yi--)
    {
        if(p2k > 0)
        {
            p2k = p2k - 2*rx*rx*yi + rx*rx;
            qPainter->drawPoint(x + xi, y + yi);
            qPainter->drawPoint(x - xi, y + yi);
            qPainter->drawPoint(x + xi, y - yi);
            qPainter->drawPoint(x - xi, y - yi);
        }
        else
        {
            xi = xi + 1;
            p2k = p2k + 2*ry*ry*xi - 2*rx*rx*yi + rx*rx;
            qPainter->drawPoint(x + xi, y + yi);
            qPainter->drawPoint(x - xi, y + yi);
            qPainter->drawPoint(x + xi, y - yi);
            qPainter->drawPoint(x - xi, y - yi);
        }
        qDebug() << xi << " " << yi << "\n";
    }*/
    thisPainter->drawPoint(x + xi, y + yi);
    thisPainter->drawPoint(x - xi, y + yi);
    thisPainter->drawPoint(x + xi, y - yi);
    thisPainter->drawPoint(x - xi, y - yi);
    xi = rx;
    yi = 0;
    thisPainter->drawPoint(x + xi, y + yi);
    thisPainter->drawPoint(x - xi, y + yi);
    float p2k = rx*rx - ry*ry*rx + (float)(ry*ry)/4;
    for(yi = yi + 1; rx*rx*yi < ry*ry*xi; yi++)
    {
        if(p2k < 0)
        {
            p2k = p2k + 2*rx*rx*yi + rx*rx;
            thisPainter->drawPoint(x + xi, y + yi);
            thisPainter->drawPoint(x - xi, y + yi);
            thisPainter->drawPoint(x + xi, y - yi);
            thisPainter->drawPoint(x - xi, y - yi);
        }
        else
        {
            xi = xi - 1;
            p2k = p2k + 2*rx*rx*yi - 2*ry*ry*xi + rx*rx;
            thisPainter->drawPoint(x + xi, y + yi);
            thisPainter->drawPoint(x - xi, y + yi);
            thisPainter->drawPoint(x + xi, y - yi);
            thisPainter->drawPoint(x - xi, y - yi);
        }
    }
}

void MainWindow::on_actionPolygon_triggered()
{
    polygonDialog->show();
}

void MainWindow::onReceive_DrawPolygon(int id, int n, vector<int> tempV, int type)
{
    if(isIdExist(id)) {
        QMessageBox::warning(this, "ERROR", tr("Drawing error! The ID is repeated."));
        return;
    }
    GraphicUnit g;
    g.id = id;
    g.para.push_back(n);
    for(int i = 0; i < tempV.size(); i++) g.para.push_back(tempV[i]);
    g.type = TYPE_POLYGON;
    g.color.r = (char)((qColor->red())&0xff);
    g.color.g = (char)((qColor->green())&0xff);
    g.color.b = (char)((qColor->blue())&0xff);
    v.push_back(g);
    drawPolygon(g.para, type, qPainter);
    ui->label->setPixmap(*qPixmap);
}

void MainWindow::drawPolygon(vector<int> tempV, int type, QPainter *thisPainter)
{
    //qDebug() << "paraSize" << tempV.size();
    int n = tempV[0];
    for(int i = 0; i < n; i++)
    {
        float x0 = (float)tempV[i*2 +1];
        float y0 = (float)tempV[i*2+1 +1];
        float x1 = (float)tempV[(i*2+2)%(2*n) +1];
        float y1 = (float)tempV[(i*2+3)%(2*n) +1];
        /*qDebug() << "(i*2+3)%2n+1=" << (i*2+3)%(2*n)+1;
        qDebug() << "From " << x0 << "," << y0 << " to " << x1 << "," << y1;*/
        if(type == 0) drawLineDDA(x0, y0, x1, y1, thisPainter);
        else if(type == 1) drawLineBresenham(x0, y0, x1, y1, thisPainter);
    }
}

void MainWindow::on_actionColor_triggered()
{
    colorDialog->show();
}

void MainWindow::onReceive_SetColor(int r, int g, int b)
{
    setColor(r, g, b, qPainter);
}

void MainWindow::setColor(int R, int G, int B, QPainter *thisPainter)
{
    *qColor = QColor(R, G, B);
    thisPainter->setPen(*qColor);
}

void MainWindow::cancelSelectedIcon()
{
    if(selectedDrawEvent == TYPE_POLYGON && selectedX1 != -1 && selectedY1 != -1) //complete drawing
    {
        drawLineBresenham((float)startX, (float)startY, (float)selectedX1, (float)selectedY1, qPainter);
        tempPixmap->fill(Qt::transparent);
        ui->tempLabel->setPixmap(*tempPixmap);
        ui->label->setPixmap(*qPixmap);
        selectedX1 = selectedY1 = -1; //clear to -1
    }
    ui->actionPointerIcon->setChecked(false);
    ui->actionLineIcon->setChecked(false);
    ui->actionEllipseIcon->setChecked(false);
    ui->actionPolygonIcon->setChecked(false);
    ui->actionScaleIcon->setChecked(false);
    ui->actionRotateIcon->setChecked(false);
    selectedDrawEvent = TYPE_NOTHING;
}

void MainWindow::on_actionPointerIcon_triggered()
{
    cancelSelectedIcon();
    selectedDrawEvent = TYPE_NOTHING;
}

void MainWindow::on_actionLineIcon_triggered()
{
    cancelSelectedIcon();
    ui->actionLineIcon->setChecked(true);
    selectedDrawEvent = TYPE_LINE;
}

void MainWindow::on_actionEllipseIcon_triggered()
{
    cancelSelectedIcon();
    ui->actionEllipseIcon->setChecked(true);
    selectedDrawEvent = TYPE_ELLIPSE;
}

void MainWindow::on_actionPolygonIcon_triggered()
{
    cancelSelectedIcon();
    ui->actionPolygonIcon->setChecked(true);
    selectedDrawEvent = TYPE_POLYGON;
    selectedX1 = selectedY1 = -1;
}

void MainWindow::on_actionScaleIcon_triggered()
{
    cancelSelectedIcon();
    ui->actionScaleIcon->setChecked(true);
    selectedDrawEvent = TYPE_SCALE;
}

void MainWindow::on_actionRotateIcon_triggered()
{
    cancelSelectedIcon();
    ui->actionRotateIcon->setChecked(true);
    selectedDrawEvent = TYPE_ROTATE;
}

int MainWindow::saveDragGraphicUnit(int x0, int y0, int x1, int y1, int type, int id)
{
    int minn = 0;
    int newId;
    if(type == TYPE_POLYGON)
    {
        if(id == 0)
        {
            for(int i = 0; i < v.size(); i++) if(v[i].id < minn) minn = v[i].id; //the Min ID of the Graphic Unit.
            newId = minn - 1;
            GraphicUnit g;
            g.id = newId;
            g.para.push_back(2);
            g.para.push_back(x0);
            g.para.push_back(y0);
            g.para.push_back(x1);
            g.para.push_back(y1);
            g.color.r = (char)((qColor->red())&(0xff));
            g.color.g = (char)((qColor->green())&(0xff));
            g.color.b = (char)((qColor->blue())&(0xff));
            g.type = type;
            v.push_back(g);
        }
        else
        {
            for(int i = 0; i < v.size(); i++)
            {
                if(v[i].id == id)
                {
                    v[i].para.push_back(x0);
                    v[i].para.push_back(y0);
                    v[i].para[0] += 1;
                    newId = id;
                    break;
                }
            }
        }
    }
    else
    {
        for(int i = 0; i < v.size(); i++) if(v[i].id < minn) minn = v[i].id; //the Min ID of the Graphic Unit.
        newId = minn - 1;
        GraphicUnit g;
        g.id = newId;
        g.para.push_back(x0);
        g.para.push_back(y0);
        g.para.push_back(x1);
        g.para.push_back(y1);
        g.color.r = (char)((qColor->red())&(0xff));
        g.color.g = (char)((qColor->green())&(0xff));
        g.color.b = (char)((qColor->blue())&(0xff));
        g.type = type;
        v.push_back(g);
    }
    return newId;
}

int MainWindow::selectGraphicUnit(int nx, int ny)
{
    float disLimit = 10.0f;
    int newId = -1;
    int sz = 0;
    float minDis = 1000.0f;
    for(int i = 0; i < v.size(); i++)
    {
        float tempDis = 1000.0f;
        switch (v[i].type) {
        case TYPE_LINE:
            qDebug() << "x0:" << v[i].para[0];
            qDebug() << "y0:" << v[i].para[1];
            qDebug() << "x1:" << v[i].para[2];
            qDebug() << "y1:" << v[i].para[3];
            qDebug() << "nx:" << nx;
            qDebug() << "ny:" << ny;
            int x0 = v[i].para[0], y0 = v[i].para[1], x1 = v[i].para[2], y1 = v[i].para[3];
            tempDis = abs(((x1-x0)*(ny-y0)-(y1-y0)*(nx-x0))/sqrt((x1-x0)*(x1-x0)));
            qDebug() << "tempDis Line: " << tempDis;
            break;
        case TYPE_ELLIPSE:
            tempDis = sqrt(abs(v[i].para[3]*v[i].para[3]*(nx-v[i].para[0])*(nx-v[i].para[0])+v[i].para[2]*v[i].para[2]*(ny-v[i].para[1])*(ny-v[i].para[1])-v[i].para[2]*v[i].para[2]*v[i].para[3]*v[i].para[3]));
            qDebug() << "tempDis Ellipse: " << tempDis;
            break;
        case TYPE_POLYGON:
            sz = v[i].para[0];
            for(int j = 0; j < sz; j++)
            {
                int x0 = v[i].para[(j*2)%sz+1];
                int y0 = v[i].para[(j*2+1)%sz+1];
                int x1 = v[i].para[(j*2+2)%sz+1];
                int y1 = v[i].para[(j*2+3)%sz+1];
                tempDis = abs((x1-x0)*ny-(y1-y0)*nx+(y1-y0)*x0+(x1-x0)*y0);
                if(tempDis < minDis) {
                    minDis = tempDis;
                    newId = v[i].id;
                    selectedIndex = i;
                }
            }
            break;
        default:
            break;
        }
        if(tempDis < minDis)
        {
            minDis = tempDis;
            newId = v[i].id;
            selectedIndex = i;
        }
    }
    if(minDis < 100) return newId;
    else return 0;
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    qDebug() << "selectedID: " << selectedId;
    //qDebug() << "x:" << e->x() << " y:" << e->y();
    int relativeX = e->x() - baseX;
    int relativeY = e->y() - baseY;
    if(relativeX < 0 || relativeY < 0 || relativeX > newCanvasWidth || relativeY > newCanvasHeight) return;
    if(e->button() == Qt::RightButton)
    {
        centralX = relativeX;
        centralY = relativeY;
        return;
    }
    else
        centralX = centralY = -1;
    if(selectedDrawEvent == TYPE_NOTHING) return;
    if(clickTimes == 0)
    {
        clickTimes = 1;
        if(selectedDrawEvent == TYPE_POLYGON && selectedX1 != -1 && selectedY1 != -1)
        {
            //selectedX1 and Y1 are set to -1 when
            //clicking the button drawPolygonIcon or finishing drawing a Polygon
            allocatedId = 0;
            selectedX0 = selectedX1;
            selectedY0 = selectedY1;
        }
        else {
            selectedX0 = relativeX;
            selectedY0 = relativeY;
            startX = selectedX0;
            startY = selectedY0;
        }
        selectedId = selectGraphicUnit(relativeX, relativeY);
     }
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *e)
{
    qDebug() << "double click";
    int relativeX = e->x() - baseX;
    int relativeY = e->y() - baseY;
    if(relativeX < 0 || relativeY < 0 || relativeX > newCanvasWidth || relativeY > newCanvasHeight) return;
    if(selectedDrawEvent != TYPE_POLYGON) return;
    selectedX1 = relativeX;
    selectedY1 = relativeY;
    drawLineBresenham((float)startX, (float)startY, (float)selectedX1, (float)selectedY1, qPainter);
    tempPixmap->fill(Qt::transparent);
    ui->tempLabel->setPixmap(*tempPixmap);
    ui->label->setPixmap(*qPixmap);
    selectedX1 = selectedY1 = -1; //clear to -1
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    int relativeX = e->x() - baseX;
    int relativeY = e->y() - baseY;
    if(relativeX < 0 || relativeY < 0 || relativeX > newCanvasWidth || relativeY > newCanvasHeight) return;
    if(selectedDrawEvent == TYPE_NOTHING) return;
    if(clickTimes == 0) return;
    else
    {
        selectedX1 = relativeX;
        selectedY1 = relativeY;
        tempPixmap->fill(Qt::transparent);
        ui->tempLabel->setPixmap(*tempPixmap);
        if(selectedDrawEvent == TYPE_LINE)
            drawLineBresenham((float)selectedX0, (float)selectedY0, (float)selectedX1, (float)selectedY1, tempPainter);
        if(selectedDrawEvent == TYPE_ELLIPSE)
            drawEllipse((selectedX0+selectedX1)/2,(selectedY0+selectedY1)/2,abs(selectedX1-selectedX0)/2,abs(selectedY1-selectedY0)/2,tempPainter);
        if(selectedDrawEvent == TYPE_POLYGON)
            drawLineBresenham((float)selectedX0, (float)selectedY0, (float)selectedX1, (float)selectedY1, tempPainter);
        if(selectedDrawEvent == TYPE_SCALE && selectedId != 0)
        {
            if(v[selectedIndex].type == TYPE_ELLIPSE)
                doScale(selectedId, centralX, centralY, (float)sqrt(((selectedX1-v[selectedIndex].para[0])*(selectedX1-v[selectedIndex].para[0])+(selectedY1-v[selectedIndex].para[1])*(selectedY1-v[selectedIndex].para[1]))/((selectedX0-v[selectedIndex].para[0])*(selectedX0-v[selectedIndex].para[0])+(selectedY0-v[selectedIndex].para[1])*(selectedY0-v[selectedIndex].para[1]))), tempPainter);
        }
        //center
        if(centralX != -1 && centralY != -1) tempPainter->drawPoint(centralX, centralY);
        ui->tempLabel->setPixmap(*tempPixmap);
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    int relativeX = e->x() - baseX;
    int relativeY = e->y() - baseY;
    if(relativeX < 0 || relativeY < 0 || relativeX > newCanvasWidth || relativeY > newCanvasHeight) return;
    if(clickTimes == 1)
    {
        clickTimes = 0;
        selectedX1 = relativeX;
        selectedY1 = relativeY;
        if(selectedDrawEvent == TYPE_LINE) /***Remember to clear allocatedId.***/
        {
            allocatedId = saveDragGraphicUnit(selectedX0, selectedY0, selectedX1, selectedY1, TYPE_LINE, 0);
            allocatedId = 0;
            drawLineBresenham((float)selectedX0, (float)selectedY0, (float)selectedX1, (float)selectedY1, qPainter);
        }
        if(selectedDrawEvent == TYPE_ELLIPSE)
        {
            allocatedId = saveDragGraphicUnit((selectedX0+selectedY0)/2, (selectedX1+selectedY1)/2, abs(selectedX1-selectedX0)/2, abs(selectedY1-selectedY0)/2, TYPE_ELLIPSE, 0);
            allocatedId = 0;
            drawEllipse((selectedX0+selectedX1)/2,(selectedY0+selectedY1)/2,abs(selectedX1-selectedX0)/2,abs(selectedY1-selectedY0)/2,qPainter);
        }
        if(selectedDrawEvent == TYPE_POLYGON)
        {
            allocatedId = saveDragGraphicUnit(selectedX0, selectedY0, selectedX1, selectedY1, TYPE_POLYGON, allocatedId);
            drawLineBresenham((float)selectedX0, (float)selectedY0, (float)selectedX1, (float)selectedY1, qPainter);
        }
        if(selectedDrawEvent == TYPE_SCALE && selectedId != 0)
        {
            if(v[selectedIndex].type == TYPE_ELLIPSE)
                doScale(selectedId, v[selectedIndex].para[0], v[selectedIndex].para[1], (float)sqrt(((selectedX1-v[selectedIndex].para[0])*(selectedX1-v[selectedIndex].para[0])+(selectedY1-v[selectedIndex].para[1])*(selectedY1-v[selectedIndex].para[1]))/((selectedX0-v[selectedIndex].para[0])*(selectedX0-v[selectedIndex].para[0])+(selectedY0-v[selectedIndex].para[1])*(selectedY0-v[selectedIndex].para[1]))),qPainter);
        }
        tempPixmap->fill(Qt::transparent);
        ui->tempLabel->setPixmap(*tempPixmap);
        ui->label->setPixmap(*qPixmap);
    }
}

void MainWindow::on_actionTranslate_triggered()
{
    translateDialog->show();
}

void MainWindow::onReceive_Translate(int id ,int dx, int dy)
{
    doTranslate(id, dx, dy, qPainter);
}

void MainWindow::doTranslate(int id, int x, int y, QPainter *thisPainter)
{
    int index = -1;
    for(int i = 0; i < v.size(); i++) if(v[i].id == id) {index = i; break;}
    if(index == -1)
    {
        QMessageBox::warning(this, "ERROR", QString("No such a graphic unit of ") + QString::number(id) + QString("."));
        return;
    }

    createTempPixmapExceptId(id);
    int tempR = qColor->red(), tempG = qColor->green(), tempB = qColor->blue();
    setColor(v[index].color.r&0x000000ff, v[index].color.g&0x000000ff, v[index].color.b&0x000000ff, qPainter);

    switch (v[index].type) {
    case TYPE_LINE:
        v[index].para[0] += x; v[index].para[1] += y; v[index].para[2] += x; v[index].para[3] += y;
        drawLineBresenham(v[index].para[0],v[index].para[1],v[index].para[2],v[index].para[3], thisPainter);
        break;
    case TYPE_ELLIPSE:
        v[index].para[0] += x; v[index].para[1] += y;
        drawEllipse(v[index].para[0],v[index].para[1],v[index].para[2],v[index].para[3], thisPainter);
        break;
    case TYPE_POLYGON:
        for(int i = 0; i < v[index].para[0]; i++)
        {
            v[index].para[i*2 + 1] += x;
            v[index].para[i*2 + 2] += y;
        }
        drawPolygon(v[index].para, 1, thisPainter);
        break;
    case TYPE_CURVE:
        break;
    default:
        break;
    }
    ui->label->setPixmap(*qPixmap);
    setColor(tempR, tempG, tempB, qPainter);
}

void MainWindow::on_actionRotate_triggered()
{
    rotateDialog->show();
}

void MainWindow::onReceive_Rotate(int id, int cx, int cy, int angle)
{
    doRotate(id, cx, cy, angle, qPainter);
}

void MainWindow::doRotate(int id, int cx, int cy, int angle, QPainter *thisPainter)
{
    int index = -1;
    for(int i = 0; i < v.size(); i++) if(v[i].id == id) {index = i; break;}
    if(index == -1)
    {
        QMessageBox::warning(this, "Error", QString("No such a graphic unit of ") + QString::number(id) + QString("."));
        return;
    }

    createTempPixmapExceptId(id);
    int tempR = qColor->red(), tempG = qColor->green(), tempB = qColor->blue();
    setColor(v[index].color.r&0x000000ff, v[index].color.g&0x000000ff, v[index].color.b&0x000000ff, qPainter);

    float pi = 3.1415926535f;
    float COS = cos((float)angle*pi/180);
    float SIN = sin((float)angle*pi/180);
    switch (v[index].type) {
    case TYPE_LINE:
        for(int i = 0; i < 2; i++)
        {
            int originX = v[index].para[i*2], originY = v[index].para[i*2+1]; //!!!!!!!!!!!!!!!!!!!!!!!!!!!
            v[index].para[i*2] = (int)((float)cx + (float)(originX - cx)*COS - (float)(originY - cy)*SIN + 0.5f);
            v[index].para[i*2+1] = (int)((float)cy + (float)(originX - cx)*SIN - (float)(originY - cy)*COS + 0.5f);
        }
        drawLineBresenham(v[index].para[0],v[index].para[1],v[index].para[2],v[index].para[3], thisPainter);
        break;
    case TYPE_ELLIPSE:
        for(int i = 0; i < 2; i++)
        {
            int originX = v[index].para[i*2], originY = v[index].para[i*2+1];
            v[index].para[i*2] = (int)((float)cx + (float)(originX - cx)*COS - (float)(originY - cy)*SIN + 0.5f);
            v[index].para[i*2+1] = (int)((float)cy + (float)(originX - cx)*SIN - (float)(originY - cy)*COS + 0.5f);
        }
        drawEllipse(v[index].para[0],v[index].para[1],v[index].para[2],v[index].para[3], thisPainter);
        break;
    case TYPE_POLYGON:
        for(int i = 0; i < v[index].para[0]; i++)
        {
            int originX = v[index].para[i*2 + 1], originY = v[index].para[i*2 + 2];
            v[index].para[i*2 + 1] = (int)((float)cx + (float)(originX - cx)*COS - (float)(originY - cy)*SIN + 0.5f);
            v[index].para[i*2 + 2] = (int)((float)cy + (float)(originX - cx)*SIN + (float)(originY - cy)*COS + 0.5f);
            qDebug() << "sin:" << sin((float)angle*pi/180) << " cos:" << cos((float)angle*pi/180);
            qDebug() << "x:" << v[index].para[i*2 + 1] << " y:" << v[index].para[i*2 + 2];
        }
        drawPolygon(v[index].para, 1, thisPainter);
        break;
    case TYPE_CURVE:
        break;
    default:
        break;
    }
    ui->label->setPixmap(*qPixmap);
    setColor(tempR, tempG, tempB, qPainter);
}

void MainWindow::on_actionScale_triggered()
{
    scaleDialog->show();
}

void MainWindow::onReceive_Scale(int id, int cx, int cy, float scale)
{
    doScale(id, cx, cy, scale, qPainter);
}

void MainWindow::doScale(int id, int cx, int cy, float scale, QPainter *thisPainter)
{
    int index = -1;
    for(int i = 0; i < v.size(); i++) if(v[i].id == id) {index = i; break;}
    if(index == -1)
    {
        QMessageBox::warning(this, "Error", QString("No such a graphic unit of ") + QString::number(id) + QString("."));
        return;
    }

    createTempPixmapExceptId(id);
    int tempR = qColor->red(), tempG = qColor->green(), tempB = qColor->blue();
    setColor(v[index].color.r&0x000000ff, v[index].color.g&0x000000ff, v[index].color.b&0x000000ff, qPainter);

    switch (v[index].type) {
    case TYPE_LINE:
        for(int i = 0; i < 2; i++)
        {
            int originX = v[index].para[i*2], originY = v[index].para[i*2+1];
            v[index].para[i*2] = (int)((float)(originX - cx)*scale + (float)cx + 0.5f);
            v[index].para[i*2+1] = (int)((float)(originY - cy)*scale + (float)cy + 0.5f);
        }
        drawLineBresenham(v[index].para[0],v[index].para[1],v[index].para[2],v[index].para[3], thisPainter);
        break;
    case TYPE_ELLIPSE:
        for(int i = 0; i < 2; i++)
        {
            int originX = v[index].para[i*2], originY = v[index].para[i*2+1];
            v[index].para[i*2] = (int)((float)(originX - cx)*scale + (float)cx + 0.5f);
            v[index].para[i*2+1] = (int)((float)(originY - cy)*scale + (float)cy + 0.5f);
        }
        drawEllipse(v[index].para[0],v[index].para[1],v[index].para[2],v[index].para[3], thisPainter);
        break;
    case TYPE_POLYGON:
        for(int i = 0; i < v[index].para[0]; i++)
        {
            int originX = v[index].para[i*2 + 1], originY = v[index].para[i*2 + 2];
            v[index].para[i*2+1] = (int)((float)(originX - cx)*scale + (float)cx + 0.5f);
            v[index].para[i*2+2] = (int)((float)(originY - cy)*scale + (float)cy + 0.5f);
        }
        drawPolygon(v[index].para, 1, thisPainter);
        break;
    case TYPE_CURVE:
        break;
    default:
        break;
    }
    ui->label->setPixmap(*qPixmap);
    setColor(tempR, tempG, tempB, qPainter);
}

void MainWindow::on_actionClip_triggered()
{
    clipDialog->show();
}

void MainWindow::onReceive_Clip(int id, int x1, int y1, int x2, int y2, int type)
{
    if(type == 0) doClipCohenSutherland(id, x1, y1, x2, y2, qPainter);
    if(type == 1) doClipLiangBarsky(id, x1, y1, x2, y2, qPainter);
}

void MainWindow::doClipCohenSutherland(int id, int x1, int y1, int x2, int y2, QPainter *thisPainter)
{
    int index = -1;
    for(int i = 0; i < v.size(); i++) if(v[i].id == id) {index = i; break;}
    if(index == -1 || v[index].type != TYPE_LINE)
    {
        QMessageBox::warning(this, "ERROR", tr("this graphic unit doesn't exist or isn't a Line."));
        return;
    }


    createTempPixmapExceptId(id);
    int tempR = qColor->red(), tempG = qColor->green(), tempB = qColor->blue();
    setColor(v[index].color.r&0x000000ff, v[index].color.g&0x000000ff, v[index].color.b&0x000000ff, qPainter);

    int endCode1 = 0, endCode2 = 0;
    int endX1 = v[index].para[0], endY1 = v[index].para[1], endX2 = v[index].para[2], endY2 = v[index].para[3];
    if(endX1 < x1) endCode1 += 1; //left
    if(endX1 > x2) endCode1 += 2; //right
    if(endY1 < y1) endCode1 += 4; //down
    if(endY1 > y2) endCode1 += 8; //up
    if(endX2 < x1) endCode2 += 1; //left
    if(endX2 > x2) endCode2 += 2; //right
    if(endY2 < y1) endCode2 += 4; //down
    if(endY2 > y2) endCode2 += 8; //up

    if((endCode1|endCode2) == 0) //inside area
    {
        drawLineBresenham(endX1, endY1, endX2, endY2, qPainter);
    }
    else if((endCode1&endCode2) != 0) //complete outside area
    {
        v[index].para[0] = v[index].para[1] = v[index].para[2] = v[index].para[3] = 0;
    }
    else
    {
        int orCode = endCode1 | endCode2;
        if(orCode & 1) //left
        {
            int newY = (int)((float)(x1 - endX2)*(endY1 - endY2)/(endX1 - endX2) + (float)endY2 + 0.5f);
            if(endX1 < x1) {endX1 = x1; endY1 = newY;}
            else if(endX2 < x1) {endX2 = x1; endY2 = newY;}
        }
        if(orCode & 2) //right
        {
            int newY = (int)((float)(x2 - endX2)*(endY1 - endY2)/(endX1 - endX2) + (float)endY2 + 0.5f);
            if(endX1 > x2) {endX1 = x2; endY1 = newY;}
            else if(endX2 > x2) {endX2 = x2; endY2 = newY;}
        }
        if(orCode & 4) //down
        {
            int newX = (int)((float)(y1 - endY2)*(endX1 - endX2)/(endY1 - endY2) + (float)endX2 + 0.5f);
            if(endY1 < y1) {endX1 = newX; endY1 = y1;}
            else if(endY2 < y1) {endX2 = newX; endY2 = y1;}
        }
        if(orCode & 8) //up
        {
            int newX = (int)((float)(y2 - endY2)*(endX1 - endX2)/(endY1 - endY2) + (float)endX2 + 0.5f);
            if(endY1 > y2) {endX1 = newX; endY1 = y2;}
            else if(endY2 > y2) {endX2 = newX; endY2 = y2;}
        }
        //qDebug() << "(x1,y1):" << x1 << "," << y2 << " (x2,y2):" << x2 << "," << y2;
        //qDebug() << "endX1:" << endX1 << " endY1:" << endY1;
        //qDebug() << "endX2:" << endX2 << " endY2:" << endY2;
        v[index].para[0] = endX1;
        v[index].para[1] = endY1;
        v[index].para[2] = endX2;
        v[index].para[3] = endY2;
        drawLineBresenham(endX1, endY1, endX2, endY2, qPainter);
    }

    ui->label->setPixmap(*qPixmap);
    setColor(tempR, tempG, tempB, qPainter);
}

void MainWindow::doClipLiangBarsky(int id, int x1, int y1, int x2, int y2, QPainter *thisPainter)
{
    int index = -1;
    for(int i = 0; i < v.size(); i++) if(v[i].id == id) {index = i; break;}
    if(index == -1 || v[index].type != TYPE_LINE)
    {
        QMessageBox::warning(this, "ERROR", tr("this graphic unit doesn't exist or isn't a Line."));
        return;
    }


    createTempPixmapExceptId(id);
    int tempR = qColor->red(), tempG = qColor->green(), tempB = qColor->blue();
    setColor(v[index].color.r&0x000000ff, v[index].color.g&0x000000ff, v[index].color.b&0x000000ff, qPainter);

    int xs = v[index].para[0];
    int ys = v[index].para[1];
    int xt = v[index].para[2];
    int yt = v[index].para[3];
    int p[4], q[4];
    float u1 = 0, u2 = 1;
    p[0] = xs - xt;
    p[1] = xt - xs;
    p[2] = ys - yt;
    p[3] = yt - ys;
    q[0] = xs - x1;
    q[1] = x2 - xs;
    q[2] = ys - y1;
    q[3] = y2 - ys;
    bool flag = false;
    for(int i = 0; i < 4; i++)
    {
        if(p[i] == 0 && q[i] < 0)
        {
            flag = true;
        }
        else
        {
            float r = (float) q[i] / p[i];
            if(p[i] < 0)
            {
                u1 = (u1>r)?(u1):(r);
            }
            else
            {
                u2 = (u2<r)?(u2):(r);
            }
            if(u1 > u2) flag = true;
        }
    }
    if(flag == false)
    {
        v[index].para[0] = xs + u1*(xt - xs);
        v[index].para[1] = ys + u1*(yt - ys);
        v[index].para[2] = xs + u2*(xt - xs);
        v[index].para[3] = ys + u2*(yt - ys);
    }
    else
    {
        v[index].para[0] = 0;
        v[index].para[1] = 0;
        v[index].para[2] = 0;
        v[index].para[3] = 0;
    }

    //qDebug() << v[index].para[0] << " " << v[index].para[1];
    //qDebug() << v[index].para[2] << " " << v[index].para[3];
    drawLineBresenham(v[index].para[0], v[index].para[1], v[index].para[2], v[index].para[3], qPainter);

    ui->label->setPixmap(*qPixmap);
    setColor(tempR, tempG, tempB, qPainter);
}

void MainWindow::on_actionCurve_triggered()
{
    curveDialog->show();
}

void MainWindow::onReceive_DrawCurve(int id, int n, vector<int> tempV, int type)
{
    if(isIdExist(id)) {
        QMessageBox::warning(this, "ERROR", tr("Curve drawing error! The ID is repeated."));
        return;
    }
    GraphicUnit g;
    g.id = id;
    g.para.push_back(n);
    for(int i = 0; i < tempV.size(); i++) g.para.push_back(tempV[i]);
    g.type = TYPE_CURVE;
    g.color.r = (char)((qColor->red())&0xff);
    g.color.g = (char)((qColor->green())&0xff);
    g.color.b = (char)((qColor->blue())&0xff);
    v.push_back(g);
    if(type == 0)
    {
        drawCurveBspline(g.para, qPainter);
    }
    else if(type == 1)
    {
        drawCurveBezier(g.para, qPainter);
    }
    ui->label->setPixmap(*qPixmap);
}

void MainWindow::drawCurveBspline(vector<int> vec, QPainter *thisPainter)
{
    /*****
     * P(t)=1/6(-t^3+3t^2-3t+1)P0+1/6(3t^3-6t^2+4)P1+1/6(-3t^3+3t^2+3t+1)P2+1/6t^3P3
     * =(-1/6P0+1/2P1-1/2P2+1/6P3)t^3+(1/2P0-P1+1/2P2)t^2+(-1/2P0+1/2P2)t+(1/6P0+2/3P1+1/6P2)
    *****/
     int n = vec[0];
    for(int i = 1; i <= n-3; i++)
    {
        int x0 = vec[i*2-1], y0 = vec[i*2];
        int x1 = vec[i*2+1], y1 = vec[i*2+2];
        int x2 = vec[i*2+3], y2 = vec[i*2+4];
        int x3 = vec[i*2+5], y3 = vec[i*2+6];
        double a3 = (double)-x0/6+x1/2-x2/2+x3/6, b3 = (double)-y0/6+y1/2-y2/2+y3/6;
        double a2 = (double)x0/2-x1+x2/2, b2 = (double)y0/2-y1+y2/2;
        double a1 = (double)-x0/2+x2/2, b1 = (double)-y0/2+y2/2;
        double a0 = (double)x0/6+2*x1/3+x2/6, b0 = (double)y0/6+2*y1/3+y2/6;
        for(int j = 0; j <= 50; j++)
        {
            double t = (double)j/50;
            double xj = a3*t*t*t+a2*t*t+a1*t+a0;
            double yj = b3*t*t*t+b2*t*t+b1*t+b0;
            thisPainter->drawPoint((int)xj,(int)xj);
        }
    }
}

MainWindow::Point MainWindow::getFinalPoint(double t, int n)
{
    if(n == 1)
    {
        //qDebug() << " ";
        return workPoint[0];
    }
    else
    {
        for(int i = 0; i < n-1; i++)
        {
            workPoint[i].x = workPoint[i].x*(1-t) + workPoint[i+1].x*t;
            workPoint[i].y = workPoint[i].y*(1-t) + workPoint[i+1].y*t;
            //qDebug() << n << ",workPoint[" << i << "]: " << workPoint[i].x << " " << workPoint[i].y;
        }
        return getFinalPoint(t, n-1);
    }
}

void MainWindow::drawCurveBezier(vector<int> v, QPainter *thisPainter)
{
    int n = v[0];
    controlPoint = new Point[n];
    workPoint = new Point[n];
    for(int i = 0; i < n; i++)
    {
        controlPoint[i].x = (double)v[i*2+1];
        controlPoint[i].y = (double)v[i*2+2];
        //qDebug() << "controlPoint: " << controlPoint[i].x << " " << controlPoint[i].y;
    }
    int d = 255;
    for(int i = 0; i <= d; i++)
    {
        for(int j = 0; j < n; j++) workPoint[j] = controlPoint[j];
        double t = (double)i/d;
        Point finalPoint = getFinalPoint(t, n);
        thisPainter->drawPoint((int)finalPoint.x, (int)finalPoint.y);
    }
    delete []controlPoint;
    delete []workPoint;
}
