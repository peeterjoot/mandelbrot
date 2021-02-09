#include "paulimatrix.h"

typedef paulimatrix<double> matrix;
matrix e1{1, 1, 1};
matrix e2{1, 0, 0};
matrix e3{1, -1, 0};

/** This function implements a 3D escape time algorithm for what is hopefully an interesting Mandelbrot like set using
 * the following iteration:

\begin{equation}
\begin{aligned}
   \Bx &\rightarrow \gpgradeone{ \Ba \Bx \Bb \Bx \Bc } + \Bd \\
       &= \lr{ \Ba \cdot \Bx } \lr{ \Bb \cross \lr{ \Bc \cross \Bx } }
         +\lr{ \Ba \cross \Bx } \lr{ \Bb \cdot \lr{ \Bc \cross \Bx } }
         + \Bd
\end{aligned}
\end{equation}

 * Under the covers, this is using a 2x2 complex matrix to represent the vectors, their products, sums, and
 * grade selection.
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

        // shouldn't have bivector grades, so this effectively selects grade1 only:
        z = (e1 * z * e2 * z * e3).grade12() + c;
        i++;
    }

    return i;
}
