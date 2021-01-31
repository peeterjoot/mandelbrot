#include "paulimatrix.h"

typedef paulimatrix<double> matrix;
matrix e{1, 0, 0};

int f( double cx, double cy, double cz, int maxiter, double outofbounds ) {
    
    matrix c{cx, 0, cy};
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
