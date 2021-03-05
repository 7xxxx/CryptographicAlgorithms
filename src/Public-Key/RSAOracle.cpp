/*
 * RSAOracle.cpp
 */

#include <cassert>
#include <iostream>
#include "RSAOracle.h"
#include "PublicKeyAlgorithmBox.h"

using namespace std;

RSAOracle::RSAOracle(Integer& p, Integer& q, Integer& d) {
    this->n = p.Times(q);
    this->d = d;
}

RSAOracle::~RSAOracle() {
}

// #half()
bool RSAOracle::half(const Integer& y) const {
    PublicKeyAlgorithmBox pk_box;

    Integer bit = pk_box.modularExponentation(y, d, n);

    if(bit < n / 2 && 0 <= bit)
        return false;
    if(n / 2 < bit && bit <= n - 1)
        return true;
}

// #parity()
bool RSAOracle::parity(const Integer& y) const {


  return false;
}
