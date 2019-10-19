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
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::isIdExist(int id) {
    for(int i = 0; i < v.size(); i++) {if(v[i].id == id) return false;}
    return true;
}

void MainWindow::on_actionNewCanvas_triggered() {
    newCanvasDialog->show();
}

void MainWindow::onReceive_NewCanvasDialogAcceptedEvent(int width, int height) {
    newCanvasWidth = width;
    newCanvasHeight = height;
    newCanvasDialog->close();
    //ui->textBrowser->setText(QString::number(newCanvasWidth)+","+QString::number(height));
    qPixmap = new QPixmap(width, height);

    //-------------Create the QPainter object----------------
    qPainter = new QPainter(qPixmap);
    qPainter->fillRect(0,0,width,height,Qt::blue);
    //-------------------------------------------------------

    //-------------Reset the size of Window------------------
    if(width > this->width() || height > this->height()) {
        this->resize(width+50,height+50);
    }
    //-------------------------------------------------------

    //-------------Reset the label (Canvas)------------------
    if(width > ui->label->width() || height > ui->label->height()) {
        ui->label->setGeometry(10,10,width,height);
    }
    //-------------------------------------------------------

    ui->label->setPixmap(*qPixmap);
    //qPixmap->save("1.bmp");
}

void MainWindow::on_actionSaveAs_triggered() {
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

void MainWindow::on_actionLine_triggered() {
    lineDialog->show();
}

void MainWindow::onReceive_DrawLine(int id, float x0, float y0, float x1, float y1, int type) {
    for(int i = 0; i < v.size(); i++) {
        if(v[i].id == id) {
            QMessageBox::warning(this, "ERROR", tr("Drawing error! The ID is repeated."));
            return;
        }
    }
    GraphicUnit g;
    g.id = id;
    g.type = TYPE_LINE;
    g.para.push_back(x0);
    g.para.push_back(y0);
    g.para.push_back(x1);
    g.para.push_back(y1);
    v.push_back(g);
    if(type == 1) drawLineDDA(x0, y0, x1, y1);
    else drawLineBresenham(x0, y0, x1, y1);
}

void MainWindow::drawLineDDA(float x0, float y0, float x1, float y1) {

}

void MainWindow::drawLineBresenham(float x0, float y0, float x1, float y1) {

}
