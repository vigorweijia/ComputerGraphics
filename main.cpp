#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    if(argc <= 1)
        w.show();
    else
    {
        if(argc != 3)
        {
            qDebug() << "invalid input, please check your parameter";
            return 0;
        }
        w.execNonGui(QString(argv[1]), QString(argv[2]));
        return 0;
    }

    return a.exec();
}
