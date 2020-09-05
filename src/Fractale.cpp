

#include "Fractale.hpp"

#include <QImage>

#include <QTime>
#include <chrono>

#include <iostream>
#include <iomanip>
#include <cmath>

#define D_BUFFER_SIZE D_FRACTALE_SIZE * D_FRACTALE_SIZE



void Fractale::Julia(QImage& img)
{
    _pImage = &img;

    // std::clock_t c_start = std::clock();
    // QTime  time_start = QTime::currentTime();

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

    // std::clock_t c_end = std::clock();
    // QTime  time_end = QTime::currentTime();

    Inject();

    // float elapsed = time_start.msecsTo(time_end);
    // float cputime = 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC;

    // std::cout
    //     << std::fixed
    //     << std::setprecision(3)
    //     << "elapsed = " << std::setw(10) << elapsed << " ms" << std::endl
    //     << "cpu     = " << std::setw(10) << cputime << " ms" << std::endl
    //     << "ratio   = " << std::setw(10) << cputime/elapsed << std::endl
    //     ;
}

void Fractale::Mandelbrot(QImage& img)
{
    _pImage = &img;

    // std::clock_t c_start = std::clock();
    // QTime  time_start = QTime::currentTime();

    {
        double inc = 2 * _settings._radius / _settings._size;

        #pragma omp parallel for
        for (int yy = 0; yy < _settings._size; ++yy)
        for (int xx = 0; xx < _settings._size; ++xx)
        {
            t_vec2d Z, Z2, P;

            P.x = _settings._Position.y + _settings._radius - inc * yy;
            P.y = _settings._Position.x - _settings._radius + inc * xx;

            Z = _settings._Perturbation;
            Z2.x = Z.x * Z.x;
            Z2.y = Z.y * Z.y;

            int ii = 1;

            while (((Z2.x + Z2.y) < 4) && (ii < _settings._iter))
            {
                Z.y = 2 * Z.x * Z.y + P.y;
                Z.x = Z2.x - Z2.y + P.x;

                Z2.x = Z.x * Z.x;
                Z2.y = Z.y * Z.y;

                ++ii;
            }

            _settings._buffer[yy * _settings._size + xx] = ii;
        }
    }

    // std::clock_t c_end = std::clock();
    // QTime time_end = QTime::currentTime();

    Inject();

    // float elapsed = time_start.msecsTo(time_end);
    // float cputime = 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC;

    // std::cout
    //     << std::fixed
    //     << std::setprecision(3)
    //     << "elapsed = " << std::setw(10) << elapsed << " ms" << std::endl
    //     << "cpu     = " << std::setw(10) << cputime << " ms" << std::endl
    //     << "ratio   = " << std::setw(10) << cputime/elapsed << std::endl
    //     ;
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

void Fractale::t_fractale_settings::reset()
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


void Fractale::reset()
{
    _settings.reset();
}



void Fractale::pixelResNormal()
{
    _settings._pixelRes = 1;
    _settings._size = D_FRACTALE_SIZE / _settings._pixelRes;
}
void Fractale::pixelResLow()
{
    _settings._pixelRes = 2;
    _settings._size = D_FRACTALE_SIZE / _settings._pixelRes;
}
void Fractale::pixelResUgly()
{
    _settings._pixelRes = 4;
    _settings._size = D_FRACTALE_SIZE / _settings._pixelRes;
}





void Fractale::injectPoint(int x, int y, double color_d)
{
    int minY = y * _settings._pixelRes;
    int minX = x * _settings._pixelRes;
    int maxY = minY + _settings._pixelRes;
    int maxX = minX + _settings._pixelRes;

    QRgb value = qRgb(color_d, color_d, color_d);

    for (int ry = minY; ry < maxY; ++ry)
    for (int rx = minX; rx < maxX; ++rx)
        _pImage->setPixel(rx, ry, value);
}

void Fractale::Inject()
{
    int size = _settings._size * _settings._size;

    switch (_settings._mode)
    {
        case 0:
        {
            for (int yy = 0; yy < _settings._size; yy++)
            for (int xx = 0; xx < _settings._size; xx++)
            {
                if (_settings._buffer[(yy * _settings._size) + xx] == _settings._iter)
                    injectPoint(xx, yy);
                else
                    injectPoint(xx, yy, 255);
            }
            break;
        }
        case 1:
        {
            int min = _settings._size, max = 0;
            for (int ii = 0; ii < _settings._size; ++ii)
            {
                if (min > _settings._buffer[ii])
                    min = _settings._buffer[ii];
                if (max < _settings._buffer[ii])
                    max = _settings._buffer[ii];
            }

            double inc;
            if (max == min)
                inc = 253.0;
            else
                inc = 253.0 / (max - min);

            for (int yy = 0; yy < _settings._size; ++yy)
            for (int xx = 0; xx < _settings._size; ++xx)
            {
                if (_settings._buffer[(yy * _settings._size) + xx] == _settings._iter)
                    injectPoint(xx, yy);
                else
                    injectPoint(xx, yy, 2 + std::floor(inc * (_settings._buffer[(yy * _settings._size) + xx] - min)));
            }
            break;
        }
        case 2:
        {
            double inc = 253.0 / _settings._iter;

            for (int yy = 0; yy < _settings._size; ++yy)
            for (int xx = 0; xx < _settings._size; ++xx)
            {
                if (_settings._buffer[(yy * _settings._size) + xx] == _settings._iter)
                    injectPoint(xx, yy);
                else
                    injectPoint(xx, yy, 2 + std::floor(inc * (_settings._buffer[(yy * _settings._size) + xx])));
            }
            break;
        }
        case 3:
        {
            int min = _settings._size, max = 0;
            for (int ii = 0; ii < size; ++ii)
            {
                if (min > _settings._buffer[ii])
                    min = _settings._buffer[ii];
                if (max < _settings._buffer[ii])
                    max = _settings._buffer[ii];
            }

            double inc;
            if (max == min)
                inc = 126.5;
            else
                inc = 126.5 / (max - min);

            for (int yy = 0; yy < _settings._size; ++yy)
            for (int xx = 0; xx < _settings._size; ++xx)
            {
                if (_settings._buffer[(yy * _settings._size) + xx] == _settings._iter)
                    injectPoint(xx, yy);
                else if (_settings._buffer[(yy * _settings._size) + xx]&1)
                    injectPoint(xx, yy, 2 + std::floor(inc * (_settings._buffer[(yy * _settings._size) + xx] - min)));
                else
                    injectPoint(xx, yy, 129 + std::floor(inc * (_settings._buffer[(yy * _settings._size) + xx] - min)));
            }
            break;
        }
        case 4:
        {
            int min = _settings._size, max = 0;
            for (int ii = 0; ii < size; ++ii)
            {
                if (min > _settings._buffer[ii])
                    min = _settings._buffer[ii];
                if (max < _settings._buffer[ii])
                    max = _settings._buffer[ii];
            }

            double inc;
            if (max == min)
                inc = 253.0;
            else
                inc = 253.0 / (max - min);

            for (int yy = 1; yy < _settings._size - 1; ++yy)
            {
                int ii = yy * _settings._size + 1;
                for (int xx = 1; xx < _settings._size - 1; ++xx)
                {
                    int flag = (
                        (_settings._buffer[ii] == _settings._buffer[ii +               1]) +
                        (_settings._buffer[ii] == _settings._buffer[ii -               1]) +
                        (_settings._buffer[ii] == _settings._buffer[ii + _settings._size]) +
                        (_settings._buffer[ii] == _settings._buffer[ii - _settings._size])
                    );

                    if (flag <= 2)
                        injectPoint(xx, yy, 2 + std::floor(inc * (_settings._buffer[ii] - min)));
                    else
                        injectPoint(xx, yy);

                    ++ii;
                }
            }
        }
    }
}



void Fractale::moveUp()
{
    _settings._Position.y = _settings._Position.y + 0.01 * _settings._radius * 1;
}
void Fractale::moveDown()
{
    _settings._Position.y = _settings._Position.y - 0.01 * _settings._radius * 1;
}
void Fractale::moveLeft()
{
    _settings._Position.x = _settings._Position.x - 0.01 * _settings._radius * 1;
}
void Fractale::moveRight()
{
    _settings._Position.x = _settings._Position.x + 0.01 * _settings._radius * 1;
}



void Fractale::zoomIn()
{
    _settings._radius -= 0.01 * _settings._radius;
}

void Fractale::zoomOut()
{
    _settings._radius += 0.01 * _settings._radius;
}



void Fractale::perturbationUp()
{
    _settings._Perturbation.y = _settings._Perturbation.y + 0.01;
}
void Fractale::perturbationDown()
{
    _settings._Perturbation.y = _settings._Perturbation.y - 0.01;
}
void Fractale::perturbationLeft()
{
    _settings._Perturbation.x = _settings._Perturbation.x - 0.01;
}
void Fractale::perturbationRight()
{
    _settings._Perturbation.x = _settings._Perturbation.x + 0.01;
}


void Fractale::injectionMode(int mode)
{
    _settings._mode = std::max(std::min(mode,4),0);
}
