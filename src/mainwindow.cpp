

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





// #include <QPoint>   /// this->pos()

#include <QLabel>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
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
        pMainLayout->addWidget( _pLabel_refresh, 0,0, 20,20 );

        //

        {
            // _pQPushButton_refresh = new QPushButton("refresh");
            // _pQPushButton_refresh->setShortcut(QKeySequence(Qt::Key_Tab));
            // pMainLayout->addWidget(_pQPushButton_refresh, 0,21);
            // connect(_pQPushButton_refresh, SIGNAL(clicked()), this, SLOT(pushed_refresh()));

            QPushButton* pQPushButton_reset = new QPushButton("reset");
            pQPushButton_reset->setShortcut(QKeySequence(Qt::Key_Tab));
            pMainLayout->addWidget(pQPushButton_reset, 0,21);
            connect(pQPushButton_reset, SIGNAL(clicked()), this, SLOT(pushed_reset()));
        }

        //

        {
            QGridLayout*    pLayout = new QGridLayout();
            pMainLayout->addLayout(pLayout, 1,21);

            QLabel* pLabel = new QLabel("position");
            pLabel->setAlignment(Qt::AlignHCenter);
            // pLabel->setStyleSheet("QLabel { background-color : red; color : blue; }");
            pLayout->addWidget( pLabel, 0,0, 1,3);

            QPushButton* pQPushButton_up = new QPushButton("up [W]");
            pQPushButton_up->setShortcut(QKeySequence(Qt::Key_W));
            pLayout->addWidget( pQPushButton_up, 1,1);
            connect(pQPushButton_up, SIGNAL(clicked()), this, SLOT(pushed_up()));

            QPushButton* pQPushButton_down = new QPushButton("down [S]");
            pQPushButton_down->setShortcut(QKeySequence(Qt::Key_S));
            pLayout->addWidget( pQPushButton_down, 2,1);
            connect(pQPushButton_down, SIGNAL(clicked()), this, SLOT(pushed_down()));

            QPushButton* pQPushButton_left = new QPushButton("left [A]");
            pQPushButton_left->setShortcut(QKeySequence(Qt::Key_A));
            pLayout->addWidget( pQPushButton_left, 2,0);
            connect(pQPushButton_left, SIGNAL(clicked()), this, SLOT(pushed_left()));

            QPushButton* pQPushButton_right = new QPushButton("right [D]");
            pQPushButton_right->setShortcut(QKeySequence(Qt::Key_D));
            pLayout->addWidget( pQPushButton_right, 2,2);
            connect(pQPushButton_right, SIGNAL(clicked()), this, SLOT(pushed_right()));
        }

        //

        {
            QGridLayout*    pLayout = new QGridLayout();
            pMainLayout->addLayout(pLayout, 2,21);

            QLabel* pLabel = new QLabel("zoom");
            pLabel->setAlignment(Qt::AlignHCenter);
            // pLabel->setStyleSheet("QLabel { background-color : red; color : blue; }");
            pLayout->addWidget( pLabel, 0,0, 1,2);

            QPushButton* pQPushButton_up = new QPushButton("zoom in [+]");
            pQPushButton_up->setShortcut(QKeySequence(Qt::Key_Plus));
            pLayout->addWidget( pQPushButton_up, 1,0);
            connect(pQPushButton_up, SIGNAL(clicked()), this, SLOT(pushed_zoom_in()));

            QPushButton* pQPushButton_down = new QPushButton("zoom out [-]");
            pQPushButton_down->setShortcut(QKeySequence(Qt::Key_Minus));
            pLayout->addWidget( pQPushButton_down, 1,1);
            connect(pQPushButton_down, SIGNAL(clicked()), this, SLOT(pushed_zoom_out()));
        }

        //

        {
            QGridLayout*    pLayout = new QGridLayout();
            pMainLayout->addLayout(pLayout, 3,21);

            QLabel* pLabel = new QLabel("perturbation");
            pLabel->setAlignment(Qt::AlignHCenter);
            // pLabel->setStyleSheet("QLabel { background-color : red; color : blue; }");
            pLayout->addWidget( pLabel, 0,0, 1,3);

            QPushButton* pQPushButton_up = new QPushButton("up");
            pQPushButton_up->setShortcut(QKeySequence(Qt::Key_Up));
            pLayout->addWidget( pQPushButton_up, 1,1);
            connect(pQPushButton_up, SIGNAL(clicked()), this, SLOT(pushed_perturbation_up()));

            QPushButton* pQPushButton_down = new QPushButton("down");
            pQPushButton_down->setShortcut(QKeySequence(Qt::Key_Down));
            pLayout->addWidget( pQPushButton_down, 2,1);
            connect(pQPushButton_down, SIGNAL(clicked()), this, SLOT(pushed_perturbation_down()));

            QPushButton* pQPushButton_left = new QPushButton("left");
            pQPushButton_left->setShortcut(QKeySequence(Qt::Key_Left));
            pLayout->addWidget( pQPushButton_left, 2,0);
            connect(pQPushButton_left, SIGNAL(clicked()), this, SLOT(pushed_perturbation_left()));

            QPushButton* pQPushButton_right = new QPushButton("right");
            pQPushButton_right->setShortcut(QKeySequence(Qt::Key_Right));
            pLayout->addWidget( pQPushButton_right, 2,2);
            connect(pQPushButton_right, SIGNAL(clicked()), this, SLOT(pushed_perturbation_right()));
        }

        //

        {
            QGridLayout*    pLayout = new QGridLayout();
            pMainLayout->addLayout(pLayout, 4,21);

            QLabel* pLabel = new QLabel("pixel resolution");
            pLabel->setAlignment(Qt::AlignHCenter);
            // pLabel->setStyleSheet("QLabel { background-color : red; color : blue; }");
            pLayout->addWidget( pLabel, 0,0, 1,3);

            QPushButton* pQPushButton_up = new QPushButton("normal [7]");
            pQPushButton_up->setShortcut(QKeySequence(Qt::Key_7));
            pLayout->addWidget( pQPushButton_up, 1,0);
            connect(pQPushButton_up, SIGNAL(clicked()), this, SLOT(pushed_pixelRes_normal()));

            QPushButton* pQPushButton_down = new QPushButton("low [8]");
            pQPushButton_down->setShortcut(QKeySequence(Qt::Key_8));
            pLayout->addWidget( pQPushButton_down, 1,1);
            connect(pQPushButton_down, SIGNAL(clicked()), this, SLOT(pushed_pixelRes_low()));

            QPushButton* pQPushButton_left = new QPushButton("ugly [9]");
            pQPushButton_left->setShortcut(QKeySequence(Qt::Key_9));
            pLayout->addWidget( pQPushButton_left, 1,2);
            connect(pQPushButton_left, SIGNAL(clicked()), this, SLOT(pushed_pixelRes_ugly()));
        }

        //

        this->setCentralWidget( pMainWidget );
    }

    pushed_refresh();
}

MainWindow::~MainWindow()
{
}



void    MainWindow::pushed_refresh()
{
    std::cout << "refresh" << std::endl;

    _Fractale.Mandelbrot( *_pImage );
    _pLabel_refresh->setPixmap( QPixmap::fromImage(*_pImage) );
}

void    MainWindow::pushed_reset()
{
    std::cout << "reset" << std::endl;

    _Fractale.reset();

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


