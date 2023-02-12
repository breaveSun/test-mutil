#include "mainwindow.h"

#include <QApplication>
#include "qTUIO/include/qtuio.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    QTuio qTUIO(&w);
    qTUIO.run();
    return a.exec();
}
