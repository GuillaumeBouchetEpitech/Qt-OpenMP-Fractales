

#include "mainwindow.hpp"


// #include "navigator_globalvalue.h"


#include <iostream>

#include <QApplication>

#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>

#include <QShortcut>
#include <QKeyEvent>

// #include <QMdiSubWindow>

#include <QGridLayout>

#include <QGroupBox>




// #include <QPoint>   /// this->pos()

#include <QLabel>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _pt2Member(&Fractale::Julia)
{
    this->setMinimumSize(800, 600);

    // QAction*    pQAction_Quit = new QAction("&Quit", this);
    // connect( pQAction_Quit, SIGNAL(triggered()), qApp, SLOT(quit()) );

    // {
    //     QMenu*  pQMenu_File = menuBar()->addMenu("&File");
    //     pQMenu_File->addAction( pQAction_Quit );

    //     // QMenu*  pQMenu_View = menuBar()->addMenu("&View");
    //     // pQMenu_View->addAction( pQAction_SwitchViewMode );
    // }

    // {
    //     QToolBar*   pQToolBar = addToolBar("File");

    //     pQToolBar->addAction( pQAction_Quit );
    //     // pQToolBar->addAction( pQAction_SwitchViewMode );
    // }

    {
        _pImage = new QImage(D_FRACTALE_SIZE, D_FRACTALE_SIZE, QImage::Format_RGB32);

        QWidget*        pMainWidget = new QWidget(this);
        QGridLayout*    pMainLayout = new QGridLayout(pMainWidget);
        pMainWidget->setLayout(pMainLayout);

        //

        _pLabel_refresh = new QLabel();
        _pLabel_refresh->setPixmap( QPixmap::fromImage(*_pImage) );
        pMainLayout->addWidget( _pLabel_refresh, 0,0, 10,10 );

        //

        {
            // _pQPushButton_refresh = new QPushButton("refresh");
            // _pQPushButton_refresh->setShortcut(QKeySequence(Qt::Key_Tab));
            // pMainLayout->addWidget(_pQPushButton_refresh, 0,11);
            // connect(_pQPushButton_refresh, SIGNAL(clicked()), this, SLOT(pushed_refresh()));

            QGroupBox*        pWidget = new QGroupBox(tr("Settings"));
            pMainLayout->addWidget(pWidget, 0,11);
            QGridLayout*    pLayout = new QGridLayout();
            pWidget->setLayout(pLayout);

            QPushButton* pQPushButton_reset = new QPushButton("reset [Tab]");
            pQPushButton_reset->setShortcut(QKeySequence(Qt::Key_Tab));
            pLayout->addWidget( pQPushButton_reset, 0,0);
            connect(pQPushButton_reset, SIGNAL(clicked()), this, SLOT(pushed_reset()));

            QPushButton* pQPushButton_mdlbrot = new QPushButton("Mandelbrot");
            pLayout->addWidget( pQPushButton_mdlbrot, 0,1);
            connect(pQPushButton_mdlbrot, SIGNAL(clicked()), this, SLOT(pushed_mandelbrot()));

            QPushButton* pQPushButton_julia = new QPushButton("Julia");
            pLayout->addWidget( pQPushButton_julia, 0,2);
            connect(pQPushButton_julia, SIGNAL(clicked()), this, SLOT(pushed_julia()));
        }

        //

        {
            QGroupBox*        pWidget = new QGroupBox(tr("Movements"));
            pMainLayout->addWidget(pWidget, 1,11);
            QGridLayout*    pLayout = new QGridLayout();
            pWidget->setLayout(pLayout);

            QPushButton* pQPushButton_up = new QPushButton("up [W/Z]");
            pLayout->addWidget( pQPushButton_up, 0,1);
            connect(pQPushButton_up, SIGNAL(clicked()), this, SLOT(pushed_up()));

            QPushButton* pQPushButton_down = new QPushButton("down [S]");
            pLayout->addWidget( pQPushButton_down, 1,1);
            connect(pQPushButton_down, SIGNAL(clicked()), this, SLOT(pushed_down()));

            QPushButton* pQPushButton_left = new QPushButton("left [A/Q]");
            pLayout->addWidget( pQPushButton_left, 1,0);
            connect(pQPushButton_left, SIGNAL(clicked()), this, SLOT(pushed_left()));

            QPushButton* pQPushButton_right = new QPushButton("right [D]");
            pLayout->addWidget( pQPushButton_right, 1,2);
            connect(pQPushButton_right, SIGNAL(clicked()), this, SLOT(pushed_right()));
        }

        //

        {
            QGroupBox*        pWidget = new QGroupBox(tr("Zoom"));
            pMainLayout->addWidget(pWidget, 2,11);
            QGridLayout*    pLayout = new QGridLayout();
            pWidget->setLayout(pLayout);

            QPushButton* pQPushButton_up = new QPushButton("zoom in [+]");
            pLayout->addWidget( pQPushButton_up, 0,0);
            connect(pQPushButton_up, SIGNAL(clicked()), this, SLOT(pushed_zoom_in()));

            QPushButton* pQPushButton_down = new QPushButton("zoom out [-]");
            pLayout->addWidget( pQPushButton_down, 0,1);
            connect(pQPushButton_down, SIGNAL(clicked()), this, SLOT(pushed_zoom_out()));
        }

        //

        {
            QGroupBox*        pWidget = new QGroupBox(tr("Perturbation"));
            pMainLayout->addWidget(pWidget, 3,11);
            QGridLayout*    pLayout = new QGridLayout();
            pWidget->setLayout(pLayout);

            QPushButton* pQPushButton_up = new QPushButton("up");
            pLayout->addWidget( pQPushButton_up, 0,1);
            connect(pQPushButton_up, SIGNAL(clicked()), this, SLOT(pushed_perturbation_up()));

            QPushButton* pQPushButton_down = new QPushButton("down");
            pLayout->addWidget( pQPushButton_down, 1,1);
            connect(pQPushButton_down, SIGNAL(clicked()), this, SLOT(pushed_perturbation_down()));

            QPushButton* pQPushButton_left = new QPushButton("left");
            pLayout->addWidget( pQPushButton_left, 1,0);
            connect(pQPushButton_left, SIGNAL(clicked()), this, SLOT(pushed_perturbation_left()));

            QPushButton* pQPushButton_right = new QPushButton("right");
            pLayout->addWidget( pQPushButton_right, 1,2);
            connect(pQPushButton_right, SIGNAL(clicked()), this, SLOT(pushed_perturbation_right()));
        }

        //

        {
            QGroupBox*        pWidget = new QGroupBox(tr("Pixel resolution"));
            pMainLayout->addWidget(pWidget, 4,11);
            QGridLayout*    pLayout = new QGridLayout();
            pWidget->setLayout(pLayout);

            QPushButton* pQPushButton_up = new QPushButton("normal [7]");
            pQPushButton_up->setShortcut(QKeySequence(Qt::Key_7));
            pLayout->addWidget( pQPushButton_up, 0,0);
            connect(pQPushButton_up, SIGNAL(clicked()), this, SLOT(pushed_pixelRes_normal()));

            QPushButton* pQPushButton_down = new QPushButton("low [8]");
            pQPushButton_down->setShortcut(QKeySequence(Qt::Key_8));
            pLayout->addWidget( pQPushButton_down, 0,1);
            connect(pQPushButton_down, SIGNAL(clicked()), this, SLOT(pushed_pixelRes_low()));

            QPushButton* pQPushButton_left = new QPushButton("ugly [9]");
            pQPushButton_left->setShortcut(QKeySequence(Qt::Key_9));
            pLayout->addWidget( pQPushButton_left, 0,2);
            connect(pQPushButton_left, SIGNAL(clicked()), this, SLOT(pushed_pixelRes_ugly()));
        }

        //

        {
            QGroupBox*        pWidget = new QGroupBox(tr("Injection mode"));
            pMainLayout->addWidget(pWidget, 5,11);
            QGridLayout*    pLayout = new QGridLayout();
            pWidget->setLayout(pLayout);

            int btt_width = 50;

            QPushButton* pQPushButton_md0 = new QPushButton("[0]");
            pQPushButton_md0->setShortcut(QKeySequence(Qt::Key_0));
            pQPushButton_md0->setMaximumWidth(btt_width);
            pLayout->addWidget( pQPushButton_md0, 0,0);
            connect(pQPushButton_md0, SIGNAL(clicked()), this, SLOT(pushed_mode_0()));

            QPushButton* pQPushButton_md1 = new QPushButton("[1]");
            pQPushButton_md1->setShortcut(QKeySequence(Qt::Key_1));
            pQPushButton_md1->setMaximumWidth(btt_width);
            pLayout->addWidget( pQPushButton_md1, 0,1);
            connect(pQPushButton_md1, SIGNAL(clicked()), this, SLOT(pushed_mode_1()));

            QPushButton* pQPushButton_md2 = new QPushButton("[2]");
            pQPushButton_md2->setShortcut(QKeySequence(Qt::Key_2));
            pQPushButton_md2->setMaximumWidth(btt_width);
            pLayout->addWidget( pQPushButton_md2, 0,2);
            connect(pQPushButton_md2, SIGNAL(clicked()), this, SLOT(pushed_mode_2()));

            QPushButton* pQPushButton_md3 = new QPushButton("[3]");
            pQPushButton_md3->setShortcut(QKeySequence(Qt::Key_3));
            pQPushButton_md3->setMaximumWidth(btt_width);
            pLayout->addWidget( pQPushButton_md3, 0,3);
            connect(pQPushButton_md3, SIGNAL(clicked()), this, SLOT(pushed_mode_3()));

            QPushButton* pQPushButton_md4 = new QPushButton("[4]");
            pQPushButton_md4->setShortcut(QKeySequence(Qt::Key_4));
            pQPushButton_md4->setMaximumWidth(btt_width);
            pLayout->addWidget( pQPushButton_md4, 0,4);
            connect(pQPushButton_md4, SIGNAL(clicked()), this, SLOT(pushed_mode_4()));
        }

        //

        this->setCentralWidget( pMainWidget );
    }


    _pQTimer = new QTimer(this);
    connect( _pQTimer, SIGNAL(timeout()), this, SLOT(slot_timeout()) );


    pushed_refresh();
}

MainWindow::~MainWindow()
{
}






//
//
// EVENTS

// 1- the user press/release a recognisaned key
// 2- the frame loop is started if needed (key pressed)
// 3- the frame loop process the keys pressed and refresh

void    MainWindow::keyPressEvent(QKeyEvent* keyEvent)
{
    managekeyEvent( keyEvent, true );
}
void    MainWindow::keyReleaseEvent(QKeyEvent* keyEvent)
{
    managekeyEvent( keyEvent, false );
}

void    MainWindow::managekeyEvent( QKeyEvent* keyEvent, bool pressed )
{

    // 1- the user press/release a recognisaned key

    switch (keyEvent->key())
    {

    //  position

    case Qt::Key_W:
    case Qt::Key_Z: _BSet_events[eArrowUp] = pressed;  break;
    case Qt::Key_S: _BSet_events[eArrowDown] = pressed;  break;
    case Qt::Key_Q:
    case Qt::Key_A: _BSet_events[eArrowLeft] = pressed;  break;
    case Qt::Key_D: _BSet_events[eArrowRight] = pressed;  break;

    //  zoom

    case Qt::Key_Plus:  _BSet_events[eZoomIn] = pressed;  break;
    case Qt::Key_Minus: _BSet_events[eZoomOut] = pressed;  break;

    //  perturbation

    case Qt::Key_Up:    _BSet_events[ePerturbUp] = pressed;  break;
    case Qt::Key_Down:  _BSet_events[ePerturbDown] = pressed;  break;
    case Qt::Key_Left:  _BSet_events[ePerturbLeft] = pressed;  break;
    case Qt::Key_Right: _BSet_events[ePerturbRight] = pressed;  break;
    }

    // 2- the frame loop is started if needed (key pressed)

    if (!_pQTimer->isActive() && _BSet_events.count())
        _pQTimer->start();
    else if (_pQTimer->isActive() && !_BSet_events.count())
        _pQTimer->stop();
}

void    MainWindow::slot_timeout()
{
    // 3- the frame loop process the keys pressed and refresh

    if      (_BSet_events[eArrowUp])    _Fractale.moveUp();
    else if (_BSet_events[eArrowDown])  _Fractale.moveDown();
    if      (_BSet_events[eArrowLeft])  _Fractale.moveLeft();
    else if (_BSet_events[eArrowRight]) _Fractale.moveRight();

    if      (_BSet_events[eZoomIn])     _Fractale.zoomIn();
    else if (_BSet_events[eZoomOut])    _Fractale.zoomOut();

    if      (_BSet_events[ePerturbUp])      _Fractale.perturbationUp();
    else if (_BSet_events[ePerturbDown])    _Fractale.perturbationDown();
    if      (_BSet_events[ePerturbLeft])    _Fractale.perturbationLeft();
    else if (_BSet_events[ePerturbRight])   _Fractale.perturbationRight();

    pushed_refresh();
}

// EVENTS
//
//







void    MainWindow::pushed_refresh()
{
    std::cout << "refresh" << std::endl;

    // just wanted a method ptr to avoid a dirty if/else with an extra bool...
    (_Fractale.*_pt2Member)( *_pImage );

    _pLabel_refresh->setPixmap( QPixmap::fromImage(*_pImage) );

    this->setFocus(); // to make the arrows behave...
}

void    MainWindow::pushed_reset()
{
    std::cout << "reset" << std::endl;

    _Fractale.reset();

    pushed_refresh();
}

void    MainWindow::pushed_mandelbrot()
{
    std::cout << "mandelbrot" << std::endl;

    _pt2Member = &Fractale::Mandelbrot;

    pushed_refresh();
}

void    MainWindow::pushed_julia()
{
    std::cout << "julia" << std::endl;

    _pt2Member = &Fractale::Julia;

    pushed_refresh();
}



void    MainWindow::pushed_up()
{
    std::cout << "up" << std::endl;
    _Fractale.moveUp();

    pushed_refresh();
}
void    MainWindow::pushed_down()
{
    std::cout << "down" << std::endl;
    _Fractale.moveDown();

    pushed_refresh();
}
void    MainWindow::pushed_left()
{
    std::cout << "left" << std::endl;
    _Fractale.moveLeft();

    pushed_refresh();
}
void    MainWindow::pushed_right()
{
    std::cout << "right" << std::endl;
    _Fractale.moveRight();

    pushed_refresh();
}



void    MainWindow::pushed_zoom_in()
{
    std::cout << "zoom_in" << std::endl;
    _Fractale.zoomIn();

    pushed_refresh();
}
void    MainWindow::pushed_zoom_out()
{
    std::cout << "zoom_out" << std::endl;
    _Fractale.zoomOut();

    pushed_refresh();
}



void    MainWindow::pushed_perturbation_up()
{
    std::cout << "perturbation_up" << std::endl;
    _Fractale.perturbationUp();

    pushed_refresh();
}
void    MainWindow::pushed_perturbation_down()
{
    std::cout << "perturbation_down" << std::endl;
    _Fractale.perturbationDown();

    pushed_refresh();
}
void    MainWindow::pushed_perturbation_left()
{
    std::cout << "perturbation_left" << std::endl;
    _Fractale.perturbationLeft();

    pushed_refresh();
}
void    MainWindow::pushed_perturbation_right()
{
    std::cout << "perturbation_right" << std::endl;
    _Fractale.perturbationRight();

    pushed_refresh();
}



void    MainWindow::pushed_pixelRes_normal()
{
    std::cout << "pixelRes_normal" << std::endl;
    _Fractale.pixelResNormal();

    pushed_refresh();
}
void    MainWindow::pushed_pixelRes_low()
{
    std::cout << "pixelRes_low" << std::endl;
    _Fractale.pixelResLow();

    pushed_refresh();
}
void    MainWindow::pushed_pixelRes_ugly()
{
    std::cout << "pixelRes_ugly" << std::endl;
    _Fractale.pixelResUgly();

    pushed_refresh();
}

void    MainWindow::pushed_mode_0()
{
    std::cout << "pushed_mode_1" << std::endl;
    _Fractale.injectionMode(0);

    pushed_refresh();
}
void    MainWindow::pushed_mode_1()
{
    std::cout << "pushed_mode_1" << std::endl;
    _Fractale.injectionMode(1);

    pushed_refresh();
}
void    MainWindow::pushed_mode_2()
{
    std::cout << "pushed_mode_2" << std::endl;
    _Fractale.injectionMode(2);

    pushed_refresh();
}
void    MainWindow::pushed_mode_3()
{
    std::cout << "pushed_mode_3" << std::endl;
    _Fractale.injectionMode(3);

    pushed_refresh();
}
void    MainWindow::pushed_mode_4()
{
    std::cout << "pushed_mode_4" << std::endl;
    _Fractale.injectionMode(4);

    pushed_refresh();
}




