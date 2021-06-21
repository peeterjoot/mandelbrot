#if !defined ga20_defined
#define ga20_defined

#include "matrix22.h"
#include "quirks.h"

/**
 * This is an implementation of a primitive 2D geometric algebra backend,
 * supporting vector multiply and addition operations.
 *
 * The implementation uses a real 2x2 matrix to represent the multivector
 * elements (but only provides initialization capability for vector elements.)
 */
template <class D>
class ga20 : public matrix22<D>
{
    typedef matrix22<D> m;

public:
    typedef std::array<D, 2> vector;

#if defined INLINE_TEMPLATE_PARAMS_NOT_ALLOWED
#define GA20D ga20
#else
#define GA20D ga20<D>
#endif
    GA20D( ){}

// Pauli matrices:
//    x: {0,1,1,0};
//    z: {1,0,0,-1};
    GA20D( D x, D z ) : m{z,x, x,-z} {}
    GA20D( const vector & v ) : GA20D( v[0], v[1] ) {}
    GA20D( const m & b ) : m{b} {}

    D norm() const {
        GA20D m = this->times(*this);

        return m.c[0];
    }

    GA20D operator+(const GA20D& b) {
        const m & a = (m&)*this;
        return a.plus((m&)b);
    }

    GA20D & operator+=(const GA20D& b) {
        m & a = (m&)*this;
        return a.pluseq((m&)b);
    }

    GA20D operator*(const GA20D& b) {
        const m & a = (m&)*this;
        return a.times((m&)b);
    }
};

#endif
