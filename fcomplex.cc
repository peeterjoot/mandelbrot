#include <complex>

typedef std::complex<double> complex;

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
