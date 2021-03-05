#include <iostream>
#include "nbtheory.h"
#include "osrng.h"
#include "PublicKeyAlgorithmBox.h"

using namespace CryptoPP;
using namespace std;

// #EEA()
bool PublicKeyAlgorithmBox::EEA(const Integer& a, const Integer& b,
		Integer& d, Integer& x, Integer& y) {
    Integer x1, y1, d1;

    if(b == 0) {
        d = a;
        x = 1;
        y = 0;
    }
    else {
        EEA(b, a % b, d1, x1, y1);
        d = d1;
        x = y1;
        y = x1 - (a / b) * y1;
        return true;
    }
    return false;
}

// #modularExponentation()
Integer PublicKeyAlgorithmBox::modularExponentation(const Integer& a,
		const Integer& b, const Integer& n) {

    Integer d = 1;

    for(int i = b.BitCount() - 1; i >= 0; i--){
        d = (d * d) % n;

        if(b.GetBit(i) == 1)
            d = (d * a) % n;

    }

    return d;
} // modularExponentation()

// #multInverse()
bool PublicKeyAlgorithmBox::multInverse(const Integer& a, const Integer& n,
		Integer& a_inv) {

    Integer d, x, y;

    EEA(a, n, d, x, y);

    if (d != 1)
        return false;
    else {
        a_inv = (x + n) % n;    //Positives Inverses zuweisen
        return true;
    }
} // multInverse()

// #witness()
bool PublicKeyAlgorithmBox::witness(const Integer& a, const Integer& n) {

    Integer d = 1, x = 0;
    Integer n_minus_one = n - 1;

//    cout << d << endl;
//    cout << n << endl;
//    cout << n_minus_one << endl;
//    cout << a << endl;

    for(int i = n_minus_one.BitCount(); i >= 0; i--){
        x = d;
        d = (d * d) % n;

        if(d == 1 && x != 1 && x != n_minus_one)
            return true;

        if(n_minus_one.GetBit(i) == 1)
            d = (d * a) % n;
    }

    if(d != 1) return true;

    return false;
} // witness()

// randomInteger()
Integer PublicKeyAlgorithmBox::randomInteger(const size_t size) {

    NonblockingRng NonblockingRng;

    Integer random;
    random.Randomize(NonblockingRng, size);

    return random;
}

// randomInteger()
Integer PublicKeyAlgorithmBox::randomInteger(const Integer& from, const Integer& to) {

    NonblockingRng NonblockingRng;

    Integer random;
    random.Randomize(NonblockingRng, from, to);

    return random;
}

// #millerRabinTest()
bool PublicKeyAlgorithmBox::millerRabinTest(Integer& n, unsigned int s) {

    for(int i = 1; i <= s; i++){
        Integer a = randomInteger(2, n - 1);

        if(witness(a, n))
            return false;
    }

	return true;
} // millerRabinTest()

// #randomPrime()
unsigned int PublicKeyAlgorithmBox::randomPrime(Integer &p,
		unsigned int bitlen, unsigned int s) {

    bool primeFound = false;
    int tries = 0;

    while(!primeFound){
        tries++;
        Integer potentialPrime = randomInteger(bitlen);

        if(millerRabinTest(potentialPrime, s)){
            p = potentialPrime;
            primeFound = true;
        }
    }
	return tries;
} // randomPrime()

// #randomPrime()
unsigned int PublicKeyAlgorithmBox::randomRabinPrime(Integer &p,
		unsigned int bitlen, unsigned int s) {

    bool rabinPrimeFound = false;

    while (!rabinPrimeFound){
        Integer potentialPrime = randomInteger(bitlen);

        if(millerRabinTest(potentialPrime, s)){
            if(potentialPrime % 4 == 3) {
                rabinPrimeFound = true;
                p = potentialPrime;
            }
        }
    }

  return 0;
} // randomRabinPrime()

// #modPrimeSqrt()
bool PublicKeyAlgorithmBox::modPrimeSqrt(const Integer& y, const Integer& p,
		vector<Integer>& v) {
    if(p.Modulo(4) == 3) {
        Integer v1 = (y.Modulo(p)).SquareRoot();
        v.push_back(p.Minus(v1));
        v.push_back(v1);
        return true;
    }
    return false;
}

Integer PublicKeyAlgorithmBox::euklid(const Integer& a, const Integer& b,
		vector<Integer>& q) {

    vector<Integer> r{a, b};
    int m = 1;
    q.push_back(0);

    while(r[m] != 0) {
        q.push_back(r[m-1] / r[m]);
        r.push_back(r[m-1] - q[m] * r[m]);
        m++;
    }

//    q.erase(q.begin());
    return r[m-1];
}

unsigned int PublicKeyAlgorithmBox::computeConvergents(const Integer& a,
		const Integer& b, vector<Integer>& c, vector<Integer>& d) {
    vector<Integer> q;
    this->euklid(a, b, q);

    for(int j = 0; j < q.size(); j++) {
        if(j == 0) {
            c.push_back(1);
            d.push_back(0);
        }
        else if(j == 1) {
            c.push_back(q[1]);
            d.push_back(1);
        }
        else {
            c.push_back(q[j] * c[j-1] + c[j-2]);
            d.push_back(q[j] * d[j-1] + d[j-2]);
        }
    }

    return 1;
}

// #sqrt()
bool PublicKeyAlgorithmBox::sqrt(const Integer& x, Integer& s) const {

    if (x == 0 || x == 1) s = x;

    Integer start = 1, end = x, answer;
    while (start <= end)
    {
        Integer mid = (start + end) / 2;
        if (mid*mid == x) answer = mid;
        if (mid*mid < x)
        {
            start = mid + 1;
            answer = mid;
        }
        else
            end = mid-1;
    }

    if(answer*answer != x){
        return false;
    }
    s = answer;
    return true;
}

void PublicKeyAlgorithmBox::generateRSAParams(Integer& p, Integer& q,
		Integer& e, Integer& d, unsigned int bitlen, unsigned int s) {

    randomPrime(p, bitlen, s);
    randomPrime(q, bitlen, s);
    Integer n = p.Times(q);
    Integer phi_n = p.Minus(1).Times(q.Minus(1));


    bool e_found = false;
    while(!e_found){
        Integer potential_e = randomInteger(1, phi_n.Minus(1));
        if(CryptoPP::GCD(potential_e, phi_n) == 1){
            e = potential_e;
            //wenn es kein multiplikatives inverses gibt, dann weiter nach einem e suchen
            if(!multInverse(e, phi_n, d)){
                continue;
            }
            e_found = true;
        }
    }
}

