#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    newCanvasDialog = new NewCanvasDialog();
    connect(newCanvasDialog, SIGNAL(newCanvasDialogAcceptedEvent(int,int)), this, SLOT(onReceive_NewCanvasDialogAcceptedEvent(int,int)));
}

MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::on_actionSave_As_triggered() {
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
