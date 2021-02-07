#if !defined myvec_defined
#define myvec_defined

#include <array>
#include <iostream>
#include <numeric>

/**
  A most simplistic 3D vector.
 */
class vec {
    using T = double;
    using A = std::array<T, 3>;
    A c{};

public:
    vec( T a_ = 0, T b_ = 0, T c_  = 0) : c{a_, b_, c_} {}
    vec( const vec & b ) : c{b.c} {}

    T dot( const vec & b ) {
        T r = std::inner_product(c.begin(), c.end(), b.c.begin(), 0); 

        return r;
    }    

    vec & operator+=(const vec& b) {
        int i = 0;
        for ( auto & e : c ) {
            e += b.c[i];
            i++;
        }
        return *this;
    }

    vec operator+(const vec& b) const {
        vec r = *this;
        int i = 0;
        for ( auto & e : r.c ) {
            e += b.c[i];
            i++;
        }
        return r;
    }

    vec & operator*=(const T& s) {
        for ( auto & e : c ) {
            e *= s;
        }
        return *this;
    }

    vec operator*(const T& s) const {
        vec r = *this;
        for ( auto & e : r.c ) {
            e *= s;
        }
        return r;
    }

    void print() const {
        const char * sep = "{";
        for ( const auto & e : c ) {
            std::cout << sep << e;
            sep = ",";
        }
        std::cout << "}\n";
    }
};

#endif
