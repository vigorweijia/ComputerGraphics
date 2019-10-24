#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    for(int i = 0; i < v.size(); i++)
    {
        if(v[i].id == id) continue;

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
    tempPainter->fillRect(30,30,40,40,Qt::blue);
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
                qDebug() << "no function of setColor right now\n";
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
                if(strList[0].compare(QString("DDA"))) drawLineDDA((float)x0, (float)y0, (float)x1, (float)y1, qPainter);
                if(strList[0].compare(QString("Bresenham"))) drawLineBresenham((float)x0, (float)y0, (float)x1, (float)y1, qPainter);
                ui->label->setPixmap(*qPixmap);
            }
            else if(strList[0].compare(QString("drawPolygon")) == 0)
            {
                qDebug() << "no function of drawPolygon right now.\n";
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
                drawEllipse(x, y, rx, ry, qPainter);
                ui->label->setPixmap(*qPixmap);
            }
            else if(strList[0].compare(QString("drawCurve")) == 0)
            {
                qDebug() << "no function of drawCurve right now.\n";
            }
            else if(strList[0].compare(QString("translate")) == 0)
            {
                qDebug() << "no function of translate right now.\n";
            }
            else if(strList[0].compare(QString("rotate")) == 0)
            {
                qDebug() << "no function of rotate right now.\n";
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
    g.para.push_back(x0);
    g.para.push_back(y0);
    g.para.push_back(x1);
    g.para.push_back(y1);
    v.push_back(g);
    if(type == 0) drawLineDDA(x0, y0, x1, y1, qPainter);
    else if(type == 1) drawLineBresenham(x0, y0, x1, y1, qPainter);
    ui->label->setPixmap(*qPixmap);
}

void MainWindow::drawLineDDA(float x0, float y0, float x1, float y1, QPainter *thisPainter)
{
    if(x1 - x0 == 0) {QMessageBox::warning(this, "error", tr("vertical!"));return;}
    float m = (y1 - y0)/(x1 - x0);
    if(abs(m) <= 1) {
        if(x1 < x0) {swap(x0,x1); swap(y0,y1);}
        thisPainter->drawPoint((int)x0, (int)y0);
        float yi = y0;
        for(int xi = (int)x0 + 1; xi <= (int)x1; xi++) {
            yi = (float)yi + m;
            thisPainter->drawPoint(xi, (int)yi);
        }
    }
    else {
        if(y1 < y0) {swap(x0,x1); swap(y0,y1);}
        thisPainter->drawPoint((int)x0, (int)y0);
        float xi = x0;
        for(int yi = (int)y0 + 1; yi <= (int)y1; yi++) {
            xi = (float)xi + (1/m);
            thisPainter->drawPoint((int)xi, yi);
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
            if(p > 0)
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
            if(p > 0)
            {
                if(m > 0) xi = xi + 1;
                else xi = xi - 1;
            }
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
        qDebug() << xi << " " << yi << "\n";
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

void MainWindow::onReceive_DrawPolygon(int id, int n, vector<int> v, int type)
{

}
