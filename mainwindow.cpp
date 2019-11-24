#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

//-------------Selected Event-------------------
    selectedDrawEvent = TYPE_NOTHING;
    clickTimes = 0;
//----------------------------------------------
}

MainWindow::~MainWindow()
{
    delete ui;
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
        switch (v[i].type) {
        case TYPE_LINE: drawLineBresenham((float)v[i].para[0],(float)v[i].para[1],(float)v[i].para[2],(float)v[i].para[3],qPainter); break;
        case TYPE_ELLIPSE: drawEllipse(v[i].para[0],v[i].para[1],v[i].para[2],v[i].para[3],qPainter); break;
        case TYPE_POLYGON: drawPolygon(v[i].para, 1, qPainter); break;
        case TYPE_CURVE: break;
        default: break;
        }
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
                qPixmap->save(strList[1]+".bmp");
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
                qDebug() << "no function of drawCurve right now.\n";
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
                qDebug() << "no function of scale right now.\n";
            }
            else if(strList[0].compare(QString("clip")) == 0)
            {
                qDebug() << "no function of clip right now.\n";
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

void MainWindow::on_actionLineIcon_triggered()
{
    if(ui->actionEllipseIcon->isChecked()) ui->actionEllipseIcon->setChecked(false);
    if(ui->actionLineIcon->isChecked() == false) selectedDrawEvent = TYPE_NOTHING;
    else selectedDrawEvent = TYPE_LINE;
}

void MainWindow::on_actionEllipseIcon_triggered()
{
    if(ui->actionLineIcon->isChecked()) ui->actionLineIcon->setChecked(false);
    if(ui->actionEllipseIcon->isChecked() == false) selectedDrawEvent = TYPE_NOTHING;
    else selectedDrawEvent = TYPE_ELLIPSE;
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    //qDebug() << "x:" << e->x() << " y:" << e->y();
    int relativeX = e->x() - baseX;
    int relativeY = e->y() - baseY;
    if(relativeX < 0 || relativeY < 0 || relativeX > newCanvasWidth || relativeY > newCanvasHeight) return;
    if(selectedDrawEvent == TYPE_NOTHING) return;
    if(clickTimes == 0)
    {
        clickTimes = 1;
        selectedX0 = relativeX;
        selectedY0 = relativeY;
    }

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
        if(selectedDrawEvent == TYPE_LINE)
            drawLineBresenham((float)selectedX0, (float)selectedY0, (float)selectedX1, (float)selectedY1, qPainter);
        if(selectedDrawEvent == TYPE_ELLIPSE)
            drawEllipse((selectedX0+selectedX1)/2,(selectedY0+selectedY1)/2,abs(selectedX1-selectedX0)/2,abs(selectedY1-selectedY0)/2,qPainter);
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
}
