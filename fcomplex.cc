#include <complex>

typedef std::complex<double> complex;

/** This function implements a 2D escape time algorithm for the Mandelbrot set using traditional complex number operations. */
int f( double cx, double cy, double cz, int maxiter, double outofbounds ) {
    complex c{cx,cy};
    complex z = c;
    int i = 0;
    while ( i < maxiter ) {
        double n = std::norm(z);
        if ( n > outofbounds ) {
            break;
        }

        z = z * z + c;
        i++;
    }

    return i;
}
