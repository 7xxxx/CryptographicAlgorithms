#include <iostream>
#include <set>
#include "osrng.h"
#include "PublicKeyAlgorithmBox.h"

using namespace std;

void integerExercise() {
	/*********************************************************************
	 * Aufgabe 2.
	 *********************************************************************/
    cout << "\nAufgabe 2" << endl;
	 Integer a = Integer(23975);
	 Integer b = Integer("12345678900987654321");
	 Integer c = Integer::Power2(10);

	 //b)
	 Integer result = a.Plus(c).Times(b).Modulo(50001);
	 cout << "a = " << a << ", b = " << b << ", c = " << c << ", Result = " << result << endl;
}

void eeaExercise() {
	/*********************************************************************
	 * Aufgabe 3.
	 *********************************************************************/
	cout << "\nAufgabe 3" << endl;
    PublicKeyAlgorithmBox pk_box;
    //a)
    Integer a = 2987, b = 1279865, d, x, y;
    pk_box.EEA(a, b, d, x, y);
    cout << "d = " << d << ", x = " << x << ", y = " << y << endl;

    //b)
    a = 78845945, b = 2503417846;
    pk_box.EEA(a, b, d, x, y);
    cout << "d = " << d << ", x = " << x << ", y = " << y << endl;

    //c)
    a = 57913313, b = 173739939;
    pk_box.EEA(a, b, d, x, y);
    cout << "d = " << d << ", x = " << x << ", y = " << y << endl;
}

void invExercise() {
	/*********************************************************************
	 * Aufgabe 4.
	 *********************************************************************/
	 //TODO: Fix c), ergebnis negativ, deshalb keine ausgabe (sollte 53494466 ergeben)
    cout << "\nAufgabe 4" << endl;

    PublicKeyAlgorithmBox pk_box;
    Integer a_inv_a, a_inv_b, a_inv_c;


    if(pk_box.multInverse(Integer("10353"), Integer("820343"), a_inv_a))
        cout << "10353 mod 820343 = " << a_inv_a << endl;
    else
        cout << "nicht invertierbar" << endl;

    if(pk_box.multInverse(Integer("10353"), Integer("820344"), a_inv_b))
        cout << "10353 mod 820344 = " << a_inv_b << endl;
    else
        cout << "nicht invertierbar" << endl;

    if(pk_box.multInverse(Integer("562312"), Integer("57913313"), a_inv_c))
        cout << "562312 mod 57913313 = " << a_inv_c << endl;
    else
        cout << "nicht invertierbar" << endl;

}

void modexpExercise() {
	/*********************************************************************
	 * Aufgabe 5.
	 *********************************************************************/
	 PublicKeyAlgorithmBox pk_box;
     cout << "\nAufgabe 5" << endl;
	 cout << endl;
	 cout << "Modulare Exponentiation" << endl;
	 cout << "---------------------------" << endl;

	 Integer a = 2, b = 100000, c = 23;
	 cout << "a: " << a << " hoch b: " << b << " mod c: " << c << " = " << pk_box.modularExponentation(a, b, c) << endl;

     a = 2343947997, b = 765, c = 111;
     cout << "a: " << a << " hoch b: " << b << " mod c: " << c << " = " << pk_box.modularExponentation(a, b, c) << endl;
}

void randExercise() {
	/*********************************************************************
	 * Aufgabe 6.
	 *********************************************************************/
    cout << "\nAufgabe 6" << endl;
	PublicKeyAlgorithmBox pk_box;

    cout << endl;
    cout << "Random Numbers" << endl;
    cout << "---------------------------" << endl;

    cout << pk_box.randomInteger(128) << endl;
    cout << pk_box.randomInteger(1024) << endl;

}

void millerRabinExercise() {
	/*********************************************************************
	 * Aufgabe 7.
	 *********************************************************************/
    cout << "\nAufgabe 7" << endl;
	PublicKeyAlgorithmBox pk_box;

    cout << endl;
    cout << "Miller Rabin Test" << endl;
    cout << "---------------------------" << endl;

    Integer a = Integer("279226292160650115722581212551219487007");
    Integer b = Integer("247278711133334795867191516244139839983");
    Integer c = Integer("192172622525902080249109244057747132167");
    Integer d = Integer("177387942943728133030691912202779547031");

	cout << "a: " << pk_box.millerRabinTest(a, 100) << endl;
	cout << "b: " << pk_box.millerRabinTest(b, 100) << endl;
	cout << "c: " << pk_box.millerRabinTest(c, 100) << endl;
	cout << "d: " << pk_box.millerRabinTest(d, 100) << endl;
}



int main(int argc, char** argv) {

	integerExercise();
	eeaExercise();
	invExercise();
	modexpExercise();
	randExercise();
	millerRabinExercise();

	return 0;

}
