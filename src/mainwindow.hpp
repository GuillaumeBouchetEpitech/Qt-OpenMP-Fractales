
#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP



#include "Fractale.hpp"



#include <QMainWindow>
// #include <QMdiArea>

#include <QVector2D>

#include <QLabel>
#include <QPushButton>

#include <QTimer>

#include <bitset>


class MainWindow : public QMainWindow
{
private:
	Q_OBJECT

private:
//    QMdiArea*   _pQMdiArea;

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private: // fractale

	struct t_fractale_settings
	{
		double		_radius;
		int			_iter, _mode;

		int*		_buffer;
		int			_index;

		QVector2D	_Position, _Perturbation;
		bool		IsJulia;
		int			_size, _scale;
	};

	t_fractale_settings	_settings;

    QLabel* _pLabel_refresh;

    QPushButton*	_pQPushButton_refresh;

    Fractale	_Fractale;
	QImage*		_pImage;


private: // events

	enum
	{
		eArrowUp = 0,
		eArrowDown,
		eArrowLeft,
		eArrowRight,

		eZoomIn,
		eZoomOut,

		ePerturbUp,
		ePerturbDown,
		ePerturbLeft,
		ePerturbRight,

		eCount
	};

	QTimer*				_pQTimer;
	std::bitset<eCount>	_BSet_events;

private:

	void	keyPressEvent(QKeyEvent* keyEvent);
	void	keyReleaseEvent(QKeyEvent* keyEvent);
	void	managekeyEvent( QKeyEvent* keyEvent, bool pressed );


private:

    void (Fractale::*_pt2Member)(QImage&);


// signals:

public slots:

	void 	slot_timeout();

	void	pushed_refresh();
	void	pushed_reset();
	void    pushed_mandelbrot();
	void    pushed_julia();

	void	pushed_up();
	void	pushed_down();
	void	pushed_left();
	void	pushed_right();

	void	pushed_zoom_in();
	void	pushed_zoom_out();

	void	pushed_perturbation_up();
	void	pushed_perturbation_down();
	void	pushed_perturbation_left();
	void	pushed_perturbation_right();

    void	pushed_pixelRes_normal();
    void	pushed_pixelRes_low();
    void	pushed_pixelRes_ugly();

    void	pushed_mode_0();
    void 	pushed_mode_1();
    void 	pushed_mode_2();
    void 	pushed_mode_3();
    void 	pushed_mode_4();
};


#endif // MAINWINDOW_HPP
