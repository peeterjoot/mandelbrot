#if !defined paulimatrix_defined
#define paulimatrix_defined

#include <complex>
#include "matrix22.h"

template <class D>
class paulimatrix : public matrix22<std::complex<D>>
{
    typedef std::complex<D> complex;
    typedef matrix22<complex> m;

    paulimatrix<D>( complex a, complex b, complex c, complex d ) : m{a,b,c,d} {}

public:
    paulimatrix<D>( ){}

    paulimatrix<D>( D x, D y, D z ) : m{z,x - complex(0,y), x + complex(0,y),-z} {}

/*
    static paulimatrix<D> paulix( D scale = 1 ) {
        return paulimatrix<D>{scale, 0, 0};
    } 

    static paulimatrix<D> pauliy( D scale = 1 ) {
        return paulimatrix<D>{0, scale, 0};
    } 

    static paulimatrix<D> pauliz( D scale = 1 ) {
        return paulimatrix<D>{0, 0, scale};
    } 
*/

    D normsq() const {
        paulimatrix<D> m{};
        m.mult(*this, *this);

        return m.c[0].real();
    }
};

#endif
