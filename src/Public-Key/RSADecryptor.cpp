/*
 * RSADecryptor.cpp
 */

#include "PublicKeyAlgorithmBox.h"
#include "RSADecryptor.h"

#include <iostream>

using namespace std;
using namespace CryptoPP;

RSADecryptor::RSADecryptor(const Integer& p, const Integer& q,
		const Integer& d) {
    this->p = p;
    this->q = q;
    this->d = d;
    this->n = p.Times(q);
}

RSADecryptor::~RSADecryptor() {
}

// #compute()
bool RSADecryptor::compute(const Integer& y, Integer& x) const {
    PublicKeyAlgorithmBox pk_box;
    x = pk_box.modularExponentation(y, d, n);

    return y < n;
}

// #crt()
bool RSADecryptor::crt(const Integer& y, Integer& x) const {
    PublicKeyAlgorithmBox pk_box;
    Integer k1, k2;

    k1 = pk_box.modularExponentation(y, d, p);
    k2 = pk_box.modularExponentation(y, d, q);

    Integer mp, mq, d_eea;
    d_eea = d;
    pk_box.EEA(p, q, d_eea, mp, mq);

    x = ((mq.Times(k1).Times(q)).Plus((mp.Times(k2)).Times(p))).Modulo(n);

    return y < n;
}

// #garner()
bool RSADecryptor::garner(const Integer& y, Integer& x) const {
    PublicKeyAlgorithmBox pk_box;
    Integer a, b, q_inv;

    a = pk_box.modularExponentation(y, d, p);
    b = pk_box.modularExponentation(y, d, q);
    pk_box.multInverse(q, p, q_inv);

    Integer a_minus_b = a.Minus(b);
    Integer qInv_mod_p = q_inv.Modulo(p);

    x = (((a_minus_b.Times(qInv_mod_p)).Modulo(p)).Times(q)).Plus(b);

    return y < n;
}
