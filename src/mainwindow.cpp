#include "mainwindow.h"
#include "qboxlayout.h"
#include "qevent.h"
#include "qguiapplication.h"
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

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_ButtonWidth(50)
    , m_TitleHeight(25)
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
    p_MinimumBtn->setText("Min");
    p_MaximumBtn = new QPushButton(p_TitleBar);
    p_MaximumBtn->setText("Max");
    p_CloseBtn = new QPushButton(p_TitleBar);
    p_CloseBtn->setText("Close");

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

    p_MainLayout = new QVBoxLayout;
    p_MainLayout->setContentsMargins(0, 0, 0, 0);
    p_MainLayout->setSpacing(0);
    p_MainLayout->addWidget(p_TitleBar);
    p_MainLayout->addWidget(p_CentralWidget);

    p_MainWidget->setLayout(p_MainLayout);
    this->setCentralWidget(p_MainWidget);

    connect(p_CloseBtn, &QPushButton::clicked, this, [this]() { this->close(); });
    connect(p_MinimumBtn, &QPushButton::clicked, this, [this]() { this->showMinimized(); });
    connect(p_MaximumBtn, &QPushButton::clicked, this, [this]() { this->toggleMaximize(); });
}
void MainWindow::setTitleBarHeight(const int& height)
{
    m_TitleHeight = height;
}
void MainWindow::setButtonWidth(const int& width)
{
    m_ButtonWidth = width;
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

    if (event->buttons() == Qt::LeftButton)
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
            this->showNormal();
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
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    m_IsPressedTitleBar = false;
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (p_TitleBar->underMouse() && !p_MinimumBtn->underMouse() && !p_MaximumBtn->underMouse() && !p_CloseBtn->underMouse())
    {
        toggleMaximize();
    }
}

void MainWindow::toggleMaximize()
{
    if (this->isMaximized())
    {
        this->showNormal();
    }
    else if (!this->isMaximized())
    {
        this->showMaximized();
    }
}

MainWindow::~MainWindow()
{
}
