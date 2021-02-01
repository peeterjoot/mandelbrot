#include "ga20pauli.h"

typedef ga20<double> matrix;
matrix e{1, 0};

/** This function implements a 2D escape time algorithm for the Mandelbrot set using
 * the vector equivalent of the traditional complex number specification.
 *
 * Under the covers, this is using a 2x2 real matrix to represent the vectors and their products and sums.
 */
int f( double cx, double cy, double cz, int maxiter, double outofbounds ) {

    matrix c{cx, cy};
    matrix z = c;

    int i = 0;
    while ( i < maxiter ) {
        double n = z.norm();

        if ( n > outofbounds ) {
            break;
        }

        matrix p = z * e * z;
        z = p + c;
        i++;
    }

    return i;
}
