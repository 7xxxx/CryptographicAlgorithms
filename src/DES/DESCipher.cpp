#include <cassert>
#include <iomanip>
#include "DESCipher.h"
#include <cstring>
#include <algorithm>
#include <bitset>

DESCipher::DESCipher() {
}

DESCipher::~DESCipher() {
}

void DESCipher::computeKeySchedule(const byte *key, bool encmode) {
/******************************************************************************
 * Aufgabe 5
 ******************************************************************************/

    byte * c_left_side = new byte[28]();        //die linken 28 bit
    byte * d_right_side = new byte[28]();       //die rechten 28 bit
    byte * merge_c_d = new byte[56]();          //linke und rechte seite zusammengefügt
    byte * keytable_after_pc2 = new byte[48](); //beinhaltet den Schlüssel nach PC2
    byte * round_key_shrinked = new byte[6]();  //beinhaltet den finalen Rundenschlüssel nachdem er auf 6 byte reduziert wurde

    //berechnung der einzelnen Rundenschlüssel
    int rounds = 16;
    int size_halves = 28;

    for(int i=0; i<rounds; i++){
        //PC1; Aufteilung in jeweils 28 bit, links und rechts
        for(int j=0; j < size_halves; j++){
            c_left_side[j] = pc1[j];
            d_right_side[j] = pc1[j + size_halves];
        }
        //Shift der arrays
        leftShiftArray(c_left_side, size_halves, total_rot[i]);
        leftShiftArray(d_right_side, size_halves, total_rot[i]);

        //zusammenfügen der linken und rechten seite
        memcpy(merge_c_d, c_left_side, size_halves);
        memcpy(merge_c_d + size_halves, d_right_side, size_halves);

        //PC2 durchführen
        for(int j=0; j < 48; j++){
            keytable_after_pc2[j] = merge_c_d[pc2[j] - 1];
        }

        //Key berechnen
        permutate(keytable_after_pc2, 48, key, 8, round_key_shrinked, 6);
        memcpy(key_schedule[i], round_key_shrinked, 6);
    }

    //wenn entschlüsselung, dann die Rundenschlüssel in umgedrehter reihenfolge sichern
    if(!encmode){
        reverse(key_schedule, key_schedule + 16);
    }

    delete[](c_left_side);
    delete[](d_right_side);
    delete[](merge_c_d);
    delete[](keytable_after_pc2);
    delete[](round_key_shrinked);
}

void DESCipher::leftShiftArray(byte *array, int size, int n){
    //Verschiebung (Rotational left shift) der Elemente im Array um n Stellen,
    for(int j=0; j < n; j++) {
        byte temp = array[0];
        for (int i = 0; i < size - 1; i++) {
            array[i] = array[i + 1];

        }
        array[size - 1] = temp;
    }
}


byte DESCipher::computeSBox(byte id, byte line, byte col) {
    /******************************************************************************
     * Aufgabe 6a
     ******************************************************************************/
    assert(id < 8);
    assert(line < 4);
    assert(col < 16);

    return sbox[id][16 * (line) + (col)];
}


int DESCipher::decrypt
        (
                const byte *cipher_text,
                int cipher_len,
                const byte *key,
                int key_len,
                byte *plain_text,
                int plain_len
        ) {
    /******************************************************************************
     * Aufgabe 8
     ******************************************************************************/

    assert(cipher_len % block_len == 0);
    assert(plain_len >= cipher_len);
    assert(key_len == 8);

    byte * to_process = new byte[block_len]();
    byte * processed = new byte[block_len]();

    computeKeySchedule(key, false);

    //Aufteilung des Cipher Textes in gleich große Blöcke
    for(int i = 0; i < cipher_len; i+=block_len){
        memcpy(to_process, cipher_text + i, block_len);
        processBlock(to_process, processed);
        memcpy(plain_text + i, processed, block_len);
    }

    delete[] to_process;
    delete[] processed;

    return plain_len;
}


int DESCipher::encrypt
        (
                const byte *plain_text,
                int plain_len,
                const byte *key,
                int key_len,
                byte *cipher_text,
                int cipher_len) {
    /******************************************************************************
     * Aufgabe 8
     ******************************************************************************/

    assert(plain_len % block_len == 0);
    assert(cipher_len >= plain_len);
    assert(key_len == 8);

    byte * to_process = new byte[block_len]();
    byte * processed = new byte[block_len]();

    computeKeySchedule(key, true);

    //Aufteilung des Plain Textes in gleich große Blöcke
    for(int i = 0; i < plain_len; i+=block_len){
        memcpy(to_process, plain_text + i, block_len);
        processBlock(to_process, processed);
        memcpy(cipher_text + i, processed, block_len);
    }

    delete[] to_process;
    delete [] processed;

    return cipher_len;
}


void DESCipher::feistel
        (
                const byte *l_in,
                const byte *r_in,
                const byte *key,
                byte *l_out,
                byte *r_out
        ) {
    /******************************************************************************
     * Aufgabe 6c
     ******************************************************************************/
    byte * f_out = new byte[4]();           //sichert das Ergebnis von functionF()

    memcpy(l_out, r_in, 4);              //kopiere r_in zu l_out
    functionF(r_in, key, f_out);            //berechne functionF und sichere in f_out

    //XOR jedes byte in l_in mit jedem byte in f_out
    for(int i = 0; i < 4; i++){
        r_out[i] = l_in[i] ^ f_out[i];
    }

    delete[] f_out;
}


void DESCipher::functionF
        (
                const byte *r_in,
                const byte *key,
                byte *r_out
        ) {
    /******************************************************************************
     * Aufgabe 6b
     *
     ******************************************************************************/
    byte * r_in_expanded = new byte[6]();   //sichert das expandierte r_in
    byte * r_xor_k = new byte[6]();         //sichert das Ergebnis von r_in_expanded XOR roundkey[rnd]
    byte * s_out = new byte[4]();           //sichert die Ausgabe der S-Boxen

    bitset<2> row_bit;                      //2 bits für die Angabe der Zeile der S-Box
    bitset<4> col_bit;                      //4 bits für die Angabe der Spalte der S-Box
    byte upper, lower;                      //sichert die höher- und niederwertigen bits des S-Box output

    //Expandiere r_in zu 48 bit
    permutate(ev, 48, r_in, 4, r_in_expanded, 6);

    //XOR r_in mit dem Rundenschlüssel
    for(int i = 0; i < 6; i++){
        r_xor_k[i] = r_in_expanded[i] ^ key[i];
    }

    //Iteriere über jeden 6 Bit Vektor, berechne den Index der S-Box und setze jedes Byte mit der Ausgabe der S-Boxen
    for(int i = 0, j = 0, k = 0;
            i != 48;
            i += 6, j++)  {

        //Sichere das Erste und das Letzte Bit von r_xor_k um den Zeilen Index der S-Box zu berechnen
        row_bit[1] = getBit(r_xor_k, sizeof(r_xor_k), i);
        row_bit[0] = getBit(r_xor_k, sizeof(r_xor_k), i+5);

        //Sichere die Bits an der Position 2-5 von r_xor_k um den Spalten Index der S-Box zu berechnen
        col_bit[3] = getBit(r_xor_k, sizeof(r_xor_k), i + 1);
        col_bit[2] = getBit(r_xor_k, sizeof(r_xor_k), i + 2);
        col_bit[1] = getBit(r_xor_k, sizeof(r_xor_k), i + 3);
        col_bit[0] = getBit(r_xor_k, sizeof(r_xor_k), i + 4);

        //konvertiere die Bitsets zu einer dezimal Zahl
        ulong row = row_bit.to_ulong();
        ulong col = col_bit.to_ulong();

        if(j % 2 == 0) { //setze die 4 höherwertigen bits der S-Box Ausgabe
            upper = computeSBox(j, row, col);
            upper <<= (uint8_t) 4;
        }
        else {          //setze die 4 niederwertigen bits der S-Box Ausgabe
            lower = computeSBox(j, row, col);
            s_out[k] = upper | lower;
            k++;
        }
    }

    //finale Permutation mit der Inversen der Eingangspermutation
    permutate(pp, 32, s_out, 4, r_out, 4);

    delete[] r_in_expanded;
    delete[] r_xor_k;
    delete[] s_out;
}


bool DESCipher::getBit(const byte *array, int array_len, int pos) const {
    int bytepos, bitpos;
    byte b;

    assert(array_len > 0);
    assert(pos >= 0);

    bytepos = pos / 8;                  //Berechne das byte des Arrays auf das zugegriffen werden soll
    bitpos = 7 - (pos % 8);             //Berechne die Position relativ zu dem byte
    if (bytepos < array_len) {
        b = 0x01;
        b = b << bitpos;

        //Return true wenn das bit gesetzt ist, andernfalls false
        return ((array[bytepos] & b) == b);
    } else {
        return false;
    }

}


void DESCipher::permutate
        (
                const byte *p,
                int p_len,
                const byte *in_array,
                int in_len,
                byte *out_array,
                int out_len
        ) const {
    int i;

    assert(in_len > 0);
    assert(p_len <= out_len * 8);

    //iteriere durch jeden Eintrag in p und ersetze das Bit in out_array mit dem bit an der Position p[i] - 1 des in_array
    for (i = 0; i < p_len; i++) {
        setBit(out_array, out_len, i, getBit(in_array, in_len, p[i] - 1));
    }

}


void DESCipher::printBitField(const byte *bytefield, int len, int block_len) const {
    int i, bit_len;

    bit_len = 8 * len;                      //berechne die Größe des Bitvektors
    for (i = 0; i < bit_len; i++) {         //iteriere über jedes Bit und gebe es aus
        cout << getBit(bytefield, len, i);
        //nach jedem Block der Größe block_len gebe ein Leerzeichen aus
        if ((i + 1 < bit_len) && ((i + 1) % block_len == 0)) {
            cout << " ";
        }
    }
}


void DESCipher::processBlock(const byte *in_block, byte *out_block) {
/******************************************************************************
 * Aufgabe 7
 ******************************************************************************/
    byte * ip_out = new byte[8]();          //sichert die Ausgabe der Eingangspermutation der feistel chiffre
    byte * l_in = new byte[4]();            //sichert die linken 32 Bits des Input Textes
    byte * r_in = new byte[4]();            //sichert die rechten 32 Bits des Input Textes
    byte * l_out_feistel = new byte[4]();   //sichert die linken 32 Bits der Ausgabe der Feistel Chiffre
    byte * r_out_feistel = new byte[4]();   //sichert die rechten 32 Bits der Ausgabe der Feistel Chiffre
    byte * feistel_rounds_out_merged = new byte[8]();  //sichert das Ergebnus der 16 Feistel runden, in 64 Bit

    //Eingangspermutation
    permutate(ip, 64, in_block, 8, ip_out, 8);

    //aufteilen in links und rrechts
    memcpy(l_in, ip_out, 4);
    memcpy(r_in, ip_out + 4, 4);

    //16 runden feistel
    for(int i = 0; i < 16; i++){
        feistel(l_in, r_in, key_schedule[i], l_out_feistel, r_out_feistel);
        memcpy(l_in, l_out_feistel, 4);
        memcpy(r_in, r_out_feistel, 4);
    }

    //Zusammenführen von l und r nach den 16 feistel runden. Die linken 32 bit werden nach der feistel chiffre die letzten 32 bit
    memcpy(feistel_rounds_out_merged, r_out_feistel, 4);
    memcpy(feistel_rounds_out_merged + 4, l_out_feistel, 4);

    //finale Permutation
    permutate(fp, 64, feistel_rounds_out_merged, 8, out_block, 8);

    delete[] ip_out;
    delete[] l_in;
    delete[] r_in;
    delete[] l_out_feistel;
    delete[] r_out_feistel;
    delete[] feistel_rounds_out_merged;
}

void DESCipher::setBit
        (
                byte *array,
                int array_len,
                int pos,
                bool value
        ) const {
    int bytepos, bitpos;
    byte b;

    assert(array_len > 0);
    assert(pos >= 0);
    assert(pos < 8 * array_len);

    bytepos = pos / 8;                  //Berechne das byte des Arrays auf das zugegriffen werden soll
    bitpos = 7 - (pos % 8);             //Berechne die Position relativ zu dem byte
    b = 0x01;                           //initialisiere b mit 00000001
    b = b << bitpos;                    //leftshift der bits

    //wenn das zu setzende Bit 1 ist, bitweise berechnen oder nur das Bit an position bitpos setzen
    if (value == true) {
        array[bytepos] |= b;
    }

    //wenn das zu setzende Bit 0 ist, setze alle Bits in b zu 1 außer das gesetzte bit. Berechne dann bitweise und setze
    //nur das Bit auf Position bitpos auf 0
    else {
        b = b ^ 0xff;
        array[bytepos] &= b;
    }
}

// #ip
byte DESCipher::ip[64] = {
        58, 50, 42, 34, 26, 18, 10, 2,
        60, 52, 44, 36, 28, 20, 12, 4,
        62, 54, 46, 38, 30, 22, 14, 6,
        64, 56, 48, 40, 32, 24, 16, 8,
        57, 49, 41, 33, 25, 17, 9, 1,
        59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5,
        63, 55, 47, 39, 31, 23, 15, 7
};


// #fp
byte DESCipher::fp[64] = {
        40, 8, 48, 16, 56, 24, 64, 32,
        39, 7, 47, 15, 55, 23, 63, 31,
        38, 6, 46, 14, 54, 22, 62, 30,
        37, 5, 45, 13, 53, 21, 61, 29,
        36, 4, 44, 12, 52, 20, 60, 28,
        35, 3, 43, 11, 51, 19, 59, 27,
        34, 2, 42, 10, 50, 18, 58, 26,
        33, 1, 41, 9, 49, 17, 57, 25
};


// #ev
byte DESCipher::ev[48] = {
        32, 1, 2, 3, 4, 5,
        4, 5, 6, 7, 8, 9,
        8, 9, 10, 11, 12, 13,
        12, 13, 14, 15, 16, 17,
        16, 17, 18, 19, 20, 21,
        20, 21, 22, 23, 24, 25,
        24, 25, 26, 27, 28, 29,
        28, 29, 30, 31, 32, 1
};


// #pc1
byte DESCipher::pc1[] = {
        57, 49, 41, 33, 25, 17, 9,
        1, 58, 50, 42, 34, 26, 18,
        10, 2, 59, 51, 43, 35, 27,
        19, 11, 3, 60, 52, 44, 36,

        63, 55, 47, 39, 31, 23, 15,
        7, 62, 54, 46, 38, 30, 22,
        14, 6, 61, 53, 45, 37, 29,
        21, 13, 5, 28, 20, 12, 4
};

// #pc2
byte DESCipher::pc2[] = {
        14, 17, 11, 24, 1, 5,
        3, 28, 15, 6, 21, 10,
        23, 19, 12, 4, 26, 8,
        16, 7, 27, 20, 13, 2,
        41, 52, 31, 37, 47, 55,
        30, 40, 51, 45, 33, 48,
        44, 49, 39, 56, 34, 53,
        46, 42, 50, 36, 29, 32
};

// #sbox
byte DESCipher::sbox[8][64] = {
        /* S1 */
        14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
        0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
        4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
        15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13,

        /* S2 */
        15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
        3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
        0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
        13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9,

        /* S3 */
        10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
        13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
        13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
        1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12,

        /* S4 */
        7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
        13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
        10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
        3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14,

        /* S5 */
        2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
        14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
        4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
        11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3,

        /* S6 */
        12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
        10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
        9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
        4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13,

        /* S7 */
        4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
        13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
        1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
        6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12,

        /* S8 */
        13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
        1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
        7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
        2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
};


// #pp
byte DESCipher::pp[32] = {
        16, 7, 20, 21,
        29, 12, 28, 17,
        1, 15, 23, 26,
        5, 18, 31, 10,
        2, 8, 24, 14,
        32, 27, 3, 9,
        19, 13, 30, 6,
        22, 11, 4, 25
};


// #total_rot
byte DESCipher::total_rot[16] = {
        1, 2, 4, 6, 8, 10, 12, 14, 15, 17, 19, 21, 23, 25, 27, 28
};
