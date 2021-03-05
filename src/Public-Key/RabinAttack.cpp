/*
 * RabinAttack.cpp
 */

#include "PublicKeyAlgorithmBox.h"
#include "RabinDecryptor.h"
#include "RabinAttack.h"

using namespace std;

RabinAttack::RabinAttack() {
}

RabinAttack::~RabinAttack() {
}

int RabinAttack::factorize(const Integer& n, Integer& f, int max_tries,
		RabinDecryptor& rabin_dec) {

    PublicKeyAlgorithmBox pk_box;
    Integer p, q, r, x;

    for(int i=0; i < max_tries; i++){
        r = pk_box.randomInteger(1, n-1);
        rabin_dec.compute((r * r) % n, x);

        if(x % n == r % n || x % n == -r % n)
            continue;
        else {
            p = Integer::Gcd(x - r, n);
            q = n / p;
            f = q;
            return i;
        }
    }
    return -1;
}
