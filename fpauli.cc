#include "paulimatrix.h"

typedef paulimatrix<double> matrix;
matrix e{1, 0, 0};

int f( double x, double y, int maxiter, double outofbounds ) {
    
    matrix c{x, 0, y};
    matrix z = c;

    int i = 0;
    while ( i < maxiter ) {
        double n = z.norm();

        if ( n > outofbounds ) {
            break;
        }

        matrix zp = z.times(e);
        zp = zp.times(z);
        zp.pluseq(c);

        z = zp;
        i++;
    }

    return i;
}
