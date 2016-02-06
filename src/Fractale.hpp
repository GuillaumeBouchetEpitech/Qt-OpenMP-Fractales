#ifndef FRACTALE_HPP
#define FRACTALE_HPP


#include <QVector2D>

class Fractale
{
public:
	Fractale();
	~Fractale();

private: // fractale

	QImage*	_pImage;

	struct	t_fractale_settings
	{
		double		_radius;
		int			_iter, _mode;

		int*		_buffer;
		int			_index;

		QVector2D	_Position, _Perturbation;
		int			_size, _pixelRes;
	};
	t_fractale_settings	_settings;

public:
	void	Julia(QImage& img);
	void	Mandelbrot(QImage& img);

	void	moveUp();
	void	moveDown();
	void	moveLeft();
	void	moveRight();

	void	zoomIn();
	void	zoomOut();

	void	perturbationUp();
	void	perturbationDown();
	void	perturbationLeft();
	void	perturbationRight();

	void    pixelResNormal();
	void    pixelResLow();
	void    pixelResUgly();

private:
	int		executeJulia(int x, int y);
	int		executeMandelbrot(int x, int y);

	void	injectPoint(int x, int y, double color_d = 0);
	void	Inject();
};


#endif // FRACTALE_HPP
