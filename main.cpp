#include "CustomWindow.h"
#include <QApplication>
#include <QFile>
#include <Windows.h>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

#if 1
    AllocConsole();                   // 为应用分配控制台
    freopen("CONOUT$", "w", stdout);  // 重定向 stdout
    freopen("CONOUT$", "w", stderr);  // 重定向 stderr
#endif

    CustomWindow w;
    w.setDarkMode(true);
    w.show();
    return a.exec();
}
