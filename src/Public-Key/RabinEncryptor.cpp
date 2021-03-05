/*
 * RabinEncryptor.cpp
 */

#include "RabinEncryptor.h"
#include <sstream>

RabinEncryptor::RabinEncryptor(const Integer& n, const Integer& padding) {
    this->n = n;
    this->padding = padding;
}

RabinEncryptor::~RabinEncryptor() {
}

// #compute()
bool RabinEncryptor::compute(const Integer& x, Integer& y) {
    y = (x.Times(x)).Modulo(n);
    return true;
}

// #compute2()
bool RabinEncryptor::compute2(const Integer& x, Integer& y) {
    std::ostringstream os;
    os << x << padding;
    std::string str = os.str();

    Integer padded = Integer(str.c_str());

    compute(padded, y);

  return false;
}
