#include <string>
#include "AESState.h"
#include <cstring>
#include <iostream>
#include <iomanip>

using namespace std;

AESState::AESState(const AESMath& aesmath, bool debug_mode)
    : debug_mode(debug_mode), math((const AESMath *) &aesmath)
{
    for (size_t i=0; i<16; i++) {
        state[i] = 0;
    }
}

void AESState::addKey(const word* key) {
    /*
     * Aufgabe 19
     */

    for(int i=0, j=0; i<4; i++, j+=4){
        state[j] ^= key[i] >> 24;
        state[j+1] ^= key[i] >> 16;
        state[j+2] ^= key[i] >> 8;
        state[j+3] ^= key[i];
    }
}

string AESState::format() const {
    ostringstream os;

    for (size_t i=0; i<16; i++) {
        os << setw(2) << setfill('0') << hex << (unsigned)state[i] << " ";
    }
    return os.str();
}


void AESState::get(byte* out) {
    /*
     * Aufgabe 14b
     */
    assert(out!=NULL);
    memcpy(out, state, 16);
}

byte AESState::getCell(size_t row, size_t col) {
    return state[row + 4*col];
}

void AESState::setCell(size_t row, size_t col, byte b) {
    state[row + 4*col] = b;
}


void AESState::subBytes() {
    /*
     * Aufgabe 17a
     */
    for(int i=0; i < 16; i++){
        state[i] = math->sBox(state[i]);
    }
}


void AESState::invSubBytes() {
    /*
     * Aufgabe 17b
     */
    for(int i=0; i < 16; i++){
        state[i] = math->invSBox(state[i]);
    }
}

void AESState::set(const byte* in) {
    /*
     * Aufgabe 14a
     */
    memcpy(&state, in, 16);
}


void AESState::mixColumns() {
    /*
     * Aufgabe 18a
     */

    byte *tmp = new byte[16];

    for(int i = 0; i < 16; i += 4) {
        tmp[i]   = math->mul(state[i], 0x2) ^ math->mul(state[i+1], 0x3) ^ state[i+2] ^ state[i+3];
        tmp[i+1] = state[i] ^ math->mul(state[i+1], 0x2) ^ math->mul(state[i+2], 0x3) ^ state[i+3];
        tmp[i+2] = state[i] ^ state[i+1] ^ math->mul(state[i+2], 0x2) ^ math->mul(state[i+3], 0x3);
        tmp[i+3] = math->mul(state[i], 0x3) ^ state[i+1] ^ state[i+2] ^ math->mul(state[i+3], 0x2);
    }

    memcpy(state, tmp, 16);
    delete[] tmp;
}

void AESState::invMixColumns() {
    /*
     * Aufgabe 18b
     */

    byte *tmp = new byte[16];

    for(int i = 0; i < 16; i+=4) {
        tmp[i] = math->mul(state[i], 0xe) ^ math->mul(state[i+1], 0xb) ^ math->mul(state[i+2], 0xd) ^ math->mul(state[i+3], 0x9);
        tmp[i+1] = math->mul(state[i], 0x9) ^ math->mul(state[i+1], 0xe) ^ math->mul(state[i+2], 0xb) ^ math->mul(state[i+3], 0xd);
        tmp[i+2] = math->mul(state[i], 0xd) ^ math->mul(state[i+1], 0x9) ^ math->mul(state[i+2], 0xe) ^ math->mul(state[i+3], 0xb);
        tmp[i+3] = math->mul(state[i], 0xb) ^ math->mul(state[i+1], 0xd) ^ math->mul(state[i+2], 0x9) ^ math->mul(state[i+3], 0xe);
    }

    memcpy(state, tmp, 16);
    delete[] tmp;
}

void AESState::shiftRow(size_t row, size_t shift) {
    /*
     * Aufgabe 15
     */
    assert(row >= 0 && row < 4);
    assert(shift > 0 && shift < 5);

    byte tmpStateRow[4];
    byte tmpValue = 0;

    //Schreibe die Werte der zu veränderten Reihe in ein Array
    for(int i=0; i<4; i++){
        tmpStateRow[i] = getCell(row, i);
    }

    //Verschiebe die Werte im Array um "shift" positionen
    for(int j=0,i=0; j<shift;j++){
        tmpValue = tmpStateRow[0];
        tmpStateRow[i] = tmpStateRow[i + 1];
        tmpStateRow[i + 1]= tmpStateRow[i + 2];
        tmpStateRow[i + 2] = tmpStateRow[i + 3];
        tmpStateRow[i + 3] = tmpValue;
    }

    //Schreibe Werte wieder zurück in State
    for(int i=0; i<4; i++){
        setCell(row, i, tmpStateRow[i]);
    }
}


void AESState::shiftRows() {
    /*
     * Aufgabe 16a
     */

    //shiftRow(0, 0);
    shiftRow(1, 1);
    shiftRow(2, 2);
    shiftRow(3, 3);
}


void AESState::invShiftRows() {
    /*
     * Aufgabe 16b
     */
    shiftRow(1, 3);
    shiftRow(2, 2);
    shiftRow(3, 1);
}
