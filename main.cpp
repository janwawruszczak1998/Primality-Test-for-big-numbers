#include <iostream>
#include <cmath>
#include "BigNumber.h"

int max_power(BigNum& number){
    int max_pow = 1;

    while( (number - 1) % pow(2, max_pow) == 0   ) { max_pow++; }

    return max_pow - 1;
}

BigNum quick_power_modulo(BigNum x, BigNum n, BigNum& mod){
    if(n == 1){
        return x;
    }

    if(n % 2 == 0){
        return ( (quick_power_modulo(x, n / 2, mod) % mod)*((quick_power_modulo(x, n / 2, mod)) % mod)) % mod;
    }
    else{
        return ((x % mod) * ((quick_power_modulo(x, n / 2, mod) % mod)*(quick_power_modulo(x, n / 2, mod) % mod)) % mod) % mod;
    }
}


void miler_rabin_test(BigNum& n){

    std::cout << "TESTING NUMBER ";
    n.write();
    std::vector<BigNum> ktab = { BigNum(2), BigNum(325), BigNum(9375), BigNum(28178), BigNum(450775), BigNum(9780504), BigNum(1795265022)};
    if(n == 2){
        std::cout << "PIERWSZA\n";
        return;
    }
    else if (n % 2 == 0){
        std::cout << "ZLOZONA\n";
        return;
    }


    int t = max_power(n);
    BigNum s = (n-1) / pow(2, t);

    for(int i = 0; i < ktab.size(); ++i){
        std::cout << "ukonczono " << (int) ( i * 100 / ktab.size()) << " procent\n";
        BigNum a = ktab[i];
        if( quick_power_modulo(a, (n-1), n) != 1)
        {
            std::cout << "ZLOZONA\n";
            return;
        }
        BigNum a_curr = quick_power_modulo(a, n-1, n);

        if(a_curr == 1) continue;
        BigNum a_next = quick_power_modulo (a, BigNum(2), n);
        while (a_next != 1){
            a_curr = a_next;
            a_next *= a_next;
            a_next %= n;
        }
        if(a_curr != (n - 1)){
            std::cout << "ZLOZONA\n";
            return;
        }
    }
    std::cout << "PIERWSZA\n";
}


int main() {


    BigNum n(104729);
    miler_rabin_test(n);
    /*
    BigNum mod(7283);
    std::string str;
    std::cin >> str;
    BigNum dwa(123);
    dwa = str;
    std::cin >> str;
    mod = str;


    dwa ^= n;
    dwa %= n;

    ((quick_power(mod, n)) % n ).write();

    if()
*/

    return 0;
}