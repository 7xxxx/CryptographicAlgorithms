/*
 * rsa.cpp
 */

#include <iostream>
#include "PublicKeyAlgorithmBox.h"
#include "RSAEncryptor.h"
#include "RSADecryptor.h"
#include "RSAAttack.h"
#include "RSAOracle.h"

using namespace std;

void generatePrime(){
    /*********************************************************************
	 * Aufgabe 14.
	 *********************************************************************/

    PublicKeyAlgorithmBox pk_box;

    Integer prime;
    int s = 100;
    int tries = pk_box.randomPrime(prime, 128, s);

    cout << endl;
    cout << "Prime number" << endl;
    cout << "---------------------------" << endl;


    cout << "Found prime: " << prime << " after " << tries << " tries." << endl;
}

// #rsaParameters()
void rsaParameters() {
	/*********************************************************************
	 * Aufgabe 15.
	 *********************************************************************/
	 PublicKeyAlgorithmBox pk_box;

    cout << endl;
    cout << "Generate RSA params" << endl;
    cout << "---------------------------" << endl;

	 Integer p,q,e,d;
	 int bitlen = 10;
	 int s = 100;
	 pk_box.generateRSAParams(p, q, e, d, bitlen, s);

	 cout << "p: " << p << endl;
	 cout << "q: " << q << endl;
	 cout << "e: " << e << endl;
	 cout << "d: " << d << endl;
}

// #rsaDemo()
void rsaDemo() {
	/*********************************************************************
	 * Aufgabe 16.
	 *********************************************************************/

    cout << endl;
    cout << "RSA demo" << endl;
    cout << "---------------------------" << endl;

	Integer p = Integer("15192846618168946907");
	Integer q = Integer("10041530829891794273");
	Integer e = Integer("141290156426204318982571851806193576543");
	Integer d = Integer("73707354481439936713886319521045114527");
	Integer x = Integer("79372353861768787619084471254314002875");
	Integer y = Integer("47915958473033255778832465116435774510");

	RSADecryptor rsaDecryptor = RSADecryptor(p, q, d);

	Integer computeValue;
	rsaDecryptor.compute(y, computeValue);
	cout << "compute answer: " << computeValue << endl;

	Integer crtValue;
	rsaDecryptor.crt(y, crtValue);
	cout << "crt answer: " << crtValue << endl;

	Integer garnerValue;
	rsaDecryptor.garner(y, garnerValue);
	cout << "garner answer: " << garnerValue << endl;

}

//#sqrtExercise()
void sqrtExercise() {
	/*********************************************************************
	 * Aufgabe 17.
	 *********************************************************************/
    PublicKeyAlgorithmBox pk_box;

    cout << endl;
    cout << "SQRT exercise" << endl;
    cout << "---------------------------" << endl;

    Integer s;
    Integer x = Integer("3157242151326374471752634944");
    if(pk_box.sqrt(x, s)){
        cout << "Square root of: " << x << " is: " << s << endl;
    }else cout << "no square root" << endl;

    x = Integer("11175843681943819792704729");
    if(pk_box.sqrt(x, s)){
        cout << "Square root of: " << x << " is: " << s << endl;
    }else cout << "no square root" << endl;

    x = Integer("3343229819990029117723");
    if(pk_box.sqrt(x, s)){
        cout << "Square root of: " << x << " is: " << s << endl;
    }else cout << "no square root"<< endl;
}

// #factorizingAttack()
void factorizingAttack() {
	/*********************************************************************
	 * Aufgabe 18.
	 *********************************************************************/

    cout << endl;
    cout << "Factorizing attack" << endl;
    cout << "---------------------------" << endl;

    Integer n = Integer("127869459623070904125109742803085324131");
    Integer phi_n = Integer("127869459623070904102412837477002840200");
    Integer p, q;

    RSAAttack rsaAttack = RSAAttack();

    if(rsaAttack.factorizeN(n, phi_n, p, q)){
        cout << "n = 127869459623070904125109742803085324131\n"
             << "Found p: " << p << " q: " << q
             << "\np * q = " << p * q << endl;
    }else cout << "P and Q not found" << endl;

}

// #euklidExercise()
void euklidExercise() {
	/*********************************************************************
	 * Aufgabe 19.
	 *********************************************************************/
    cout << endl;
	cout << "Euklid Exercise" << endl;
	PublicKeyAlgorithmBox pk_box;
	Integer a = 39, b = 112, d;
	vector<Integer> q;

	d = pk_box.euklid(a, b, q);
	cout << "d: " << d << "    q = [";
	for(int i = 1; i < q.size(); i++) {
	    cout << q[i] << ", ";
	}
	cout << "]" << endl;
}

// #convergentsExercise()
void convergentsExercise() {
	/*********************************************************************
	 * Aufgabe 20.
	 *********************************************************************/
    PublicKeyAlgorithmBox pk_box;

    cout << endl;
    cout << "Convergents Exercise" << endl;

    Integer a = 39, b = 112;
    vector<Integer> c;
    vector<Integer> d;

    pk_box.computeConvergents(a, b, c, d);

    cout << "c = (";
    for(auto c : c) {
        cout << c << ", ";
    }

    cout << ")" << endl;

    cout << "d = (";
    for(auto d : d)
        cout << d << ", ";
    cout << ")" << endl;
}

// #wienerAttack()
void wienerAttack() {
	/*********************************************************************
	 * Aufgabe 21.
	 *********************************************************************/
    cout << endl;
    cout << "Wiener Attack" << endl;
    RSAAttack rsaAttack;
    Integer p, q;

    Integer n = Integer("224497286580947090363360894377508023561");
    Integer e = Integer("163745652718951887142293581189022709093");

    if(rsaAttack.wienerAttack(n, e, p, q)){
        cout << "p: " << p << " q: " << q << "\np * q = " << p * q << endl;
    }

}

// #oracleExercise()
void oracleExercise() {
	/*********************************************************************
	 * Aufgabe 22.
	 *********************************************************************/
    cout << endl;
    cout << "oracle Attack" << endl;

     Integer p = Integer("16015510136412338011");
	 Integer q = Integer("12177032305856164321");
	 Integer d = Integer("946975621");
	 vector<Integer> yv{Integer("116415012259126332853105614449093205668"),
                       Integer("74304303162215663057995326922844871006"),
                       Integer("102949691974634609941445904667722882083)"),
                       Integer("42549620926959222864355800078420537413")};

	 RSAOracle rsaOracle = RSAOracle(p, q, d);

	 for(auto y : yv) {
	     cout << rsaOracle.half(y) << endl;
	 }

}

// #halfAttack()
void halfAttack() {
	/*********************************************************************
	 * Aufgabe 23.
	 *********************************************************************/
    cout << endl;
    cout << "half Attack" << endl;

    Integer p = Integer("12889769717276679053");
    Integer q = Integer("17322528238664264177");
    Integer e = Integer("55051594731967684255289987977028610689");
    Integer d = Integer("149154082258429024247010774747829057473");
    Integer x = Integer("167092961114842952923160287194683529938");
    Integer y, clear_text;

    RSAEncryptor rsaEncryptor = RSAEncryptor(p * q, e);
    RSAAttack rsaAttack;
    RSAOracle rsaOracle = RSAOracle(p, q, d);

    cout << " p = " << p << endl;
    cout << " q = " << q << endl;
    cout << " e = " << e << endl;
    cout << " d = " << d << endl;
    cout << " x = " << x << endl;

    rsaEncryptor.compute(x, y);
    cout << "Geheimtext: " << y << endl;

    clear_text = rsaAttack.halfAttack(p * q, e, y, rsaOracle);
    cout << "Klartext: " << clear_text << endl;

}

// #main()
int main() {
    generatePrime();
	rsaParameters();
	rsaDemo();
	sqrtExercise();
	factorizingAttack();
	euklidExercise();
	convergentsExercise();
	wienerAttack();
	oracleExercise();
	halfAttack();
	return 0;
}

