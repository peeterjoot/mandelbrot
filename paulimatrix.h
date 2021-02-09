#if !defined paulimatrix_defined
#define paulimatrix_defined

#include <complex>
#include "matrix22.h"

/**
 * This is an implementation of a primitive 3D geometric algebra backend,
 * supporting vector multiply and addition operations.
 *
 * The implementation uses a complex 2x2 matrix to represent the multivector
 * elements (but only provides initialization capability for vector elements.)
 */
template <class D>
class paulimatrix : public matrix22<std::complex<D>>
{
    typedef std::complex<D> complex;
    typedef matrix22<complex> m;

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

    static paulimatrix<D> init03( const complex & s ) {
        m r{};
        r.c[0] = s;
        r.c[3] = s;

        return r;
    }

    D norm() const {
        paulimatrix<D> m = this->times(*this);

        return m.c[0].real();
    }

    paulimatrix<D> operator+(const paulimatrix<D>& b) {
        const m & a = (m&)*this;
        return a.plus((m&)b);
    }

    paulimatrix<D> operator-(const paulimatrix<D>& b) {
        const m & a = (m&)*this;
        return a.plus((m&)b, -1);
    }

    paulimatrix<D> & operator+=(const paulimatrix<D>& b) {
        m & a = (m&)*this;
        a.pluseq((m&)b);
        return a;
    }

    paulimatrix<D> & operator-=(const paulimatrix<D>& b) {
        m & a = (m&)*this;
        a.pluseq((m&)b, -1);
        return a;
    }

    paulimatrix<D> operator*(const paulimatrix<D>& b) {
        const m & a = (m&)*this;
        return a.times((m&)b);
    }

    paulimatrix<D> grade0() const {
        const m & a = (m&)*this;
        complex diag = a.trace().real()/2;

        return init03( diag );
    }

    paulimatrix<D> grade3() const {
        const m & a = (m&)*this;
        complex diag{0,a.trace().imag()/2};

        return init03( diag );
    }

    paulimatrix<D> grade03() const {
        const m & a = (m&)*this;
        complex diag = a.trace()/2.0;

        return init03( diag );
    }

    paulimatrix<D> grade12() const {
        m r = (m&)*this;
        complex diag = r.trace()/2.0;
        r.diagscale( - diag );

        return r;
    }
};

#endif
