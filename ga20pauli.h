#if !defined ga20_defined
#define ga20_defined

#include "matrix22.h"

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

    ga20( ){}

// Pauli matrices:
//    x: {0,1,1,0};
//    z: {1,0,0,-1};
    ga20( D x, D z ) : m{z,x, x,-z} {}
    ga20( const vector & v ) : ga20( v[0], v[1] ) {}
    ga20( const m & b ) : m{b} {}

    D norm() const {
        ga20 m = this->times(*this);

        return m.c[0];
    }

    ga20 operator+(const ga20& b) {
        const m & a = (m&)*this;
        return a.plus((m&)b);
    }

    ga20 & operator+=(const ga20& b) {
        m & a = (m&)*this;
        return a.pluseq((m&)b);
    }

    ga20 operator*(const ga20& b) {
        const m & a = (m&)*this;
        return a.times((m&)b);
    }
};

#endif
