#include <iostream>

#include "BigNumber.h"


void miler_rabin_test(BigNum& n){

    std::vector<BigNum> ktab = { BigNum(2), BigNum(325), BigNum(9375), BigNum(28178), BigNum(450775), BigNum(9780504), BigNum(1795265022)};
    if(n == 2){
        printf("PIERWSZA\n");
    }
    else if (n % 2 == 0){
        printf("ZLOZONA\n");
    }

    BigNum tmp(n), log(0), m(2);
    tmp -= 1;                       //tmp = n - 1, m to najwieksza liczba taka, ze 2^log * costam < tmp, a log to logarytm dwojkowy z n, czyli liczba sprawdzen w tescie dla kolejnych 2^i
    while(log <= tmp){
        log += 1;
        m *= 2;
    }
    m /= 2;

    for(int i = 0; i < ktab.size(); ++i){
        BigNum act = ktab[i];
        if(act > n) break; // liczba z tablicy jest wieksza niz badana
        BigNum prev_power(0), power(1);
        for(BigNum i(0); i < log; i += 1){
            prev_power = power;
            power *= power;
            power %= n;
            if(power == 1 && !(prev_power == 1) && !(prev_power == tmp)){
                printf("ZLOZONA\n");
            }

            if( (1 << i.array_of_digits[0]) & n.array_of_digits[0]) {
                power *= act;
                power %= n;
            }
        }
        if( !(power == 1) ){
            printf("NO\n");
            return;
        }
        else{
            printf("NIEWIEM\n");
            return;
        }
    }

}

int main() {

    BigNum a(0);
    std::string str;
    std::cin >> str;


    miler_rabin_test(a);

    return 0;
}