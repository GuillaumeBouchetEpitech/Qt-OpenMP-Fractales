

#include "Fractale.hpp"

#include <QImage>

#include <QTime>
#include <chrono>

#include <iostream>
#include <iomanip>

#define D_BUFFER_SIZE D_FRACTALE_SIZE * D_FRACTALE_SIZE



void	Fractale::Julia(QImage& img)
{
	_pImage = &img;

    std::clock_t c_start = std::clock();
    QTime  time_start = QTime::currentTime();

    {
        double  inc = 2 * _settings._radius / _settings._size;

    	#pragma omp parallel for
    	for ( int y = 0; y < _settings._size; ++y )
    		for ( int x = 0; x < _settings._size; ++x )
            {
                t_vec2d   Z, Z2, P;

                P.x = _settings._Position.y + _settings._radius - inc * y;
                P.y = _settings._Position.x - _settings._radius + inc * x;

                Z.x = P.x;
                Z.y = P.y;
                Z2.x = Z.x * Z.x;
                Z2.y = Z.y * Z.y;

                int   i = 1;

                while ( ((Z2.x + Z2.y) < 4) && (i < _settings._iter) )
                {
                    Z.y = 2 * Z.x * Z.y + _settings._Perturbation.y;
                    Z.x = Z2.x - Z2.y + _settings._Perturbation.x;

                    Z2.x = Z.x * Z.x;
                    Z2.y = Z.y * Z.y;

                    ++i;
                }

                _settings._buffer[y * _settings._size + x] = i;
            }
    }

    std::clock_t c_end = std::clock();
    QTime  time_end = QTime::currentTime();

    Inject();

    float elapsed = time_start.msecsTo(time_end);
    float cputime = 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC;

    std::cout
        << std::fixed
        << std::setprecision(3)
        << "elapsed = " << std::setw(10) << elapsed << " ms" << std::endl
        << "cpu     = " << std::setw(10) << cputime << " ms" << std::endl
        << "ratio   = " << std::setw(10) << cputime/elapsed << std::endl
        ;
}

void	Fractale::Mandelbrot(QImage& img)
{
	_pImage = &img;

    std::clock_t c_start = std::clock();
    QTime  time_start = QTime::currentTime();

    {
        double  inc = 2 * _settings._radius / _settings._size;

        #pragma omp parallel for
        for ( int y = 0; y < _settings._size; ++y )
            for ( int x = 0; x < _settings._size; ++x )
            {

                t_vec2d   Z, Z2, P;

                P.x = _settings._Position.y + _settings._radius - inc * y;
                P.y = _settings._Position.x - _settings._radius + inc * x;

                Z = _settings._Perturbation;
                Z2.x = Z.x * Z.x;
                Z2.y = Z.y * Z.y;

                int i = 1;

                while ( ((Z2.x + Z2.y) < 4) && (i < _settings._iter) )
                {
                    Z.y = 2 * Z.x * Z.y + P.y;
                    Z.x = Z2.x - Z2.y + P.x;

                    Z2.x = Z.x * Z.x;
                    Z2.y = Z.y * Z.y;

                    ++i;
                }

                _settings._buffer[y * _settings._size + x] = i;
            }
    }

    std::clock_t c_end = std::clock();
    QTime  time_end = QTime::currentTime();

    Inject();

    float elapsed = time_start.msecsTo(time_end);
    float cputime = 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC;

    std::cout
        << std::fixed
        << std::setprecision(3)
        << "elapsed = " << std::setw(10) << elapsed << " ms" << std::endl
        << "cpu     = " << std::setw(10) << cputime << " ms" << std::endl
        << "ratio   = " << std::setw(10) << cputime/elapsed << std::endl
        ;
}


Fractale::t_fractale_settings::t_fractale_settings()
    : _buffer(NULL)
{
    reset();
}

Fractale::t_fractale_settings::~t_fractale_settings()
{
    delete[] _buffer;
}

void    Fractale::t_fractale_settings::reset()
{
    _radius = 2;
    _iter = 400;

    _Position = t_vec2d(0,0);
    _Perturbation = t_vec2d(0,0);

    _mode = 4;

    _pixelRes = 1;
    _size = D_FRACTALE_SIZE / _pixelRes;
    if (!_buffer)
        _buffer = new int[ D_BUFFER_SIZE ];
}


void    Fractale::reset()
{
    _settings.reset();
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