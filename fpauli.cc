#include "paulimatrix.h"

typedef paulimatrix<double> matrix;
matrix e{1, 0, 0};

/** This function implements a #D escape time algorithm for the Mandelbrot set using
 * the vector equivalent of the traditional complex number specification (which also extends
 * the Mandlebrot set off the z=0 plane.)
 *
 * Under the covers, this is using a 2x2 complex matrix to represent the vectors and their products and sums.
 */
int f( double cx, double cy, double cz, int maxiter, double outofbounds ) {

    matrix c{cx, cy, cz};
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
