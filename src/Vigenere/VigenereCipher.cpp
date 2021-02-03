#include <cassert>
#include "VigenereCipher.h"

int VigenereCipher::decrypt
        (
                const vector<byte> &cipher_text,
                const vector<byte> &key,
                vector<byte> &plain_text) {
    /*************************************************************************
     * Aufgabe 2.
     *
     *************************************************************************/

    for (int j = 0; j < cipher_text.size(); j++){
        // + 26 because otherwise it would calculate the modulo of a negative number, which would result in wrong chars
        // Calculate each character of the key with modulus of keysize so the key extends to the size of the plain text
        byte decrypted_char = (cipher_text[j] + 26 - key[j % key.size()]) % 26;
        plain_text.push_back(decrypted_char);
    }

    return plain_text.size();

    //Aufgabe 3b): vigenere -k rot -d ../../data/vc-aufg3b.txt = angewandtekryptographie
    //Aufgabe 10): vigenere -k sauron -d ../../data/vc-praktikum-cipher.txt =
    //itwashotwhenifirsttookithotasagledeandmyhandwassco
    // ...
    //fallthewordsofsaurontheonlyfairitisprecioustometho
    //ughibuyitwithgreatpain
} 


int VigenereCipher::encrypt
        (
                const vector<byte> &plain_text,
                const vector<byte> &key,
                vector<byte> &cipher_text
        ) {
    /*************************************************************************
     * Aufgabe 2.
     *************************************************************************/

    for (int j = 0; j < plain_text.size(); j++){
        cipher_text.push_back((plain_text[j] + key[j % key.size()]) % 26);
    }

    return cipher_text.size();
    //Aufgabe 3a): vigenere -k blau -e ../../data/vc-aufg3a.txt = BYGYXLNXUPKLZATIHCAJITE
}


