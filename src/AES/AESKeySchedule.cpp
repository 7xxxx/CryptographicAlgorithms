#include <iostream>
#include <iomanip>
#include <sstream>
#include "AESKeySchedule.h"
#include <cmath>
#include <cstring>

using namespace std;

AESKeySchedule::AESKeySchedule(const AESMath &aesmath, bool debug_mode)
        : debug_mode(debug_mode), math((const AESMath *) &aesmath), key_schedule(), nk(0),
          nb(4), nr(0), r_con(11,0) {

    /*
     * Aufgabe 10
     */
    for(int i=0; i < 11; i++){
        if(i == 1){
            r_con[i] = 1;
        }
        else if(i > 1 and r_con[i-1] < 0x80){
            r_con[i] = 2 * r_con[i-1];
        }
        else if (i > 1 and r_con[i-1] >= 0x80){
            r_con[i] = (2 * r_con[i-1]) ^ 0x11b;
        }
        else{
            r_con[i] = i;
        }
    }

}

AESKeySchedule::~AESKeySchedule() {}

size_t AESKeySchedule::getNrOfRounds() const {
    return nr;
}

const word* AESKeySchedule::getRoundKey(size_t round) {
    assert(round<=nr);
    return &key_schedule[4*round];
}

string AESKeySchedule::formatRoundKey(size_t round) {
    assert(round<=nr);
    ostringstream os;

    for (size_t i=0; i<nb; i++) {
        os << hex << setw(8) << setfill('0') << key_schedule[4*round+i];
    }
    return os.str();
}

word AESKeySchedule::rotWord(word w) const {
    /*
     * Aufgabe 11
     */
    // Zuerst werden 8 Bit nach links verschoben, sodass die niederen 24 Bit korrekt gesetzt sind. Anschliessend
    // werden 24 bit nach rechts verschoben und mit dem vorherigen Ergebnis bitweise verodert, um die höchstwertigen 8 Bit
    // korrekt zu setzen.
    return (w << 8) | (w >> (24));
}

bool AESKeySchedule::setKey(const vector<byte>& key) {
    /*
     * Aufgabe 13
     */
    nk = key.size() / 4;
    if(nk == 4) nr = 10; else if(nk == 6) nr = 12; else if(nk == 8) nr = 14;
    else return false;
    word w[nb * (nr + 1)];
    word temp;

    //Spalte die Schlüsselbytes in Schlüsselwörter
    for(int i = 0, j = 0; i < key.size(); i += 4, j++) {
        w[j] = 0;
        w[j] |= key[i];     w[j] <<= 8;
        w[j] |= key[i+1];   w[j] <<= 8;
        w[j] |= key[i+2];   w[j] <<= 8;
        w[j] |= key[i+3];
    }

    if(!debug_mode) {
        for(int i = nk; i < nb * (nr + 1); i++) {
            temp = w[i - 1];

            if (i % nk == 0)
                temp = subWord(rotWord(temp)) ^ r_con[i / nk] << 24;
            else if (nk > 6 && i % nk == 4)
                temp = subWord(temp);

            w[i] = w[i - nk] ^ temp;
        }
    } else {
        cout << setfill(' ') << setw(2) << "i" << " | "
             << setfill(' ') << setw(4) << "temp  " << setfill(' ') << setw(5) << " | "
             << setfill(' ') << setw(8) << "After Rot" << " | "
             << setfill(' ') << setw(8) << "After Sub" << " | "
             << setfill(' ') << setw(4) << "Rcon" << setfill(' ') << setw(7) << " | "
             << "After XOR with rcon" << " | "
             << setfill(' ') << setw(4) << "w[i-nk]" << setfill(' ') << setw(4) << " | "
             << setfill(' ') << setw(4) << "w[i]" << setfill(' ') << setw(5) << endl;

        for(int i = nk; i < nb * (nr + 1); i++) {
            temp = w[i - 1];
            cout << setbase(10) << setw(2) << i;
            cout << setbase(16) << " | ";
            cout << setfill('0') << setw(8) << temp << " | ";

            if (i % nk == 0) {
                temp = rotWord(temp);
                cout << setfill('0') << setw(8) << temp << setfill(' ') << setw(4) << " | "; //after rot

                temp = subWord(temp);
                cout << setfill('0') << setw(8) << temp << setfill(' ') << setw(4) << " | "
                     << setfill('0') << setw(8) << (r_con[i / nk] << 24) << setfill(' ') << setw(2) << " | "; //after sub, rcon

                temp = temp ^ r_con[i / nk] << 24;
                cout << setfill('0') << setw(8) << temp << setfill(' ') << setw(14) << " | ";
            }
            else if (nk > 6 && i % nk == 4) {
                temp = subWord(temp);
                cout << setfill(' ') << setw(12) << " | " << setfill('0') << setw(8) << temp << setfill(' ')
                     << setw(4) << " | " << setfill(' ') << setw(11) << " | " << setfill(' ') << setw(22) << " | ";
            }
            else {
                cout << setfill(' ') << setw(12) << " | " << setfill(' ') << setw(12) << " | "
                     << setfill(' ') << setw(11) << " | " << setfill(' ') << setw(22) << " | ";
            }
            cout << setfill('0') << setw(8) << w[i - nk] << setfill(' ') << setw(2) << " | ";
            w[i] = w[i - nk] ^ temp;
            cout << setfill('0') << setw(8) << w[i] << setfill(' ') << setw(2) << endl;
        }
    }

    for(auto elem : w)
        key_schedule.push_back(elem);

    return true;
}

word AESKeySchedule::subWord(word w) const {
    /*
     * Aufgabe 12
     */
    word result = 0;
    byte byte_to_sub = 0;

    for(int i=0; i < 4; i++){
        byte_to_sub = 0;
        byte_to_sub |= (w >> 24);
        w <<= 8;

        byte_to_sub = math->sBox(byte_to_sub);

        result = result | byte_to_sub;

        if(i < 3) result <<= 8;
        else return result;
    }
}
