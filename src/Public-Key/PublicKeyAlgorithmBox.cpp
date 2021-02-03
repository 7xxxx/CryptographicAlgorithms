#include <iostream>
#include "nbtheory.h"
#include "osrng.h"
#include "PublicKeyAlgorithmBox.h"

using namespace CryptoPP;
using namespace std;

// #EEA()
bool PublicKeyAlgorithmBox::EEA(const Integer& a, const Integer& b,
		Integer& d, Integer& x, Integer& y) {
// TODO: Korrekte implementierung von Folien
    if(a == 0) {
        x = 0;
        y = 1;
        return false;
    }

    Integer x1, y1;
    EEA(b.Modulo(a), a, d, x1, y1);

    x = y1.Minus(b.DividedBy(a).Times(x1));
    y = x1;

    if((d = CryptoPP::GCD(a, b)) == 1)
        return true;

    return false;
} // EEA()

// #modularExponentation()
Integer PublicKeyAlgorithmBox::modularExponentation(const Integer& a,
		const Integer& b, const Integer& n) {

    Integer d = 1;
    for(int i = b.BitCount() - 1; i >= 0; i--){
        d = (d.Times(d)).Modulo(n);
        if(b.GetBit(i) == 1){
            d = (d.Times(a)).Modulo(n);
        }
    }

    //cout << "d: " << d << endl;
    //Integer x = a_exp_b_mod_c(a, b, n);
    //cout << "ergebnis: " << x << endl;

    return d;
} // modularExponentation()

// #multInverse()
bool PublicKeyAlgorithmBox::multInverse(const Integer& a, const Integer& n,
		Integer& a_inv) {
    Integer d, x, y;
    EEA(a, n, d, x, y);
    if (d != 1 || x.IsNegative())
        return false;
    else {
        a_inv = x;
        return true;
    }
} // multInverse()

// #witness()
bool PublicKeyAlgorithmBox::witness(const Integer& a, const Integer& n) {

    Integer d = 1;
    Integer n_minus_one = n.Minus(1);
    Integer x = 0;

    for(int i = n_minus_one.BitCount(); i >= 0; i--){
        x = d;
        d = d.Times(d).Modulo(n);
        if(d == 1 && x != 1 && x != n_minus_one){
            return true;
        }
        if(n.GetBit(i) == 1){
            d = d.Times(a).Modulo(n);
        }
    }

    if(d != 1) return true;

    return false;
} // witness()

// randomInteger()
Integer PublicKeyAlgorithmBox::randomInteger(const Integer& n) {

    BlockingRng blockingRng;

    size_t size = 24;
    Integer random;
    random.Randomize(blockingRng, size);

    return random;
}

// #millerRabinTest()
bool PublicKeyAlgorithmBox::millerRabinTest(Integer& n, unsigned int s) {
	return true;
} // millerRabinTest()

// #randomPrime()
unsigned int PublicKeyAlgorithmBox::randomPrime(Integer &p,
		unsigned int bitlen, unsigned int s) {
	return 0;
} // randomPrime()

// #randomPrime()
unsigned int PublicKeyAlgorithmBox::randomRabinPrime(Integer &p,
		unsigned int bitlen, unsigned int s) {
  return 0;
} // randomRabinPrime()

// #modPrimeSqrt()
bool PublicKeyAlgorithmBox::modPrimeSqrt(const Integer& y, const Integer& p,
		vector<Integer>& v) {
  return false;
}

Integer PublicKeyAlgorithmBox::euklid(const Integer& a, const Integer& b,
		vector<Integer>& q) {
  return Integer("1");
}

unsigned int PublicKeyAlgorithmBox::computeConvergents(const Integer& a,
		const Integer& b, vector<Integer>& c, vector<Integer>& d) {
  return 1;
}

// #sqrt()
bool PublicKeyAlgorithmBox::sqrt(const Integer& x, Integer& s) const {
  return false;
}

void PublicKeyAlgorithmBox::generateRSAParams(Integer& p, Integer& q,
		Integer& e, Integer& d, unsigned int bitlen, unsigned int s) {
}

