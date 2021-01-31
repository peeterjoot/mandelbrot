#include <complex>
#include <iostream>

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

// -1, 1

class points {
    #define NX 35
    #define NY 20
    //#define NX 3
    //#define NY 2
public:
    points( double x0, double x1, double y0, double y1 ) {
        double rx = x1 - x0;
        double ry = y1 - y0;
        double dx = rx/(NX-1);
        double dy = ry/(NY-1);

        int i = 0;
        double x = x0;
        //std::cout << "{\n";
        //const char * comma = "";
        for ( ; i < NX ; x += dx, i++ ) {
            int j = 0;
            double y = y0;
            for ( ; j < NY ; y += dy, j++ ) {
                auto n = f( complex(x, y), 30, 4 );

                //std::cout << comma << "{" << x << ", " << y << ", " << n << "}\n";
                std::cout << x << " " << y << " " << n << "\n";
                //comma = ", ";
            }
        }
        //std::cout << "}\n";
    }
};

int main()
{
    points p( -2.5, 1, -1, 1 );

    return 0;
}
