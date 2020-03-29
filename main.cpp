#include <iostream>
#include <cmath>
#include <chrono>
#include "BigNumber.h"

// Funkcja przeprowadza test Millera-Rabina sprawdzajacy pierwszosc liczby 'n' testujac dla zadanego 'a'
bool miler_rabin_test(BigNum a, BigNum n) {

    // jesli liczba 'a' jest wieksza niz n-1 to nie zachodzi twierdzenie
    if (a >= n) return false;

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
bool is_prime(const BigNum &x) {
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



int main() {


    int zero;
    scanf("%d", &zero);
    BigNum ten(1);
    for(int i = 0; i < zero; ++i){
        ten *= 10;
    }
    ten += 9;
    ten.write();

    //std::string str;
    //std::cout << "Podaj liczbe:\n";
    //std::cin >> str;



    BigNum n(0);
    n = ten;

    std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();

    if(is_prime(n)) std::cout << "PIERWSZA" << std::endl;
    else std::cout << "ZLOZONA\n";

    std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now();
    std::cout << "Czas: " << (std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count()) / 1000 << std::endl;

    return 0;
}
