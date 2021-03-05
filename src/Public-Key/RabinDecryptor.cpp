/*
 * RabinDecryptor.cpp
 */

#include <vector>
#include <sstream>
#include <iostream>
#include "RabinDecryptor.h"
#include "PublicKeyAlgorithmBox.h"

RabinDecryptor::RabinDecryptor(const Integer& p, const Integer& q,
		const Integer& padding) {
    this->p = p;
    this->q = q;
    this->padding = padding;
}

// #compute()
bool RabinDecryptor::compute(const Integer& y, vector<Integer>& xv) {
    //vec[0] = d; vec[1] = mq; vec[2] = mp;
    vector<Integer> vec(3, 0);
    Integer d, mq, mp, x1, x2, y1, y2;
    PublicKeyAlgorithmBox pk_box;

    Integer n = p * q;

    pk_box.EEA(p, q, d, mq, mp);

    x1 = pk_box.modularExponentation(y, (p + 1) / 4, p);
    x2 = p - x1;

    y1 = pk_box.modularExponentation(y, (q + 1) / 4, q);
    y2 = q - y1;

    xv.push_back(((x1 * q * mp) + (y1 * p * mq)) % n);

    xv.push_back(((x2 * q * mp) + (y1 * p * mq)) % n);

    xv.push_back(((x1 * q * mp) + (y2 * p * mq)) % n);

    xv.push_back(((x2 * q * mp) + (y2 * p * mq)) % n);

    return true;
}

bool RabinDecryptor::compute(const Integer& y, Integer& x) {
    vector<Integer> xv;

    if(compute(y, xv)) {
        x = xv[0];
        return true;
    }

    return false;
}

// #compute2()
bool RabinDecryptor::compute2(const Integer& y, Integer& x) {
    vector<Integer> v;
    compute(y, v);
    std::ostringstream os;

    for(auto v:v){
        os << v;
        std::string str_y = os.str();
        //std::cout << str_y << endl;
        os.str("");
        os << padding;
        std::string str_pad = os.str();

        size_t index = str_y.find(str_pad);
        if(index != string::npos){
            str_y.erase(index, str_pad.length());
            x = Integer(str_y.c_str());
            return true;
        }

        os.str("");
    }

  return false;
}

RabinDecryptor::~RabinDecryptor() {
}

