#include <cassert>
#include <iostream>
#include <iomanip>
#include "AESCipher.h"
#include <cstring>

using namespace std;

AESCipher::AESCipher(bool debug_mode) : debug_mode(debug_mode), aesmath(),
    key_schedule(aesmath, debug_mode), state(aesmath, debug_mode) {
}

bool AESCipher::encrypt(const vector<byte>& plain_text, vector<byte>& cipher_text) {
    /*
     * Aufgabe 24b
     */

    return process(plain_text, cipher_text, Encryption);
}

bool AESCipher::decrypt(const vector<byte>& plain_text, vector<byte>& cipher_text) {
    /*
     * Aufgabe 24c
     */
    return process(plain_text, cipher_text, Decryption);
}

bool AESCipher::process(const vector<byte>& in, vector<byte>& out, bool mode) {
    /*
     * Aufgabe 24a
     */
    if(in.empty()) return false;

    int size = in.size();

    byte inArray[size];
    memcpy(&inArray, &in[0], in.size());

    byte outArray[size];

    for(int i=0; i < size; i+=16){
        if(mode == Encryption){
            encryptBlock(&inArray[i], &outArray[i]);
        }
        else if(mode == Decryption){
            decryptBlock(&inArray[i], &outArray[i]);
        }
    }

    for(byte x: outArray ){
        out.push_back(x);
    }

  return true;
}

bool AESCipher::setKey(const vector<byte>& key) {
    return key_schedule.setKey(key);
}

void AESCipher::debugMessage(size_t round, string msg1, string msg2) {
    if (debug_mode) {
            cout << "round[" << dec << setw(2) << setfill(' ') << round
                 << "]." << msg1 << setw(6) << setfill(' ') << "\t" << msg2 << endl;
    }
}

void AESCipher::decryptBlock(const byte *cipher_text, byte *plain_text) {
    /*
     * Aufgabe 23
     */

    state.set(cipher_text);
    int nr = key_schedule.getNrOfRounds();

    debugMessage(0, "iinput" , state.format());

    //Invert Final round
    state.addKey(key_schedule.getRoundKey(nr));
    debugMessage(0, "ik_sch" , key_schedule.formatRoundKey(key_schedule.getNrOfRounds()));


    for(int i=nr-1, j=1; i > 0; i--, j++){
        debugMessage(j, "istart" , state.format());

        state.invShiftRows();
        debugMessage(j, "is_row" , state.format());

        state.invSubBytes();
        debugMessage(j, "is_box" , state.format());

        state.addKey(key_schedule.getRoundKey(i));
        debugMessage(j, "ik_sch" , key_schedule.formatRoundKey(i));

        debugMessage(j, "ik_add" , state.format());
        state.invMixColumns();
    }

    //Invert first round

    debugMessage(nr, "istart" , state.format());

    state.invShiftRows();
    debugMessage(nr, "is_row" , state.format());

    state.invSubBytes();
    debugMessage(nr, "is_box" , state.format());

    state.addKey(key_schedule.getRoundKey(0));
    debugMessage(nr, "ik_sch" , key_schedule.formatRoundKey(0));

    debugMessage(key_schedule.getNrOfRounds(), "ioutput" , state.format());
    state.get(plain_text);

}

void AESCipher::encryptBlock(const byte *plain_text, byte *cipher_text) {
    /*
     * Aufgabe 22
     */

    state.set(plain_text);
    int nr = key_schedule.getNrOfRounds();

    debugMessage(0, "input" , state.format());

    //First round
    state.addKey(key_schedule.getRoundKey(0));
    debugMessage(0, "k_sch" , key_schedule.formatRoundKey(0));

    for(int i=1; i < nr; i++){
        debugMessage(i, "start" , state.format());

        state.subBytes();
        debugMessage(i, "s_box" , state.format());

        state.shiftRows();
        debugMessage(i, "s_row" , state.format());

        state.mixColumns();
        debugMessage(i, "m_col" , state.format());

        state.addKey(key_schedule.getRoundKey(i));
        debugMessage(i, "k_sch" , key_schedule.formatRoundKey(i));
    }

    //Final round
    debugMessage(key_schedule.getNrOfRounds(), "start" , state.format());

    state.subBytes();
    debugMessage(key_schedule.getNrOfRounds(), "s_box" , state.format());

    state.shiftRows();
    debugMessage(key_schedule.getNrOfRounds(), "s_row" , state.format());

    state.addKey(key_schedule.getRoundKey(nr));
    debugMessage(key_schedule.getNrOfRounds(), "k_sch" , key_schedule.formatRoundKey(key_schedule.getNrOfRounds()));

    debugMessage(key_schedule.getNrOfRounds(), "output" , state.format());
    state.get(cipher_text);
}

vector<byte> AESCipher::toVector(const string& msg, size_t block_len) {
    assert((block_len>0) && (block_len<=32) );

    vector<byte> v;

    copy(msg.begin(), msg.end(), back_inserter(v));

    v.push_back(0);
    while (v.size() % block_len != 0) {
        v.push_back(1);
    }

    return v;
}

string AESCipher::toString(const vector<byte>& vec) {
    auto it = vec.begin();
    string r;

    while ((*it!=0) and (it!=vec.end())) {
        r.push_back(*it);
        it++;
    }
    return r;
}
