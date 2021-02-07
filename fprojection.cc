#include "myvec.h"

vec a{2,1,1};
vec b{1,0,-1};

/** Let's try a sequence mapping vector -> vector with a first order linear transformation and see what happens:

    x -> <a x b>_1 + c = a (x.b) + (a.x) b + x(a.b) + c
 */
int f( double cx, double cy, double cz, int maxiter, double outofbounds ) {
    int i = 0;
    vec c{cx,cy,cz};
    vec x = c;
    while ( i < maxiter ) {
        double n = x.dot(x);
        if ( n > outofbounds ) {
            break;
        }

        x = a * (x.dot(b)) + b * (x.dot(a)) + x * (a.dot(b)) + c;
        i++;
    }

    return i;
}

#if 0
int main() {
    std::cout << "\na = ";
    a.print();
    std::cout << "b = ";
    b.print();
    std::cout << "a.b = ";
    auto v = a.dot(b);
    std::cout << v << "\n";

    std::cout << "\na = ";
    a.print();
    std::cout << "(a *= 3) = ";
    a *= 3;
    a.print();

    std::cout << "\na = ";
    a.print();
    std::cout << "b = ";
    b.print();
    std::cout << "a.b = ";
    v = a.dot(b);
    std::cout << v << "\n";

    std::cout << "\na = ";
    a.print();
    std::cout << "b = ";
    b.print();
    std::cout << "(a += b) = ";
    a+=b;
    a.print();

    std::cout << "\na = ";
    a.print();
    std::cout << "b = ";
    b.print();
    std::cout << "a + b = ";
    vec c = a + b;
    c.print();

    std::cout << "\nb = ";
    b.print();
    std::cout << "b * 3 = ";
    vec d = b * 3;
    d.print();

    return 0;
}
#endif
