#include <iostream>
#include <cmath>
#include <bits/stdc++.h>
#include <algorithm>
#include <cstdio>
#include <ctime>
using namespace std;

vector <int> pf2;
map <unsigned long long, int> prQ; //map of primes

//prime factorization, return [primes, power] map (e.g. 48 = 2^4 * 3, returns map of elems [2,4] and [3,1])
map <unsigned  long long, unsigned  long long> prime_factor(unsigned long long n){
    map <unsigned  long long, unsigned  long long> tmp;
    unsigned long long p = 2;
    while(n >= p * p){
        if (n % p == 0){
            tmp[p]++;
            n = n / p;
        }
        else p++;
    }
    tmp[n]++;
    return tmp;
}


//returns seive (vector of ints) of prime number till max
vector<int> get_primes(int min, int max){
    vector<int> primes;
    char *sieve;
    sieve = new char[max/8+1];
    //pf2.resize(max,0);
    // Fill sieve with 1
    memset(sieve, 0xFF, (max/8+1) * sizeof(char));
    for(int x = min; x <= max; x++)
        if(sieve[x/8] & (0x01 << (x % 8))){
            primes.push_back(x);
            prQ[x]=1;

            // Is prime. Mark multiplicates.
            for(int j = 2*x; j <= max; j += x)
                sieve[j/8] &= ~(0x01 << (j % 8));

        }
    delete[] sieve;
    return primes;
}

//return greatest common divisor between __m and __n
unsigned long long _gcd(unsigned long long __m, unsigned long long __n){
    if (__n > __m){
        swap(__m, __n);
    }
    while (__n != 0){
        unsigned long long __t = __m % __n;
        __m = __n;
        __n = __t;
    }
    return __m;
}

//least common multiplier for a & b (ull)
unsigned long long lcm(unsigned long long a, unsigned long long b)
{
    unsigned long long temp = _gcd(a, b);
    unsigned long long tmp =temp ? (a / temp * b) : 0;
    //printf("\na:%d, b:%d, r:%d\n", a, b, tmp);
    return tmp;
}

//https://codereview.stackexchange.com/questions/155432/modular-exponentiation-in-c by Jerry Coffin
unsigned long long mul_mod(unsigned long long a, unsigned long long b, unsigned long long m) {
    if (m == 0) return a * b;
    unsigned long long r = 0;
    while (a > 0) {
        if (a & 1)
            if ((r += b) > m) r %= m;
        a >>= 1;
        if ((b <<= 1) > m) b %= m;
    }
    return r;
}
//modulus power, takes a - base, n - power, m - modulus (returns a^n mod m)
unsigned long long pow_m(unsigned long long a, unsigned long long n, unsigned long long m){
    unsigned long long r = 1;

    while (n > 0) {
        if (n & 1)
            r = mul_mod(r, a, m);
        a = mul_mod(a, a, m);
        n >>= 1;
    }
    return r;
}

//function to define sequence length, returns length
unsigned long long seq(int p0, int q0, unsigned long long M){

    unsigned long long lf = lcm(p0-1, q0-1); //least common mult of p-1 and q-1
    map <unsigned  long long, unsigned  long long> pf = prime_factor(lf); //lf prime factorization, returns [factor, power]
    unsigned long long D = 1; //D (=seq length at the end)

    for (auto &p : pf){ //for all prime factors of lcm(p-1, q-1)
        if (p.first == 2 && p.second > 2) D = lcm(D, (unsigned long long)pow(2, p.second-2)); //if its 2^n, n>2
        else D = lcm(D,(unsigned long long)pow(p.first, p.second-1)*(p.first-1)); //otherwise

    }


    unsigned long long x1 = (unsigned long long) pow((p0+q0)/2,2) % M; //calculate x1 = (p + q) / 2 mod M
    unsigned long long x1tc = x1 % M; //x1tc = x1 mod M


    map <unsigned  long long, unsigned  long long> pf23 = prime_factor(D); //takes prime factors of D [prime, factor]
    vector <unsigned  long long> tr; //store only primes

    for (auto &p:pf23){
        while (p.second--) {
            tr.push_back(p.first);
        }
    }
    //printf("%d\n", tr.back());
    //bool toGo = true; //toGo until we check all primes from bottom to end
    //while(toGo){
    for (const auto &p : tr){ //checking all primes from bottom to end
        //toGo = false; //initally false
        if (pow_m(x1, pow_m(2, D/p, lf), M) == x1tc){ //check if condition (x1^(2^D mod lamda(M)) = x1 mod M) is still true
            D /= p; // D = D / its prime
            //toGo = true; //set if this round yields any succesful division of D
        }
    }
    //}

    return D;
}

int main() {
    unsigned long long int iM, fM;
    cin >> iM; //lower bound of M
    cin >> fM; //upper bound of M

    int pMax = ((int)(sqrt(fM)))*2.9; //max number for seive
    vector<int> primes = get_primes(2, pMax); //returns vector of primes from 2 till pMax
    map <unsigned long long, int> checked; //checker of p q (to skip qp if pq is our result)
    unsigned long long int count = 0; //counter of all sequences starts from M1 till M2

    //printf("%d %d\n", primes.back(), pMax);
    pMax = pMax >> 2;
    //pMax =
    for (auto it = primes.begin(); it != primes.end(); it++){ //take all primes one by one
        if (*it < pMax) continue;
        if (((*it) & 0x3) == 3){ //check if prime = 3 mod 4

            unsigned long long beg; //starting M
            if ((iM % (*it)) != 0) beg = iM + ((*it) - (iM % (*it))); //find first M to start from
            else beg = iM; //find first M to start from

            //iterate from first M by prime till M2
            for (unsigned long long n = beg; n <= fM; n += (*it) ){

                int q = n / (*it); //M is always divisible by p
                if (prQ[q] == 1){ //check if q is prime
                    if ((q & 0x3) == 3) { //check if q = 3 mod 4
                        if (to_string(*it).length() == to_string(q).length()){ //check if it's the same digit length
                            if(_gcd((*it)-1,q-1) == 2){ //gcd(p-1, q-1) should be 2
                                if(checked[n] != q){ //if not previously checked - mark checked and count sequence
                                    checked[n] = *it;
                                    count+= seq(*it, q, n);

                                }
                            }
                        }
                    }
                }
            }
        }

    }

    printf("%llu", count);
    return 0;
}