

#include "Fractale.hpp"

#include <QImage>

#define D_FRACTALE_SIZE 600
#define D_BUFFER_SIZE D_FRACTALE_SIZE * D_FRACTALE_SIZE


Fractale::Fractale()
{
    _settings._radius = 2;
    _settings._iter = 400;
    _settings._Position = QVector2D(0, 0);
    _settings._Perturbation = QVector2D(0, 0);

    _settings._mode = 4;

    _settings._pixelRes = 1;
    _settings._size = D_FRACTALE_SIZE / _settings._pixelRes;
    _settings._buffer = new int[ D_BUFFER_SIZE ];
}

Fractale::~Fractale()
{
	delete[] _settings._buffer;
}






void	Fractale::Julia(QImage& img)
{
	_pImage = &img;

	#pragma omp parallel for
	for ( int y = 0; y < _settings._size; y++ )
		for ( int x = 0; x < _settings._size; x++ )
            _settings._buffer[y * _settings._size + x] = executeJulia(x,y);

    Inject();
}

void	Fractale::Mandelbrot(QImage& img)
{
	_pImage = &img;

	#pragma omp parallel for
	for ( int y = 0; y < _settings._size; y++ )
		for ( int x = 0; x < _settings._size; x++ )
            _settings._buffer[y * _settings._size + x] = executeMandelbrot(x,y);

    Inject();
}






void	Fractale::moveUp()
{
	_settings._Position.setY( _settings._Position.y() + 0.02 * _settings._radius * 10);
}
void	Fractale::moveDown()
{
	_settings._Position.setY( _settings._Position.y() - 0.02 * _settings._radius * 10);
}
void	Fractale::moveLeft()
{
	_settings._Position.setX( _settings._Position.x() - 0.02 * _settings._radius * 10);
}
void	Fractale::moveRight()
{
	_settings._Position.setX( _settings._Position.x() + 0.02 * _settings._radius * 10);
}

void	Fractale::zoomIn()
{
	_settings._radius -= 0.2 * _settings._radius;
}
void	Fractale::zoomOut()
{
	_settings._radius += 0.2 * _settings._radius;
}



void	Fractale::perturbationUp()
{
	_settings._Perturbation.setY( _settings._Perturbation.y() + 0.1);
}
void	Fractale::perturbationDown()
{
	_settings._Perturbation.setY( _settings._Perturbation.y() - 0.1);
}
void	Fractale::perturbationLeft()
{
	_settings._Perturbation.setX( _settings._Perturbation.x() - 0.1);
}
void	Fractale::perturbationRight()
{
	_settings._Perturbation.setX( _settings._Perturbation.x() + 0.1);
}



void    Fractale::pixelResNormal()
{
    _settings._pixelRes = 1;
    _settings._size = D_FRACTALE_SIZE / _settings._pixelRes;
}
void    Fractale::pixelResLow()
{
    _settings._pixelRes = 2;
    _settings._size = D_FRACTALE_SIZE / _settings._pixelRes;
}
void    Fractale::pixelResUgly()
{
    _settings._pixelRes = 4;
    _settings._size = D_FRACTALE_SIZE / _settings._pixelRes;
}






int		Fractale::executeJulia(int x, int y)
{
    double          inc = 2 * _settings._radius / _settings._size;

    QVector2D    Z, Z2, P;

    P.setX(_settings._Position.y() + _settings._radius - inc * y);
    P.setY(_settings._Position.x() - _settings._radius + inc * x);

    int   i = 1;

    Z = P;
    Z2 = Z * Z;

    while ( ((Z2.x() + Z2.y()) < 4) && (i < _settings._iter) )
    {
        Z.setY( 2 * Z.x() * Z.y() + _settings._Perturbation.y() );
        Z.setX( Z2.x() - Z2.y() + _settings._Perturbation.x() );

        Z2 = Z * Z;

        ++i;
    }

    return i;
}

int		Fractale::executeMandelbrot(int x, int y)
{
    double          inc = 2 * _settings._radius / _settings._size;

    QVector2D    Z, Z2, P;

    P.setX(_settings._Position.y() + _settings._radius - inc * y);
    P.setY(_settings._Position.x() - _settings._radius + inc * x);

    int   i = 1;

    Z = _settings._Perturbation;
    Z2 = Z * Z;

    while ( ((Z2.x() + Z2.y()) < 4) && (i < _settings._iter) )
    {
        Z.setY( 2 * Z.x() * Z.y() + P.y() );
        Z.setX( Z2.x() - Z2.y() + P.x() );

        Z2 = Z * Z;

        ++i;
    }

    return i;
}





void     Fractale::injectPoint(int x, int y, double color_d)
{
    int   minY = y * _settings._pixelRes;
    int   minX = x * _settings._pixelRes;
    int   maxY = minY + _settings._pixelRes;
    int   maxX = minX + _settings._pixelRes;

    QRgb value = qRgb(color_d, color_d, color_d);

    for (int ry = minY; ry < maxY; ry++)
        for (int rx = minX; rx < maxX; rx++)
            _pImage->setPixel( rx, ry, value );
}

void	Fractale::Inject()
{
    int size = _settings._size * _settings._size;

    if (_settings._mode == 0)
    {
        for (int y = 0; y < _settings._size; y++)
            for (int x = 0; x < _settings._size; x++)
                if (_settings._buffer[(y * _settings._size) + x] == _settings._iter)
                    injectPoint(x, y);
                else
                    injectPoint(x, y, 255);
    }

    else if (_settings._mode == 1)
    {
        int     min = _settings._size, max = 0;
        for (int i = 0; i < _settings._size; i++)
        {
            if (min > _settings._buffer[i])
                min = _settings._buffer[i];
            if (max < _settings._buffer[i])
                max = _settings._buffer[i];
        }
        double  inc;
        if (max == min)
            inc = 253.0;
        else
            inc = 253.0 / (max - min);

        for (int y = 0; y < _settings._size; y++)
            for (int x = 0; x < _settings._size; x++)
                if (_settings._buffer[(y * _settings._size) + x] == _settings._iter)
                    injectPoint(x, y);
                else
                    injectPoint(x, y, 2 + floor(inc * (_settings._buffer[(y * _settings._size) + x] - min)));
    }

    else if (_settings._mode == 2)
    {
        double  inc = 253.0 / _settings._iter;

        for (int y = 0; y < _settings._size; y++)
            for (int x = 0; x < _settings._size; x++)
                if (_settings._buffer[(y * _settings._size) + x] == _settings._iter)
                    injectPoint(x, y);
                else
                    injectPoint(x, y, 2 + floor(inc * (_settings._buffer[(y * _settings._size) + x])));
    }

    else if (_settings._mode == 3)
    {
        int     min = _settings._size, max = 0;
        for (int i = 0; i < size; i++)
        {
            if (min > _settings._buffer[i])
                min = _settings._buffer[i];
            if (max < _settings._buffer[i])
                max = _settings._buffer[i];
        }

        double  inc;
        if (max == min)
            inc = 126.5;
        else
            inc = 126.5 / (max - min);

        for (int y = 0; y < _settings._size; y++)
            for (int x = 0; x < _settings._size; x++)
                if (_settings._buffer[(y * _settings._size) + x] == _settings._iter)
                    injectPoint(x, y);
                else if (_settings._buffer[(y * _settings._size) + x]&1)
                    injectPoint(x, y, 2 + floor(inc * (_settings._buffer[(y * _settings._size) + x] - min)));
                else
                    injectPoint(x, y, 129 + floor(inc * (_settings._buffer[(y * _settings._size) + x] - min)));
    }

    else if (_settings._mode == 4)
    {
        int     min = _settings._size, max = 0;
        for (int i = 0; i < size; i++)
        {
            if (min > _settings._buffer[i])
                min = _settings._buffer[i];
            if (max < _settings._buffer[i])
                max = _settings._buffer[i];
        }

        double  inc;
        if (max == min)
            inc = 253.0;
        else
            inc = 253.0 / (max - min);

        for (int y = 1; y < _settings._size - 1; y++)
        {
            int i = y * _settings._size + 1;
            for (int x = 1; x < _settings._size - 1; x++)
            {
                int flag =
                    (_settings._buffer[i] == _settings._buffer[i +      1]) +
                    (_settings._buffer[i] == _settings._buffer[i -      1]) +
                    (_settings._buffer[i] == _settings._buffer[i + _settings._size]) +
                    (_settings._buffer[i] == _settings._buffer[i - _settings._size]);
                if (flag <= 2)
                    injectPoint(x, y, 2 + floor(inc * (_settings._buffer[i] - min)));
                else
                    injectPoint(x, y);
                i++;
            }
        }
    }

}