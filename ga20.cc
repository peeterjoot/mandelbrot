#include "paulimatrix.h"

int main()
{
    typedef std::complex<double> complex;
    typedef matrix22<complex> m;
    complex i{0,1};
    m a(1,2,3,4);
    m b(2,3,4,5);
    m c;

// Pauli matrices:
//    m e1{0,1,1,0};
//    m e2{0,-i,i,0};
//    m e3{1,0,0,-1};

    a.print();
    b.print();
    c.mult(a,b);
    c.print();

    typedef paulimatrix<double> pauli;
    pauli e1{1,0,0};
    pauli e2{0,1,0};
    pauli e3{0,0,1};
    e1.print();
    e2.print();
    e3.print();

    pauli x{1,2,3};
    x.print();
    std::cout << x.normsq() << "\n";

    return 0;
}
