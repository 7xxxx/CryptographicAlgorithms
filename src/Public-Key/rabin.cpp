/*
 * rabin.cpp
 */

#include <iostream>
#include <vector>
#include "PublicKeyAlgorithmBox.h"
#include "RabinDecryptor.h"
#include "RabinEncryptor.h"
#include "RabinAttack.h"

using namespace std;

void createRabinPrime(){

    PublicKeyAlgorithmBox pk_box;

    Integer p;
    int bitlen = 50;
    int s = 100;

    pk_box.randomRabinPrime(p, bitlen, s);

    cout << p << endl;
}

// #rabinParameters()
void rabinParameters() {
	/*********************************************************************
	 * Aufgabe 9.
	 *********************************************************************/

	PublicKeyAlgorithmBox pk_box;

	Integer p = 0;
	Integer q = 0;
	pk_box.randomRabinPrime(p, 256, 50);
	pk_box.randomRabinPrime(q, 256, 50);

    cout << endl;
    cout << "Rabin parameters" << endl;
    cout << "---------------------------" << endl;
    cout << "p: " << p << endl;
    cout << "q: " << q << endl;

}

// #sqrtModPrimeExercise()
void sqrtModPrimeExercise() {
	/*********************************************************************
	 * Aufgabe 10.
	 *********************************************************************/
	PublicKeyAlgorithmBox pk_box;
    vector<Integer> v;

    cout << endl;
    cout << "SQRTModPrime" << endl;
    cout << "---------------------------" << endl;

    Integer y = Integer("400040001");
    Integer p = Integer("884249923643");
    pk_box.modPrimeSqrt(y, p, v);

	for(auto v: v){
        cout << v << endl;
	}

	vector<Integer> w;
	y = Integer("644640535034");
	p = Integer("868380007367");
    pk_box.modPrimeSqrt(y, p, w);

    for(auto w: w){
        cout << w << endl;
    }


}

// #rabinDemo()
void rabinDemo() {
	/*********************************************************************
	 * Aufgaben 11 und 12.
	 *********************************************************************/

    cout << endl;
    cout << "Rabin Demo" << endl;
    cout << "---------------------------" << endl;

	Integer p = Integer("728768879148869666628372866383");
	Integer q = Integer("1178365175275537416785439551531");
	Integer n = p.Times(q);
	Integer v;

	cout << "ENCRYPTION of 234131892323212" << endl;
	Integer y;
	RabinEncryptor rabin = RabinEncryptor(n, Integer("987654"));
	rabin.compute(234131892323212, y);

	cout << y << endl;

	cout << "DECRYPTION" << endl;
	RabinDecryptor rabinDec = RabinDecryptor(p, q, Integer("987654"));
	vector<Integer> xv;
	rabinDec.compute(y, xv);

    //cout << v << endl;

	for(auto v:xv){
	    cout << v << endl;
	}

}

// #rabinAttack()
void rabinAttack() {
	/*********************************************************************
	 * Aufgabe 13.
	 *********************************************************************/
    cout << endl;
    cout << "Rabin Attack" << endl;
    cout << "---------------------------" << endl;

    Integer p = Integer("728768879148869666628372866383");
    Integer q = Integer("1178365175275537416785439551531");
    Integer n = p.Times(q);

    Integer y;
    RabinEncryptor rabin = RabinEncryptor(n);
    rabin.compute(234131892323212, y);

    RabinAttack rbAttack;
    Integer factorF;
    RabinDecryptor rabin_dec = RabinDecryptor(p, q);

    int returnVal = rbAttack.factorize(n, factorF, 1000, rabin_dec);

    if(returnVal == -1){
        cout << "Factor not found" << endl;
    }else{
        cout << "Found factor: " << factorF << " after " << returnVal << " tries." << endl;
    }


}

// #main()
int main() {

    //createRabinPrime();
	rabinParameters();
	sqrtModPrimeExercise();
	rabinDemo();
	rabinAttack();

	return 0;
}
