#include "mainwindow.h"
#include <QApplication>

#include <windows.h>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    AllocConsole();                   // 为应用分配控制台
    freopen("CONOUT$", "w", stdout);  // 重定向 stdout
    freopen("CONOUT$", "w", stderr);  // 重定向 stderr

    MainWindow w;
    w.show();
    return a.exec();
}
