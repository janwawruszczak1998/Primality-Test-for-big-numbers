#ifndef PRIMES_BIGNUMBER_H
#define PRIMES_BIGNUMBER_H

#include <cstdio>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>


// Standardowe makra skracajace kod
#define FOR(x, b, e) for(int x = b; x <= (e); ++x)
#define FORD(x, b, e) for(int x = b; x >= (e); --x)
#define REP(x, n) for(int x = 0; x < (n); ++x)


// Implementacja klasy BigNum, realizujacej arytmetyke wielkich liczb - element projektu OiAK
class BigNum {
public:
    // Makro słuzace do eliminowania wiodacych zer
    #define REDUCE() while(len>1 && !array_of_digits[len-1]) len--;

    // Podstawa, przy której wykonywane sa obliczenia oraz liczba zer w podstawie
    static const int BASE = 1000000000, BD = 9;

    // Zmienna len reprezentuje aktualna długosc liczby (liczbe cyfr), a al
    // wielkosc zaalokowanej pamieci do przechowywania cyfr liczby
    int len, al;

    // Wskaznik do reprezentacji liczby w tablicy
    int *array_of_digits;

    // Konstruktor liczby o wartosci v i zaalokowanej pamieci dla l cyfr
    BigNum(int v = 0, int l = 2) : len(1), al(l), array_of_digits(new int[l]) {
        REP(x, al) array_of_digits[x] = 0;
        if ((array_of_digits[0] = v) >= BASE) carry(1);
    }

    // Konstruktor, przypisujacy wartosc innej liczby typu BigNum
    BigNum(const BigNum &a) : len(a.len), al(len), array_of_digits(new int[al]) {
        REP(x, al) array_of_digits[x] = a.array_of_digits[x];
    }

    // Destruktor
    ~BigNum() {
        delete[] array_of_digits;
    }

    // Funkcja przyjmuje jako parametr zapotrzebowanie na liczbe cyfr i jesli
    // zapotrzebowanie jest wieksze od aktualnego rozmiaru tablicy cyfr, to dokonuje
    // realokacji
    void Res(int l) {
        if (l > al) {
            int *n = new int[l = std::max(l, 2 * al)];
            REP(x, l) n[x] = x >= al ? 0 : array_of_digits[x];
            delete[] array_of_digits;
            array_of_digits = n;
            al = l;
        }

    }

    // Funkcja dokonuje przenoszenia do starszych cyfr nadmiaru powstałego na skutek
    // wykonywania operacji. Parametr okresla liczbe cyfry, do której nalezy
    // wykonac przenoszenie nadmiaru
    void carry(int p) {
        int x = 0;
        for (; x < p || array_of_digits[x] < 0 || array_of_digits[x] >= BASE; x++) {
            Res(x + 2);
            // W razie potrzeby wykonaj zapozyczenie ze starszej pozycji
            if (array_of_digits[x] < 0) {
                int i = (-array_of_digits[x] - 1) / BASE + 1;
                array_of_digits[x] += i * BASE;
                array_of_digits[x + 1] -= i;
            } else
            // lub wykonaj przeniesienie powstałego nadmiaru
            if (array_of_digits[x] >= BASE) {
                int i = array_of_digits[x] / BASE;
                array_of_digits[x] -= i * BASE;
                array_of_digits[x + 1] += i;
            }
        }
        len = std::max(len, x + 1);
        REDUCE();
    }


    // Operatory porównawcze na wielkich liczbach
    bool operator == (const BigNum &a) const {
        if (a.len != len) return false;
        REP(x, len) if (array_of_digits[x] != a.array_of_digits[x]) return false;
        return true;
    }

    bool operator < (const BigNum &a) const {
        if (len != a.len) return len < a.len;
        int x = len - 1;
        while (x && a.array_of_digits[x] == array_of_digits[x]) x--;
        return array_of_digits[x] < a.array_of_digits[x];
    }

    bool operator > (const BigNum &a) const {
        return a < *this;
    }

    bool operator <= (const BigNum &a) const {
        return !(a < *this);
    }

    bool operator >= (const BigNum &a) const {
        return !(*this < a);
    }

    bool operator != (const BigNum &a) const {
        return !(*this == a);
    }

    // Operacje dla liczb typu int na wielkich liczbach
    BigNum &operator=(int a) {
        REP(x, len) array_of_digits[x] = 0;
        len = 1;
        if (array_of_digits[0] = a >= BASE) carry(1);
        return *this;
    }

    void operator+=(int a) {
        array_of_digits[0] += a;
        carry(1);
    }

    void operator-=(int a) {
        array_of_digits[0] -= a;
        carry(1);
    }

    void operator*=(int a) {
        REP(x, len) {
            long long tmp = array_of_digits[x];
            tmp *= a;
            array_of_digits[x] = tmp % BASE;
            array_of_digits[x + 1] += tmp / BASE;
        }
        carry(len);
    }

    void operator^=(int a) {
        BigNum c = *this;
        REP(x, a - 1){
            *this *= c;
        }
    }

    int operator/=(int a) {
        long long w = 0;
        FORD(p, len - 1, 0) {
            w = w * BASE + array_of_digits[p];
            array_of_digits[p] = w / a;
            w %= a;
        }
        REDUCE();
        return w;
    }

    int operator%(int a) {
        long long w = 0;
        FORD(p, len - 1, 0) w = (w * BASE + array_of_digits[p]) % a;
        return w;
    }

    // Operacje arytmetyncze na wielkich liczbach
    BigNum& operator += (const BigNum &a) {
        Res(a.len);
        REP(x, a.len) array_of_digits[x] += a.array_of_digits[x];
        carry(a.len);
        return *this;
    }

    BigNum& operator -= (const BigNum &a) {
        REP(x, a.len) array_of_digits[x] -= a.array_of_digits[x];
        carry(a.len);
        return *this;
    }

    BigNum& operator *= (const BigNum &a) {
        BigNum c(0, len + a.len);
        REP(x, a.len) {
            REP(y, len) {
                long long prod = a.array_of_digits[x];
                prod *= array_of_digits[y];

                c.array_of_digits[y + x] += prod % BASE;

                if (prod >= BASE) {
                    long long i = prod / BASE;
                    c.array_of_digits[y + x + 1] += i;
                }
                c.carry(len + x);
            }
        }
        *this = c;
        return *this;
    }


    BigNum& operator /= (const BigNum &a) {
        int n = std::max(len - a.len + 1, 1);
        BigNum quotient(0, n), prod(0);
        FORD(i, n - 1, 0) {
            int l = 0, r = BASE - 1;
            while (l < r) {
                int m = (l + r + 1) / 2;
                if (*this < prod + ((a * m) << i)) r = m - 1;
                else l = m;
            }
            prod += a * l << i;
            quotient.array_of_digits[i] = l;
            if (l) quotient.len = std::max(quotient.len, i + 1);
        }
        *this = quotient;
        return *this;
    }

    BigNum& operator %= (const BigNum &a) {

        BigNum v = *this;
        //v.write();
        //a.write();
        v /= a;
        //v.write();
        v *= a;
        *this -= v;
        while( *this >= a ) *this -= a;

        return *this;
    }

    //operator przypisania kopiujacy
    BigNum& operator = (const BigNum &a) {
        Res(a.len);
        FORD(x, len - 1, a.len) array_of_digits[x] = 0;
        REP(x, a.len) array_of_digits[x] = a.array_of_digits[x];
        len = a.len;
        return *this;
    }

// Funkcje słuzace do wczytywania i wypisywania liczb

    // Funkcja przypisuje liczbie BigNum wartosc liczby z napisu zapisanego przy
    // podstawie 10
    BigNum& operator=(std::string a) {
        int s = a.length();
        *this = 0;
        Res(len = s / BD + 1);
        REP(x, s) array_of_digits[(s - x - 1) / BD] = 10 * array_of_digits[(s - x - 1) / BD] + a[x] - '0';
        REDUCE();
        return *this;
    }

    // Funkcja wypisuje wartosc liczby BigNum zapisanej przy podstawie 10
    void write() const {
        printf("%d", int(array_of_digits[len - 1]));
        FORD(x, len - 2, 0) printf("%0*d", BD, int(array_of_digits[x]));
        printf("\n");
    }


    // Operator przesuniecia w prawo o n cyfr
    BigNum &operator>>=(int n) {
        if (n >= len) n = len;
        REP(x, len - n) array_of_digits[x] = array_of_digits[x + n];
        FOR(x, len - n, n) array_of_digits[x] = 0;
        len -= n;
        if (len == 0) len = 1;
        return *this;
    }

    // Operator przesuniecia w lewo
    BigNum &operator<<=(int n) {
        if (array_of_digits[0] == 0 && len == 1) return *this;
        Res(len + n);
        FORD(x, len - 1, 0) array_of_digits[x + n] = array_of_digits[x];
        REP(x, n) array_of_digits[x] = 0;
        len += n;
        return *this;
    }



    BigNum operator +(const BigNum &a) const {BigNum w = *this; w += a; return w;}
    BigNum operator -(const BigNum &a) const {BigNum w=*this; w -= a; return w; }
    BigNum operator *(const BigNum &a) const {BigNum w=*this; w *= a; return w; }
    BigNum operator /(const BigNum &a) const {BigNum w=*this; w /= a; return w; }
    BigNum operator %(const BigNum &a) const {BigNum w=*this; w %= a; return w; }
    BigNum operator ^(int a) {BigNum w = *this; w ^= a; return w;}
    BigNum operator <<(int a) {BigNum w = *this; w <<= a; return w; }
    BigNum operator >>(int a) {BigNum w = *this; w >>= a; return w; }

};

#endif //PRIMES_BIGNUMBER_H
