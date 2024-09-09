#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qboxlayout.h"
#include "qevent.h"
#include "qmainwindow.h"
#include "qpoint.h"
#include "qpushbutton.h"
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QWidget>
#include <QHBoxLayout>
#include <QMouseEvent>


class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum class ResizeRegion
    {
        None = 0,
        Top,
        Bottom,
        Left,
        Right,
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight
    };

  public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

    void setTitleBarHeight(const int& height);
    void setButtonWidth(const int& width);

  private:
    void InitWindow();
    void connectBtns();
    void updateTitleBarButton();
    void updateTitle();

    void toggleMaximize();

    MainWindow::ResizeRegion getResizeRegion(const QPoint& pos);
    void                     resizeWindow(const QPoint& globalPos);

    void setAllChildrenMouseTracking(QWidget* parent);

  protected:
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;

    void paintEvent(QPaintEvent* event) override;

  private:
    // 标题栏和中心部件
    QWidget* p_MainWidget;
    QWidget* p_TitleBar;
    QWidget* p_CentralWidget;
    // 三个按钮
    QPushButton* p_MinimumBtn;
    QPushButton* p_MaximumBtn;
    QPushButton* p_CloseBtn;
    // 标题和图标
    QLabel* p_TitleIcon;
    QLabel* p_TitleText;
    // 布局
    QVBoxLayout* p_MainLayout;
    QHBoxLayout* p_TitleLayout;

  private:
    int m_TitleHeight;
    int m_ButtonWidth;

    // Move
    QPoint m_StartMousePos;
    QPoint m_StartWindowPos;
    bool   m_IsPressedTitleBar;
    bool   m_IsDoubleClicked;

    bool         m_isResizing;
    int          m_borderWidth;
    ResizeRegion m_resizeRegion;
    QPoint       m_lastMousePos;
};

#endif  // MAINWINDOW_H
