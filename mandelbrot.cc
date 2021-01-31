#include <iostream>
#include <cstdlib>

// Algorithm from here:
//    https://en.wikipedia.org/wiki/Mandelbrot_set

int f( double x, double y, int maxiter, double outofbounds );

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
            double n = f( x, y, maxiter, 4 );

            std::cout << x << " " << y << " " << n/maxiter << "\n";
        }
    }

    return 0;
}
