#include <iostream>
#include <set>
#include "osrng.h"
#include "PublicKeyAlgorithmBox.h"

using namespace std;

void integerExercise() {
	/*********************************************************************
	 * Aufgabe 2.
	 *********************************************************************/
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
    PublicKeyAlgorithmBox pk_box;
    Integer a_inv_a, a_inv_b, a_inv_c;
    pk_box.multInverse(10353, 820343, a_inv_a);
    pk_box.multInverse(10353, 820344, a_inv_b);
    pk_box.multInverse(562312, 57913313, a_inv_c);
    cout << "a_inv a): " << a_inv_a << endl;
    cout << "a_inv b):" << a_inv_b << endl;
    cout << "a_inv c): " << a_inv_c << endl;

}

void modexpExercise() {
	/*********************************************************************
	 * Aufgabe 5.
	 *********************************************************************/
	 PublicKeyAlgorithmBox pk_box;

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

	PublicKeyAlgorithmBox pk_box;

    cout << endl;
    cout << "Random Numbers" << endl;
    cout << "---------------------------" << endl;

    Integer x = 2;

    cout << pk_box.randomInteger(x) << endl;

}

void millerRabinExercise() {
	/*********************************************************************
	 * Aufgabe 7.
	 *********************************************************************/
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
