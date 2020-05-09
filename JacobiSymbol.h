//
// Created by jan on 5/9/20.
//

#ifndef PRIMES_JACOBISYMBOL_H
#define PRIMES_JACOBISYMBOL_H



template <typename T>
constexpr inline int Ep( const T& n ) noexcept {
    if ( n % 4 == 1 ) return 1;
    return -1;
}

template <typename T>
constexpr inline int Omega( const T& n ) noexcept {
    if ( n % 8 == 1 || n % 8 == 7 ) return 1;
    return -1;
}

template <typename T>
constexpr inline int Theta( const T& m, const T& n ) noexcept {
    if ( m % 4 == 1 || n % 4 == 1 ) return 1;
    return -1;
}


// funkcja oblicza symbol jacobiego
template <typename T>
constexpr int jacobi_symbol(T a, T n) noexcept {
    int result = 1;
    T tmp;
    if ( a >= 0 ) {
        result = 1;
    }
    else {
        a.is_negative = !a.is_negative;
        result = Ep(n);
    }
    while ( a != 1 && a != 0 ) {
        if ( a % 2 == 0 ) {
            a /= 2;
            result *= Omega( n );
        } else {
            result *= Theta( a, n );
            tmp = a;
            a = n % a;
            n = tmp;
        }
    }
    if ( a == 1 ) return result;
    else return 0;
}

#endif //PRIMES_JACOBISYMBOL_H
