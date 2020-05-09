#include <iostream>
#include <cmath>
#include <chrono>
#include "BigNumber.h"
#include "JacobiSymbol.h"

template <typename T>
constexpr T PowerModule( T b, T e, T m ) noexcept {
    const T id = 1;
    if ( e == 0 ) return id;
    b = b % m;
    if ( e == 1 || b == 0 ) return b;
    T result = id;
    while ( e > 0 ) {
        if ( e % 2 != 0 ) {
            result = (result * b) % m;
        }
        e /= 2;
        b = ( b * b ) % m;
    }
    return result;
}

bool solovay_strassen_test(const BigNum& a, const BigNum& n) noexcept {
    // jesli liczba 'a' jest wieksza niz n-1 to nie zachodzi twierdzenie
    if (a >= n) return false;

    // dwojka i liczby parzyste mozna sprawdzic ad hoc
    if ( n == 2 ) return true;
    if (n % 2 == 0) return false;

    const BigNum diff = PowerModule<BigNum>(a, (n-1)/2, n ) - jacobi_symbol(a, n);
    return (diff != 0) && (diff != n);

}

// Funkcja przeprowadza test Millera-Rabina sprawdzajacy pierwszosc liczby 'n' testujac dla zadanego 'a'
bool miler_rabin_test(BigNum a, const BigNum& n) noexcept {

    // jesli liczba 'a' jest wieksza niz n-1 to nie zachodzi twierdzenie
    if (a >= n) return false;

    // dwojka i liczby parzyste mozna sprawdzic ad hoc
    if ( n == 2 ) return true;
    if ( n % 2 == 0 ) return 0;

    // przedstawiamy liczbe (n-1) jako d*(2**r)
    BigNum a_curr(1), y, l(n - 1);
    int r = 0;
    bool stop = 0;

    //ustal r
    while ((l % 2) == 0) {
        ++r;
        l /= 2;
    }

    // dla kazdego r rozwazamy liczbe a_curr = a**(d*(2**r))
    for (; l > 0 || r--; l /= 2) {
        // sprawdzenie parzystosci ostatniego bitu do wzoru z szybkim potegowaniem
        if ((l % 2) == 1) {
            a_curr = (a_curr * a) % n;
        }
        else if (!stop && l == 0) {
            stop = 1;
            a = a_curr;
        }
        // podniesienie do kwadratu -> szybkie potegowanie
        y = (a * a) % n;
        if (y == 1 && a != 1 && a != n - 1) return true;
        a = y;
    }

    return a != 1;
  }

// Funkcja sprawdza, czy dana liczba typu BigNum jest pierwsza. W tym celu
// wykonuje kilka razy test Millera-Rabina
bool is_prime_mr(const BigNum &x) noexcept {
    // tablica liczb dla ktorych bedziemy testowac pierwszosci, wedle zrodel te liczby sa 'wartosciowe' dla testow
    std::vector<BigNum> ktab
        = { BigNum(2), BigNum(325), BigNum(9375), BigNum(28178), BigNum(450775), BigNum(9780504), BigNum(1795265022)};

    if (x < 2) return false;

    // dla kolejnych liczb z tablicy wykonuj test M-R
    for(const auto& i : ktab) {
        // jesli choc raz zanleziono swiadka zlozonosci mozna zwrocic falsz
        if (miler_rabin_test(i, x)) {
            return false;
        }
    }
    return true;
}

bool is_prime_ss(const BigNum &x) noexcept {
    // tablica liczb dla ktorych bedziemy testowac pierwszosci, wedle zrodel te liczby sa 'wartosciowe' dla testow
    std::vector<BigNum> ktab
            = { BigNum(2), BigNum(325), BigNum(9375), BigNum(28178), BigNum(450775), BigNum(9780504), BigNum(1795265022)};

    if (x < 2) return false;

    // dla kolejnych liczb z tablicy wykonuj test M-R
    for(const auto& i : ktab) {
        // jesli choc raz zanleziono swiadka zlozonosci mozna zwrocic falsz
        if (solovay_strassen_test(i, x)) {
            return false;
        }
    }
    return true;
}


int main() {


    int zero;
    std::cin >> zero;
    BigNum ten(1);
    for(int i = 0; i < zero; ++i){
        ten *= 10;
    }
    ten += 9;

/*
    std::string str;
    std::cout << "Podaj liczbe:\n";
    std::cin >> str;
*/

    BigNum n(0);
    n = ten;

    // Miler Rabin
    auto start_time = std::chrono::high_resolution_clock::now();

    if(is_prime_mr(n)) std::cout << "PIERWSZA zdaniem MR" << std::endl;
    else std::cout << "ZLOZONA zdaniem MR" << std::endl;;

    auto end_time = std::chrono::high_resolution_clock::now();
    std::cout << "Czas MR: " << std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count() << "s" << std::endl;

    // Solovay Strassen
    start_time = std::chrono::high_resolution_clock::now();

    if(is_prime_ss(n)) std::cout << "PIERWSZA zdaniem SS" << std::endl;
    else std::cout << "ZLOZONA zdaniem SS" << std::endl;;

    end_time = std::chrono::high_resolution_clock::now();
    std::cout << "Czas SS: " << std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count() << "s" << std::endl;

    return 0;
}
