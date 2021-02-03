#include <iostream>
#include <iomanip>
#include "AESMath.h"

using namespace std;

AESMath::AESMath() : exp_table(256, 0), log_table(256, 0), sbox(256, 0), inv_sbox(256, 0) {

    /*
     * Aufgabe 5a
     */

    byte t = 0x01;
    byte g = 0x03;
    for(int i = 0; i < 256; i++){
        exp_table[i] = t;
        t = rpmul(g, t);
    }

//    printTable(exp_table);

    /*
     * Aufgabe 5b
     */

    for(int i = 0; i < 256; i++){
        log_table[exp_table[i]] = i;
    }

//    printTable(log_table);


    /*
     * Aufgabe 9a + 9b
     */

    for(int i = 0; i < 256; i++){
        sbox[i] = atrans(inv(i));
        inv_sbox[sbox[i]] = i;
    }
//    printTable(sbox);
//    printTable(inv_sbox);

}

byte AESMath::add(byte a, byte b) {
    /*
     * Aufgabe 2
     */
    return a ^ b;
}

byte AESMath::atrans(byte x) {
    /*
     * Aufgabe 8
     */

    vector<byte> matrix = {0xF8, 0x7C, 0x3E, 0x1F, 0x8F, 0xC7, 0xE3, 0xF1}; //every row of the matrix
    vector<byte> thirtySix = {0x0, 0x1, 0x1, 0x0, 0x0, 0x0, 0x1, 0x1}; // 0x63 into byte vector

    byte result_bit = 0x0;
    byte temp = 0x0;
    byte return_val = 0x0;

    for (int i = 0, j = 7; i < matrix.size(); i++, j--){
        temp = matrix[i] & x;
        result_bit = parity(temp) ^ thirtySix[i];

        if(result_bit){
            return_val |= 1UL << (j);
        }
    }
    return return_val;
}

byte AESMath::exp(byte i) const {
    /*
     * Aufgabe 5c
     */
    return exp_table[i];
}

byte AESMath::inv(byte b) const {
    /*
     * Aufgabe 6
     */

    if (b == 0) return 0;

    //Berechnung von i
//    int i = log(b);
//    return exp(256 - 1 - i);
    // g hoch m - 1 - i (siehe Folie Seite 15)

    return exp(255 - log(b));
}

byte AESMath::log(byte b) const {
    /*
     * Aufgabe 5d
     */
  return log_table[b];
}

byte AESMath::rpmul(byte a, byte b) {
    /*
     * Aufgabe 4
     */
    if (a == 0 || b == 0) return 0;
    else {
       byte p = 0;

       while(a > 0) {
           if(a % 2 != 0){
               p = add(p, b);
           }
           a = a >> 1;
           b = xtime(b);
       }
       return p;
    }
}

byte AESMath::mul(byte a, byte b) const {
    /*
     * Aufgabe 5e
     */

    if (a == 0 || b == 0) return 0;
    int i = 0;
    int j = 0;

    for(; i < log_table.size(); i++){
        if(exp(i) == a){
            break;
        }
    }

    for(; j < log_table.size(); j++){
        if(exp(j) == b){
            break;
        }
    }
    int l = (i + j) % 255;

    return exp_table[l];
}

byte AESMath::parity(byte b) {
    /*
     * Aufgabe 7
     */

    int counter = 0;

    for(int i = 0; i < 8; i++){
        if (b & 0x01)       //Prüfe, ob niederwertigstes Bit gesetzt. Wenn ja, counter++
            counter++;
        b = b >> 1;         //Nächstes zu prüfende Bit nach rechts schieben
    }
    return counter % 2;     //Wenn Anzahl bits gerade, gebe 0 zurück, sonst 1
}

void AESMath::printTable(const vector<byte> &table) {
    cout << "         +----------------------------------------";
    cout << "---------------------------------------+" << endl;
    cout << "         |                                       y";
    cout << "                                       |" << endl;
    cout << "         +----+----+----+----+----+----+----+----+";
    cout << "----+----+----+----+----+----+----+----+" << endl;
    cout << "        ";
    for (size_t i = 0; i < 16; i++) {
        cout << " | " << setw(2) << setfill(' ') << hex << i;
    }
    cout << " |" << endl;
    cout << "+---+----+----+----+----+----+----+----+----+----+";
    cout << "----+----+----+----+----+----+----+----+" << endl;
    for (size_t i = 0; i < 16; i++) {
        cout << "|   | " << setw(2) << setfill(' ') << hex << i;
        for (size_t j = 0; j < 16; j++) {
            cout << " | " << setw(2) << setfill('0') << (unsigned) table[(i << 4) + j];
        }
        cout << " |" << endl;
        if (i < 15) {
            if (i == 7) {
                cout << "| x ";
                cout << "+----+----+----+----+----+----+----+----+----+";
                cout << "----+----+----+----+----+----+----+----+" << endl;

            } else {
                cout << "|   ";
                cout << "+----+----+----+----+----+----+----+----+----+";
                cout << "----+----+----+----+----+----+----+----+" << endl;
            }
        } else {
            cout << "|---";
            cout << "+----+----+----+----+----+----+----+----+----+";
            cout << "----+----+----+----+----+----+----+----+" << endl;
        }
    }
}

byte AESMath::sBox(byte b) const {
    /*
     * Aufgabe 9c
     */
    return sbox[b];
}

byte AESMath::invSBox(byte b) const {
    /*
     * Aufgabe 9d
     */
    return inv_sbox[b];
}

byte AESMath::xtime(byte b) {
    /*
     * Aufgabe 3
     */
    byte r = b << 1;                //r = b * b
    if ((b & 0x80) == 0x80){        //Polynom grad 8? Wenn ja r = r - 0x1b
        r = r ^ 0x1b;
    }
    return r;
}

string AESMath::format(byte b) {
    ostringstream os;
    os << "{" << hex << setfill('0') << setw(2) << (unsigned)b <<"}";
    return os.str();
}
