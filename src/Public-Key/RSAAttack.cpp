/*
 * RSAAttack.cpp
 */

#include <vector>
#include "PublicKeyAlgorithmBox.h"
#include "RSAAttack.h"
#include <iostream>
#include <cmath>

using namespace std;
using namespace CryptoPP;

RSAAttack::RSAAttack() {
}

RSAAttack::~RSAAttack() {
}

// #factorizeN()
bool RSAAttack::factorizeN(const Integer& n, const Integer& phi_n, Integer& p,
		Integer& q) const {

    PublicKeyAlgorithmBox pk_box;
    Integer a = 1;
    Integer b = phi_n - n - 1;
    Integer c = n;

    Integer root;
    pk_box.sqrt(b.Squared() - (4 * a * c), root);

    Integer p1 = (-b + root) / 2 * a;
    Integer p2 = (-b - root) / 2 * a;

    if(p1 * p2 == n){
        p = p1;
        q = p2;
        return true;
    }
    else return false;
}

// #wienerAttack()
bool RSAAttack::wienerAttack(const Integer& n, const Integer& e, Integer& p,
		Integer& q) const {
    PublicKeyAlgorithmBox pk_box;
    vector<Integer> c, d;
    Integer phi_n;

    pk_box.computeConvergents(e, n, c, d);

    for(int i = 2; i < c.size(); i++) {
        phi_n = (e * d[i] - 1) / c[i];

        if(factorizeN(n, phi_n, p, q)) {
            return true;
        }
    }
    return false;
}

// #halfAttack()
Integer RSAAttack::halfAttack(const Integer& n, const Integer& e, const Integer& y,
		RSAOracle& rsa_oracle) const {

    PublicKeyAlgorithmBox pk_box;
    Integer l, h, hi, lo, mid, y_tmp = y;
    l = n.BitCount();

    for(int i = 0; i <= l; i++) {
        h.SetBit(i, rsa_oracle.half(y_tmp));
        y_tmp = y_tmp * pk_box.modularExponentation(2, e, n);
    }

    lo = 0;
    hi = n * 1000000000000;

    for(int i = 0; i <= l; i++) {
        mid = (hi + lo) / 2;

        if(h.GetBit(i))
            lo = mid;
        else
            hi = mid;
    }

    return hi / 1000000000000;
}
