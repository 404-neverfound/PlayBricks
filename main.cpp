#include "prewindow.h"
#include <QApplication>
#include "Mythread.h"
#include "audiothread.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    PreWindow w;
    w.show();
    return a.exec();
}
