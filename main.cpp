#include <iostream>
#include <cmath>
#include "BigNumber.h"

int max_power(BigNum& number){
    int max_pow = 1;

    while( (number - 1) % pow(2, max_pow) == 0   ) { max_pow++; }

    return max_pow - 1;
}

BigNum quick_power(BigNum x, BigNum n){
    if(n == 1){
        return x;
    }

    if(n % 2 == 0){
        return (quick_power(x, n / 2)*quick_power(x, n / 2));
    }
    else{
        return (x * quick_power(x, n / 2)*quick_power(x, n / 2));
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
        BigNum a = ktab[i];

        if( ((a ^ (n-1)) % n) != 1){
            std::cout << "ZLOZONA\n";
            return;
        }
        std::cout << "a\n";
        BigNum a_curr( (a ^ s)  );
        a_curr.write();
        a_curr %= n;
        a_curr.write();

        std::cout << "b\n";
        BigNum test( (quick_power(a, s)) );
        test.write();
        test %= n;
        test.write();


        if(a_curr == 1) continue;
        BigNum a_next( (a_curr^BigNum(2)) % n);
        while (a_next != 1){
            a_curr = a_next;
            a_next *= a_next;
            a_next %= n;
            /*
            std::cout << "n: ";
            n.write();
            std::cout << "a_curr: ";
            a_curr.write();
            std::cout << "a_next: ";
            a_next.write();
             */
        }
        if(a_curr != (n - 1)){
            std::cout << "ZLOZONA\n";
            return;
        }
    }
    std::cout << "PIERWSZA\n";
}


int main() {


    BigNum n(523  );
    //miler_rabin_test(n);
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


    return 0;
}