#if !defined matrix22_defined
#define matrix22_defined

#include <iostream>
#include <array>

/** This is a simple implementation of a generic 2x2 matrix class
 * supporting multiplication, addition and negation operations.
 *
 * The matrix elements may be real or complex, as desired.
 */
template <class F>
class matrix22 {
    typedef std::array< F, 4 > array;

    static inline constexpr int offset( int r, int c ) {
        // 1,1 => 0
        // 1,2 => 1
        // 2,1 => 2
        // 2,2 => 3
        return (r-1) * 2 + c-1;
    }

protected:
    array c{};

public:
    matrix22<F>(){}
    matrix22<F>( F c11, F c12, F c21, F c22 ) : c{ c11, c12, c21, c22 } {}
    matrix22<F>( const matrix22<F> & ) = default;

    void pluseq(const matrix22<F>& b, const F s = 1) {
        int i = 0;
        for ( auto & e : c ) {
            e += s * b.c[i];
            i++;
        }
    }

    void diagscale( const F & s ) {
        c[0] += s;
        c[3] += s;
    }

    matrix22<F> plus(const matrix22<F>& b, const F s = 1) const {
        matrix22<F> r;

        int i = 0;
        for ( auto & e : r.c ) {
            e = c[i] + s * b.c[i];
            i++;
        }

        return r;
    }

    matrix22<F> negate() const {
        matrix22<F> r;

        int i = 0;
        for ( auto & e : r.c ) {
            e = -c[i];
            i++;
        }

        return r;
    }

    matrix22<F> times(const matrix22<F>& b) const {
        const matrix22<F> & a = *this;
        matrix22<F> r{
             a.c[offset(1,1)] * b.c[offset(1,1)] + a.c[offset(1,2)] * b.c[offset(2,1)],
             a.c[offset(1,1)] * b.c[offset(1,2)] + a.c[offset(1,2)] * b.c[offset(2,2)],
             a.c[offset(2,1)] * b.c[offset(1,1)] + a.c[offset(2,2)] * b.c[offset(2,1)],
             a.c[offset(2,1)] * b.c[offset(1,2)] + a.c[offset(2,2)] * b.c[offset(2,2)] };

        return r;
    }

    F trace() const {
        return c[0] + c[3];
    }

    void print( ) const {
        std::cout << "{{ "
                  << c[offset(1,1)] << ", "
                  << c[offset(1,2)] << " }{ "
                  << c[offset(2,1)] << " , "
                  << c[offset(2,2)] << " }}\n";
    }
};

#endif
