#include "CustomWindow.h"
#include "qlogging.h"

#include <QIcon>
#include <QLayout>
#include <QLayoutItem>
#include <QObject>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QApplication>
#include <QPainter>
#include <QStyle>
#include <QButtonGroup>
#include <QGraphicsDropShadowEffect>
#include <QPainterPath>
#include <QFile>

CustomWindow::CustomWindow(QWidget* parent)
    : QWidget(parent)
    , m_ButtonWidth(50)
    , m_TitleHeight(36)
    , m_borderWidth(5)
    , m_isResizing(false)
    , m_IsPressedTitleBar(false)
{
    // 初始化部件，设置布局
    InitWindow();
    // 连接按钮到对应功能
    connectBtns();
}

void CustomWindow::InitWindow()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->resize(800, 600);
    this->setObjectName("CustomWindow-Background");

    // 标题栏
    p_TitleBar = new QWidget(this);
    p_TitleBar->setObjectName("titleBar");

    // 中心部件
    p_CentralWidget = new QWidget(this);

    // 窗口部件
    p_MainWidget = new QWidget(this);

    // 三个按钮
    p_MinimumBtn = new QPushButton(p_TitleBar);
    p_MaximumBtn = new QPushButton(p_TitleBar);
    p_CloseBtn   = new QPushButton(p_TitleBar);
    p_MinimumBtn->setObjectName("titleBtn");
    p_MaximumBtn->setObjectName("titleBtn");
    p_CloseBtn->setObjectName("titleBtn");
    updateTitle();

    // 标题和图标
    p_TitleIcon = new QLabel(p_TitleBar);
    p_TitleText = new QLabel(p_TitleBar);
    p_TitleText->setText("Title");

    // ------- 布局 --------
    // 标题栏
    p_TitleLayout = new QHBoxLayout;
    p_TitleLayout->addWidget(p_TitleIcon);
    p_TitleLayout->addWidget(p_TitleText);
    p_TitleLayout->addSpacerItem(new QSpacerItem(300, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
    p_TitleLayout->addWidget(p_MinimumBtn);
    p_TitleLayout->addWidget(p_MaximumBtn);
    p_TitleLayout->addWidget(p_CloseBtn);

    p_TitleLayout->setContentsMargins(0, 0, 0, 0);
    p_TitleLayout->setAlignment(Qt::AlignRight);
    p_TitleLayout->setSpacing(0);

    p_TitleBar->setLayout(p_TitleLayout);

    // 中心部件
    // 中心部件保持空白，mainwindow窗口可以放进去
    p_CentralLayout = new QVBoxLayout(this);
    p_CentralLayout->setContentsMargins(0, 0, 0, 0);
    p_CentralLayout->setSpacing(0);
    p_CentralWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    p_CentralWidget->setLayout(p_CentralLayout);

    // 总体部件及其布局
    p_MainLayout = new QVBoxLayout;
    p_MainLayout->setContentsMargins(0, 0, 0, 0);
    p_MainLayout->setSpacing(0);
    p_MainLayout->addWidget(p_TitleBar);
    p_MainLayout->addWidget(p_CentralWidget);
    p_MainWidget->setLayout(p_MainLayout);
    p_MainWidget->setObjectName("MainWidget");

    // 窗口布局
    p_ThisLayout = new QVBoxLayout;
    p_ThisLayout->addWidget(p_MainWidget);
    p_ThisLayout->setContentsMargins(1, 1, 1, 1);
    // p_ThisLayout->setContentsMargins(0, 0, 0, 0);
    p_ThisLayout->setSpacing(0);

    this->setLayout(p_ThisLayout);

    this->setMouseTracking(true);
    setAllChildrenMouseTracking(this);
}

void CustomWindow::setAllChildrenMouseTracking(QWidget* parent)
{
    // 获取所有子对象
    const auto& children = parent->children();

    // 遍历每个子对象
    for (QObject* child : children)
    {
        // 只处理QWidget类型的子对象
        QWidget* widget = qobject_cast<QWidget*>(child);
        if (widget)
        {
            widget->setMouseTracking(true);
            // 递归遍历其子部件
            setAllChildrenMouseTracking(widget);
        }
    }
}


void CustomWindow::connectBtns()
{
    p_MinimumBtn->setIcon(QIcon(":/image/icon_window_minimize_dark"));
    p_CloseBtn->setIcon(QIcon(":/image/icon_window_close_dark"));
    p_MaximumBtn->setIcon(QIcon(":/image/icon_window_maximize_dark"));

    connect(p_CloseBtn, &QPushButton::clicked, this, [this]() { this->close(); });
    connect(p_MinimumBtn, &QPushButton::clicked, this, [this]() { this->showMinimized(); });
    connect(p_MaximumBtn, &QPushButton::clicked, this, [this]() { toggleMaximize(); });
}

void CustomWindow::setTitleBarHeight(const int& height)
{
    m_TitleHeight = height;
    updateTitle();
}

void CustomWindow::setButtonWidth(const int& width)
{
    m_ButtonWidth = width;
    updateTitle();
}

void CustomWindow::updateTitleBarButton()
{
    p_MinimumBtn->setFixedSize(m_ButtonWidth, m_TitleHeight);
    p_MaximumBtn->setFixedSize(m_ButtonWidth, m_TitleHeight);
    p_CloseBtn->setFixedSize(m_ButtonWidth, m_TitleHeight);
}

void CustomWindow::updateTitle()
{
    p_TitleBar->setFixedHeight(m_TitleHeight);
    updateTitleBarButton();
}

void CustomWindow::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() == Qt::LeftButton && !m_IsDoubleClicked && !m_isResizing)
    {
        if (isMaximized() || isMinimized())
        {
            // 计算最大化时鼠标在标题栏的相对位置

            int width            = this->screen()->availableSize().width();
            int titleBeforeWidth = width - m_ButtonWidth * 3;
            int beforeCursorX    = event->pos().x();

            // 计算Normal下鼠标在标题栏的期望相对位置
            int titleAfterWidth = normalGeometry().width() - m_ButtonWidth * 3;
            int afterLenght     = titleAfterWidth * beforeCursorX / titleBeforeWidth;

            // 取消最大化，这一步必须在normalGeometry()之后，因为此函数获取的是另一个状态的窗口Geo，即normal模式下获取的是最大化的Geo
            toggleMaximize();
            this->move(event->globalPosition().x() - afterLenght, this->pos().y());

            m_StartMousePos  = event->globalPosition().toPoint();
            m_StartWindowPos = this->pos();
        }

        if (p_TitleBar->underMouse() && !p_MinimumBtn->underMouse() && !p_MaximumBtn->underMouse() && !p_CloseBtn->underMouse())
        {
            QPoint diffPos = m_StartMousePos - event->globalPosition().toPoint();
            this->move(m_StartWindowPos - diffPos);
        }
    }

    if (!this->isMaximized())
    {
        if (m_isResizing)
        {
            resizeWindow(event->globalPosition().toPoint());
        }
        else
        {
            // 根据鼠标位置设置不同的光标形状
            ResizeRegion region = getResizeRegion(event->pos());
            switch (region)
            {
            case ResizeRegion::Top:
            case ResizeRegion::Bottom:
                setCursor(Qt::SizeVerCursor);
                break;
            case ResizeRegion::Left:
            case ResizeRegion::Right:
                setCursor(Qt::SizeHorCursor);
                break;
            case ResizeRegion::TopLeft:
            case ResizeRegion::BottomRight:
                setCursor(Qt::SizeFDiagCursor);
                break;
            case ResizeRegion::TopRight:
            case ResizeRegion::BottomLeft:
                setCursor(Qt::SizeBDiagCursor);
                break;
            default:
                setCursor(Qt::ArrowCursor);
                break;
            }
        }
    }

    event->accept();
}

void CustomWindow::mousePressEvent(QMouseEvent* event)
{

    if (event->button() == Qt::LeftButton && !m_IsPressedTitleBar)
    {
        m_StartMousePos     = event->globalPosition().toPoint();
        m_StartWindowPos    = this->pos();
        m_IsPressedTitleBar = true;
    }

    if (event->button() == Qt::LeftButton)
    {
        m_resizeRegion = getResizeRegion(event->pos());
        if (m_resizeRegion != ResizeRegion::None)
        {
            m_isResizing   = true;
            m_lastMousePos = event->globalPosition().toPoint();
            event->accept();
        }
    }
}

void CustomWindow::mouseReleaseEvent(QMouseEvent* event)
{
    // Titlebar
    m_IsPressedTitleBar = false;
    m_IsDoubleClicked   = false;
    // // Resize
    m_isResizing = false;
    setCursor(Qt::ArrowCursor);
    event->accept();
}

void CustomWindow::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (p_TitleBar->underMouse() && !p_MinimumBtn->underMouse() && !p_MaximumBtn->underMouse() && !p_CloseBtn->underMouse())
    {
        m_IsDoubleClicked = true;  // 使用flag防止双击时触发move事件
        toggleMaximize();
    }
}

void CustomWindow::toggleMaximize()
{
    if (this->isMaximized())
    {
        this->showNormal();
        p_MaximumBtn->setIcon(QIcon(":/image/icon_window_maximize_dark"));
    }
    else if (!this->isMaximized())
    {
        this->showMaximized();
        p_MaximumBtn->setIcon(QIcon(":/image/icon_window_restore_dark"));
    }
}

void CustomWindow::paintEvent(QPaintEvent* event)
{

    // TODO: Window shadow

    // QPainter painter(this);
    // painter.setRenderHint(QPainter::Antialiasing, true);

    // // 绘制阴影效果
    // QColor shadowColor(0, 0, 0, 50);  // 半透明黑色，模拟macOS阴影效果
    // int    shadowSize = 10;           // 阴影大小为8像素

    // // 绘制矩形的阴影
    // for (int i = 0; i < shadowSize; ++i)
    // {
    //     shadowColor.setAlpha(50 - i * 5);  // 渐变阴影，透明度从100逐渐减少
    //     painter.setPen(Qt::NoPen);
    //     painter.setBrush(shadowColor);
    //     painter.drawRect(i, i, width() - 2 * i, height() - 2 * i);  // 绘制普通矩形阴影
    // }

    // // 绘制窗口本身
    // painter.setBrush(Qt::white);
    // painter.setPen(Qt::NoPen);
    // painter.drawRect(shadowSize, shadowSize, width() - 2 * shadowSize, height() - 2 * shadowSize);  // 绘制普通矩形窗口

    // if (isMaximized())
    // {
    //     return;
    // }
    // int          nShadowsWidth = 10;
    // int          nRadius       = 0;
    // QPainterPath path;
    // path.setFillRule(Qt::WindingFill);
    // path.addRoundedRect(nShadowsWidth, nShadowsWidth, this->width() - nShadowsWidth * 2, this->height() - nShadowsWidth * 2, nRadius, nRadius);
    // QPainter painter(this);
    // QColor   color(0, 0, 0, 200);
    // for (int i = 0; i < nShadowsWidth; i++)
    // {
    //     QPainterPath path;
    //     path.setFillRule(Qt::WindingFill);
    //     path.addRoundedRect(nShadowsWidth - i, nShadowsWidth - i, this->width() - (nShadowsWidth - i) * 2, this->height() - (nShadowsWidth - i) *
    //     2,
    //                         nRadius + i, nRadius + i);
    //     color.setAlpha(100 - qSqrt(i) * 50);
    //     painter.setPen(color);
    //     painter.drawPath(path);
    // }

    // QWidget::paintEvent(event);  // 保持对父类的调用
}


CustomWindow::ResizeRegion CustomWindow::getResizeRegion(const QPoint& pos)
{
    bool onLeft   = pos.x() < m_borderWidth;
    bool onRight  = pos.x() > width() - m_borderWidth;
    bool onTop    = pos.y() < m_borderWidth;
    bool onBottom = pos.y() > height() - m_borderWidth;

    if (onTop && onLeft)
        return ResizeRegion::TopLeft;
    if (onTop && onRight)
        return ResizeRegion::TopRight;
    if (onBottom && onLeft)
        return ResizeRegion::BottomLeft;
    if (onBottom && onRight)
        return ResizeRegion::BottomRight;
    if (onTop)
        return ResizeRegion::Top;
    if (onBottom)
        return ResizeRegion::Bottom;
    if (onLeft)
        return ResizeRegion::Left;
    if (onRight)
        return ResizeRegion::Right;

    return ResizeRegion::None;
}

void CustomWindow::resizeWindow(const QPoint& globalPos)
{
    QRect geom  = geometry();
    int   diffX = globalPos.x() - m_lastMousePos.x();
    int   diffY = globalPos.y() - m_lastMousePos.y();

    switch (m_resizeRegion)
    {
    case ResizeRegion::Top:
        geom.setTop(geom.top() + diffY);
        break;
    case ResizeRegion::Bottom:
        geom.setBottom(geom.bottom() + diffY);
        break;
    case ResizeRegion::Left:
        geom.setLeft(geom.left() + diffX);
        break;
    case ResizeRegion::Right:
        geom.setRight(geom.right() + diffX);
        break;
    case ResizeRegion::TopLeft:
        geom.setTop(geom.top() + diffY);
        geom.setLeft(geom.left() + diffX);
        break;
    case ResizeRegion::TopRight:
        geom.setTop(geom.top() + diffY);
        geom.setRight(geom.right() + diffX);
        break;
    case ResizeRegion::BottomLeft:
        geom.setBottom(geom.bottom() + diffY);
        geom.setLeft(geom.left() + diffX);
        break;
    case ResizeRegion::BottomRight:
        geom.setBottom(geom.bottom() + diffY);
        geom.setRight(geom.right() + diffX);
        break;
    default:
        break;
    }
    setGeometry(geom);
    m_lastMousePos = globalPos;
}

void CustomWindow::setDarkMode(bool isDark)
{
    if (isDark)
    {
        // qDebug() << "qss";
        // QFile file(":/qss/dark.qss");
        // file.open(QFile::ReadOnly);
        // QString styleSheet = file.readAll();
        // qApp->setStyleSheet(styleSheet);

        QFile styleFile(":/qss/dark.qss");
        if (!styleFile.exists())
        {
            qWarning() << "QSS file does not exist!";
        }

        if (styleFile.open(QFile::ReadOnly))
        {
            QString styleSheet = QLatin1String(styleFile.readAll());
            qApp->setStyleSheet(styleSheet);
            styleFile.close();
        }
        else
        {
            qWarning() << "Unable to open QSS file";
        }
    }
    else
    {
        // TODO: Light mode
        qApp->setStyleSheet("");
    }
}

CustomWindow::~CustomWindow()
{
}
