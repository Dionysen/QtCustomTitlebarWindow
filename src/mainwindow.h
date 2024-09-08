#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qboxlayout.h"
#include "qpoint.h"
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QWidget>
#include <QHBoxLayout>
#include <QMouseEvent>


class MainWindow : public QMainWindow
{
    Q_OBJECT

  protected:
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;

  public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

    void setTitleBarHeight(const int& height);
    void setButtonWidth(const int& width);
    void updateTitleBarButton();
    void updateTitle();

  private:
    void toggleMaximize();

  private:
    // 标题栏
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

    QVBoxLayout* p_MainLayout;
    QHBoxLayout* p_TitleLayout;


  private:
    int m_TitleHeight;
    int m_ButtonWidth;

    // Move
    QPoint m_StartMousePos;
    QPoint m_StartWindowPos;
    bool   m_IsPressedTitleBar;
};

#endif  // MAINWINDOW_H
