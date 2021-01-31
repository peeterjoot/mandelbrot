#if !defined paulimatrix_defined
#define paulimatrix_defined

#include <complex>
#include "matrix22.h"

template <class D>
class paulimatrix : public matrix22<std::complex<D>>
{
    typedef std::complex<D> complex;
    typedef matrix22<complex> m;

//    paulimatrix<D>( complex a, complex b, complex c, complex d ) : m{a,b,c,d} {}

public:
    typedef std::array<D, 3> vector;

    paulimatrix<D>( ){}

// Pauli matrices:
//    x: {0,1,1,0};
//    y: {0,-i,i,0};
//    z: {1,0,0,-1};
    paulimatrix<D>( D x, D y, D z ) : m{z,x - complex(0,y), x + complex(0,y),-z} {}
    paulimatrix<D>( const vector & v ) : paulimatrix<D>( v[0], v[1], v[2] ) {}
    paulimatrix<D>( const m & b ) : m{b} {}

    D normsq() const {
        paulimatrix<D> m = this->times(*this);

        return m.c[0].real();
    }
};

#endif
