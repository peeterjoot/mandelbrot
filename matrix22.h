#if !defined matrix22_defined
#define matrix22_defined

#include <iostream>
#include <array>

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

    void mult( const matrix22<F> & a, const matrix22<F> & b ) {
        // temp to avoid aliasing issues if a = *this, or b = *this.
        array tmp{
             a.c[offset(1,1)] * b.c[offset(1,1)] + a.c[offset(1,2)] * b.c[offset(2,1)],
             a.c[offset(1,1)] * b.c[offset(1,2)] + a.c[offset(1,2)] * b.c[offset(2,2)],
             a.c[offset(2,1)] * b.c[offset(1,1)] + a.c[offset(2,2)] * b.c[offset(2,1)],
             a.c[offset(2,1)] * b.c[offset(1,2)] + a.c[offset(2,2)] * b.c[offset(2,2)] };

        c = tmp;
    }

    void add( const matrix22<F> & a, const matrix22<F> & b ) {
        for ( int i = 0 ; i < 4 ; i++ ) {
            c[i] = a.c[i] + b.c[i];
        }
    }

    void incr( const matrix22<F> & o ) {
        for ( int i = 0 ; i < 4 ; i++ ) {
            c[i] += o.c[i];
        }
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
