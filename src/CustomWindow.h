#ifndef CustomWindow_H
#define CustomWindow_H

// DLL Export API
#ifdef _WIN32
#if defined(CTW_IMPORT)
#define EXPORT_API
#elif defined(CTW_EXPORT)
#define EXPORT_API __declspec(dllexport)
#else
#define EXPORT_API __declspec(dllimport)
#endif
#else
#define EXPORT_API
#endif

#include <QEvent>
#include <QPoint>
#include <QPushButton>
#include <QWidget>
#include <QMainWindow>
#include <QLabel>
#include <QHBoxLayout>
#include <QMouseEvent>

class EXPORT_API CustomWindow : public QWidget
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

    // Title Bar
    void setTitleBarHeight(const int& height);
    void setButtonWidth(const int& width);

    // Title and Icon
    void setWindowTitle(const QString& title);
    void setWindowIcon(const QIcon& icon);

    // DarkMode
    void setDarkMode(bool isDark);

    // Window Border
    void setEnableWindowBorder(bool isBorder);
    bool isEnableWindowBorder();
    void setWindowBorderStyle(uint32_t width, QColor color);

    // Override centralWidget related
    void setCentent(QWidget* widget)
    {
        p_CentralWidget->layout()->addWidget(widget);
    }

  private:
    void InitWindow();
    void connectBtns();
    void updateTitleBarButton();
    void updateTitle();
    void updateWindowBorder(bool isMaximized);

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
    // Main Widget, Title Bar, Central Widget
    QWidget* p_MainWidget;
    QWidget* p_TitleBar;
    QWidget* p_CentralWidget;
    // 三个按钮
    QPushButton* p_MinimumBtn;
    QPushButton* p_MaximumBtn;
    QPushButton* p_CloseBtn;
    // Title and Icon
    QLabel* p_TitleIcon;
    QLabel* p_TitleText;
    // Layout
    QVBoxLayout* p_MainLayout;
    QVBoxLayout* p_CentralLayout;
    QHBoxLayout* p_TitleLayout;
    QVBoxLayout* p_ThisLayout;

  private:
    // Move
    QPoint m_StartMousePos;
    QPoint m_StartWindowPos;
    bool   m_IsPressedTitleBar;
    bool   m_IsDoubleClicked;

    // Resize
    bool         m_isResizing;
    int          m_borderWidth;
    ResizeRegion m_resizeRegion;
    QPoint       m_lastMousePos;

    // DarkMode
    bool m_isDark;

    // Window Border
    int      m_TitleHeight;
    int      m_ButtonWidth;
    bool     m_isEnableWindowBorder;
    QColor   m_windowBorderColor;
    uint32_t m_windowBorderWidth;
};

#endif  // CustomWindow_H
