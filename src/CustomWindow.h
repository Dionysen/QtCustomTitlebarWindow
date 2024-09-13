#ifndef CustomWindow_H
#define CustomWindow_H

#include <QEvent>
#include <QPoint>
#include <QPushButton>
#include <QWidget>
#include <QMainWindow>
#include <QLabel>
#include <QHBoxLayout>
#include <QMouseEvent>

class CustomWindow : public QWidget
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
    explicit CustomWindow(QWidget* parent = 0);
    ~CustomWindow();

    void setTitleBarHeight(const int& height);
    void setButtonWidth(const int& width);

    void setDarkMode(bool isDark);

    // 覆写centralWidget相关
    void setCentent(QWidget* widget)
    {
        p_CentralWidget->layout()->addWidget(widget);
    }

  private:
    void InitWindow();
    void connectBtns();
    void updateTitleBarButton();
    void updateTitle();

    void toggleMaximize();

    CustomWindow::ResizeRegion getResizeRegion(const QPoint& pos);
    void                       resizeWindow(const QPoint& globalPos);

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
    QVBoxLayout* p_CentralLayout;
    QHBoxLayout* p_TitleLayout;
    QVBoxLayout* p_ThisLayout;

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

#endif  // CustomWindow_H
