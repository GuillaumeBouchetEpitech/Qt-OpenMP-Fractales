#pragma once

// #include <QVector2D>
#include <QImage>

#define D_FRACTALE_SIZE 600

class Fractale
{
public:

    //
    // -> this is 5 time faster than a using a QVector2D...
    struct t_vec2d
    {
        double x, y;

        t_vec2d() = default;

        t_vec2d(int X, int Y)
            : x(X)
            , y(Y)
        {}
    };
    // -> this is 5 time faster than a using a QVector2D...
    //

private: // fractale

    QImage*	_pImage;

    struct	t_fractale_settings
    {
        double  _radius;
        int     _iter, _mode;

        int*    _buffer;
        int     _index;

        t_vec2d _Position, _Perturbation;
        int     _size, _pixelRes;

        t_fractale_settings();
        ~t_fractale_settings();
        void reset();
    }
    _settings;

public:
    void Julia(QImage& img);
    void Mandelbrot(QImage& img);

    void reset();

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    void zoomIn();
    void zoomOut();

    void perturbationUp();
    void perturbationDown();
    void perturbationLeft();
    void perturbationRight();

    void pixelResNormal();
    void pixelResLow();
    void pixelResUgly();

    void injectionMode(int mode);

private:
    void injectPoint(int x, int y, double color_d = 0);
    void Inject();
};
