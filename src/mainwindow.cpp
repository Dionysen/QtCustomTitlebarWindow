#include "mainwindow.h"
#include "qboxlayout.h"
#include "qevent.h"
#include "qicon.h"
#include "qlabel.h"
#include "qlayout.h"
#include "qlayoutitem.h"
#include "qmainwindow.h"
#include "qnamespace.h"
#include "qobject.h"
#include "qpushbutton.h"
#include <QSpacerItem>
#include <QSizePolicy>
#include "qsizepolicy.h"
#include "qwidget.h"
#include <QApplication>
#include <QPainter>
#include <QStyle>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_ButtonWidth(50)
    , m_TitleHeight(36)
    , m_borderWidth(5)
    , m_isResizing(false)
{
    // 初始化指针，设置布局
    InitWindow();
    // 连接按钮到对应功能
    connectBtns();
}

void MainWindow::InitWindow()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    resize(800, 600);
    // 标题栏
    p_TitleBar      = new QWidget(this);
    p_CentralWidget = new QWidget(this);
    p_MainWidget    = new QWidget(this);

    p_CentralWidget->setToolTip("CentralWidget");
    p_CentralWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    p_CentralWidget->setStyleSheet(QString("background-color: gray"));
    // 三个按钮
    p_MinimumBtn = new QPushButton(p_TitleBar);
    p_MaximumBtn = new QPushButton(p_TitleBar);
    p_CloseBtn   = new QPushButton(p_TitleBar);

    updateTitle();

    // 标题和图标
    p_TitleIcon = new QLabel(p_TitleBar);
    p_TitleText = new QLabel(p_TitleBar);
    p_TitleText->setText("Title");
    // 布局
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
    // p_TitleBar->setStyleSheet("QWidget { background-color: #1f2020; color: #000000;}");

    p_MainLayout = new QVBoxLayout;
    p_MainLayout->setContentsMargins(0, 0, 0, 0);
    p_MainLayout->setSpacing(0);
    p_MainLayout->addWidget(p_TitleBar);
    p_MainLayout->addWidget(p_CentralWidget);

    p_MainWidget->setLayout(p_MainLayout);
    this->setCentralWidget(p_MainWidget);

    this->setMouseTracking(true);
    setAllChildrenMouseTracking(this);
}

void MainWindow::setAllChildrenMouseTracking(QWidget* parent)
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


void MainWindow::connectBtns()
{
    p_MinimumBtn->setStyleSheet("QPushButton { border: none; }");
    p_MaximumBtn->setStyleSheet("QPushButton { border: none; }");
    p_CloseBtn->setStyleSheet("QPushButton { border: none; }");

    p_MinimumBtn->setIcon(QIcon(":/icon/icon_window_minimize"));
    p_CloseBtn->setIcon(QIcon(":/icon/icon_window_close"));
    p_MaximumBtn->setIcon(QIcon(":/icon/icon_window_maximize"));

    connect(p_CloseBtn, &QPushButton::clicked, this, [this]() { this->close(); });
    connect(p_MinimumBtn, &QPushButton::clicked, this, [this]() { this->showMinimized(); });
    connect(p_MaximumBtn, &QPushButton::clicked, this, [this]() { this->toggleMaximize(); });
}

void MainWindow::setTitleBarHeight(const int& height)
{
    m_TitleHeight = height;
    updateTitle();
}

void MainWindow::setButtonWidth(const int& width)
{
    m_ButtonWidth = width;
    updateTitle();
}

void MainWindow::updateTitleBarButton()
{
    p_MinimumBtn->setFixedSize(m_ButtonWidth, m_TitleHeight);
    p_MaximumBtn->setFixedSize(m_ButtonWidth, m_TitleHeight);
    p_CloseBtn->setFixedSize(m_ButtonWidth, m_TitleHeight);
}

void MainWindow::updateTitle()
{
    p_TitleBar->setFixedHeight(m_TitleHeight);
    updateTitleBarButton();
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
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
                qDebug() << "LEft";
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

void MainWindow::mousePressEvent(QMouseEvent* event)
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

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    // Titlebar
    m_IsPressedTitleBar = false;
    m_IsDoubleClicked   = false;
    // // Resize
    m_isResizing = false;
    setCursor(Qt::ArrowCursor);
    event->accept();
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (p_TitleBar->underMouse() && !p_MinimumBtn->underMouse() && !p_MaximumBtn->underMouse() && !p_CloseBtn->underMouse())
    {
        m_IsDoubleClicked = true;  // 使用flag防止双击时触发move事件
        toggleMaximize();
    }
}

void MainWindow::toggleMaximize()
{
    if (this->isMaximized())
    {
        this->showNormal();
        p_MaximumBtn->setIcon(QIcon(":/icon/icon_window_maximize"));
    }
    else if (!this->isMaximized())
    {
        this->showMaximized();
        p_MaximumBtn->setIcon(QIcon(":/icon/icon_window_restore"));
    }
}

void MainWindow::paintEvent(QPaintEvent* event)
{
    // QPainter painter(this);
    // painter.setRenderHint(QPainter::Antialiasing);

    // // 自定义阴影颜色和透明度
    // QColor shadowColor(0, 0, 0, 180);  // 调整透明度以增强阴影效果
    // int    shadowWidth = 20;           // 增加阴影宽度

    // // 绘制外部阴影矩形框
    // for (int i = 0; i < shadowWidth; ++i)
    // {
    //     shadowColor.setAlpha(180 * (1.0 - (i / (double)shadowWidth)));  // 阴影透明度逐渐减弱
    //     painter.setPen(shadowColor);
    //     painter.drawRoundedRect(rect().adjusted(i, i, -i, -i), 10, 10);  // 使用圆角矩形绘制阴影
    // }

    // // 继续绘制窗口的其他内容
    // QMainWindow::paintEvent(event);
}

MainWindow::ResizeRegion MainWindow::getResizeRegion(const QPoint& pos)
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

void MainWindow::resizeWindow(const QPoint& globalPos)
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

MainWindow::~MainWindow()
{
}
