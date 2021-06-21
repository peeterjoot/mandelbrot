#if !defined paulimatrix_defined
#define paulimatrix_defined

#include <complex>
#include "matrix22.h"
#include "quirks.h"

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

#if defined INLINE_TEMPLATE_PARAMS_NOT_ALLOWED
#define PAULIMATRIXD paulimatrix
#else
#define PAULIMATRIXD paulimatrix<D>
#endif
    PAULIMATRIXD( ){}

// Pauli matrices:
//    x: {0,1,1,0};
//    y: {0,-i,i,0};
//    z: {1,0,0,-1};
    PAULIMATRIXD( D x, D y, D z ) : m{z,x - complex(0,y), x + complex(0,y),-z} {}
    PAULIMATRIXD( const vector & v ) : PAULIMATRIXD( v[0], v[1], v[2] ) {}
    PAULIMATRIXD( const m & b ) : m{b} {}

    static PAULIMATRIXD init03( const complex & s ) {
        m r{};
        r.c[0] = s;
        r.c[3] = s;

        return r;
    }

    D norm() const {
        PAULIMATRIXD m = this->times(*this);

        return m.c[0].real();
    }

    PAULIMATRIXD operator+(const PAULIMATRIXD& b) {
        const m & a = (m&)*this;
        return a.plus((m&)b);
    }

    PAULIMATRIXD operator-(const PAULIMATRIXD& b) {
        const m & a = (m&)*this;
        return a.plus((m&)b, -1);
    }

    PAULIMATRIXD & operator+=(const PAULIMATRIXD& b) {
        m & a = (m&)*this;
        a.pluseq((m&)b);
        return a;
    }

    PAULIMATRIXD & operator-=(const PAULIMATRIXD& b) {
        m & a = (m&)*this;
        a.pluseq((m&)b, -1);
        return a;
    }

    PAULIMATRIXD operator*(const PAULIMATRIXD& b) {
        const m & a = (m&)*this;
        return a.times((m&)b);
    }

    PAULIMATRIXD grade0() const {
        const m & a = (m&)*this;
        complex diag = a.trace().real()/2;

        return init03( diag );
    }

    PAULIMATRIXD grade3() const {
        const m & a = (m&)*this;
        complex diag{0,a.trace().imag()/2};

        return init03( diag );
    }

    PAULIMATRIXD grade03() const {
        const m & a = (m&)*this;
        complex diag = a.trace()/2.0;

        return init03( diag );
    }

    PAULIMATRIXD grade12() const {
        m r = (m&)*this;
        complex diag = r.trace()/2.0;
        r.diagscale( - diag );

        return r;
    }
};

#endif
