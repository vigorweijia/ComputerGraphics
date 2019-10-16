#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QPixmap>

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
    QPixmap pixmap(width, height);
    QPainter painter(&pixmap);
    painter.fillRect(0,0,width,height,Qt::black);
    ui->label->setPixmap(pixmap);
}
