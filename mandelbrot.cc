#include <complex>
#include <iostream>
#include <cstdlib>
#include <assert.h>
//#include <getopt.h>

/*
https://en.wikipedia.org/wiki/Mandelbrot_set

for each pixel (Px, Py) on the screen do
    x0 := scaled x coordinate of pixel (scaled to lie in the Mandelbrot X scale (-2.5, 1))
    y0 := scaled y coordinate of pixel (scaled to lie in the Mandelbrot Y scale (-1, 1))
    x := 0.0
    y := 0.0
    iteration := 0
    max_iteration := 1000
    while (x*x + y*y ≤ 2*2 AND iteration < max_iteration) do
        xtemp := x*x - y*y + x0
        y := 2*x*y + y0
        x := xtemp
        iteration := iteration + 1

    color := palette[iteration]
    plot(Px, Py, color)
*/

typedef double F;
typedef std::complex<F> complex;

int f( complex c, int maxiter, F outofbounds ) {
    complex z = c;
    int i = 0;
    while ( i < maxiter ) {
        F n = std::norm(z);
        if ( n > outofbounds ) {
            break;
        }

        z = z * z + c;
        i++;
    }

    return i;
}

int main( int argc, char ** argv )
{
    if ( argc < 5 ) {
        std::cerr << "usage: mandlebrot x0 x1 y0 y1\n";
        return 1;
    }

    double x0 = std::strtod( argv[1], nullptr );
    double x1 = std::strtod( argv[2], nullptr );
    double y0 = std::strtod( argv[3], nullptr );
    double y1 = std::strtod( argv[4], nullptr );
    double rx = x1 - x0;
    double ry = y1 - y0;
    int NX = 100;
    int NY;
    if ( argc > 5 ) {
        NX = std::atoi( argv[5] );
    }

    if ( argc > 6 ) {
        NY = std::atoi( argv[6] );
    } else {
        NY = int(NX * (ry/rx));
    }

    double dx = rx/(NX-1);
    double dy = ry/(NY-1);
    int maxiter = 30;

    int i = 0;
    double x = x0;
    for ( ; i < NX ; x += dx, i++ ) {
        int j = 0;
        double y = y0;
        for ( ; j < NY ; y += dy, j++ ) {
            double n = f( complex(x, y), maxiter, 4 );

            std::cout << x << " " << y << " " << n/maxiter << "\n";
        }
    }

    return 0;
}
