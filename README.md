# QtCustomTitlebarWindow

A C++ Qt-based custom title bar window with a built-in dark style.

![image-20240913122300970](https://cdn.jsdelivr.net/gh/Dionysen/BlogCDN@main/imgimage-20240913122300970.png)

## Requisites

- Qt >= 6.7.2
- xmake >= 2.9.3

## Get Started

### XMake

Add this item to your xmake project.

1. Add to your main `xmake.lua`.

```lua
includes("QtCustomTitlebarWindow")
add_deps("CustomWindow")
-- add resources
add_files("QtCustomTitlebarWindow/assets/images/image.qrc")
add_files("QtCustomTitlebarWindow/assets/qss/qss.qrc")
-- When using xmake to build a library that relies on qt, the .qrc file does not work properly and needs to be added to the current project manually, using a relative path
```

2. Create a custom window, and set you mainwindow to it's content

```cpp
#include "CustomWindow.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    CustomWindow w;
    w.setDarkMode(true); // Set dark style

    MainWindow* mainWindow = new MainWindow;
    w.setCentent(mainWindow); // Set you main window as the content of custom window
    w.show();
    return a.exec();
}
```

3. Run

```bash
xmake b; xmake r
```

If the interface looks strange, check that the included `.qrc` path is correct.

### CMake

Gen the `CMakeLists.txt` in the project's root path. 

```bash
xmake project -k cmake
```

### Visual Studio

```bash
xmake project -k vsxmake
```

Or

```bash
xmake project -k vsxmake2022
```

