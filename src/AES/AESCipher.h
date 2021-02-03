#ifndef _AESCIPHER_H
#define _AESCIPHER_H

#include <string>
#include <vector>
#include "AESMath.h"
#include "AESState.h"
#include "AESKeySchedule.h"
#include "BlockCipher.h"

using namespace std;

class AESCipher : public BlockCipher {
private:
    bool debug_mode;
    AESMath aesmath;
    AESKeySchedule key_schedule;
    AESState state;

    /**
     * Funktion zur sauberen Ausgabe der Runde und der übergebenen Messages für Debug Zwecke.
     */
    void debugMessage(size_t round, string msg1, string msg2);

public:

    static const bool Encryption=false;
    static const bool Decryption=true;

    AESCipher(bool debug_mode=false);

    /**
     * Hinzufügen des übergebenen Schlüssel zum Attribut key_shedule mithilfe der Methode AESKeyShedule::setKey()
     *
     * @param key Vector vom Typ Byte, der den Schlüssel enthält
     * @return True bei Erfolg, sonst False
     */
    bool setKey(const vector<byte>& key);

    /**
     * Verschlüsselung eines Blocks der Größe 16 Byte, indem die Verschlüsselungsoperationen im festgelegter Reihenfolge
     * durchgeführt werden.
     *
     * @details
     * Zu beginn wird der interne Zustand mit 16 Byte des Plaintexts initialisiert. Anschließend wird die erste Runde
     * der Verschlüsselung durchgeführt, indem addKey() aufgerufen wird. Daraufhin wird der Zustand zwischen der ersten
     * und der finalen Runde modifiziert, indem die Operationen subBytes(), shiftRows(), mixColumns() und addKey()
     * durchgeführt werden. Abschließend wird der Zustand in der finalen Runde durch die Operationen subBytes(), shiftRows() und addKey()
     * modifiziert. Der verschlüsselte Text wird im Vector cipher_text gespeichert.
     *
     * @param plain_text Zeiger auf einen Vector vom Typ Byte, der den Klartext enthält
     * @param cipher_text Zeiger auf einen Vector vom Typ Byte, der den Ciphertext enthält
     *
     */

    void encryptBlock(const byte *plain_text, byte *cipher_text);

    /**
     * Entschlüsselung eines Blocks der Größe 16 Byte, indem die Verschlüsselungsoperationen im umgekehrter Reihenfolge
     * durchgeführt werden.
     *
     * @details
     * Zu beginn wird der interne Zustand mit 16 Byte des Ciphertexts initialisiert. Anschließend wird die finale Runde
     * der Verschlüsselung umgekehrt, indem addKey() aufgerufen wird. Daraufhin werden die Runden zwischen der finalen
     * und der ersten rückgängig gemacht, indem die Operationen invShiftRows(), invSubBytes(), addKey() und invMixColumns()
     * durchgeführt werden. Abschließend wird die erste Runde durch die Operationen invShiftRows(), invSubBytes() und addKey()
     * rückgängig gemacht. Der Entschlüsselte Text wird im Vector plain_text gespeichert.
     *
     * @param cipher_text Zeiger auf einen Vector vom Typ Byte, der den Ciphertext enthält
     * @param plain_text Zeiger auf einen Vector vom Typ Byte, der den Klartext enthält
     */
    void decryptBlock(const byte *cipher_text, byte *plain_text);

    /**
     * Modifikation der übergebenen Werte, um die zuvor implementierten Funktionen encryptBlock und decryptBlock
     * aufrufen zu können
     *
     * @details
     * Der übergebene Byte-Vektor in wird in ein Byte-Array geschrieben um die Funktion von encryptBlock und
     * decryptBlock zu gewährleisten. Anschließend wird anhand der Größe der übergebenen Byte-Vektoren die Funktion
     * encryptBlock bzw. decryptBlock (abhängig vom übergebenen Wert mode) aufgerufen. Abschließend werden die Werte
     * des Arrays noch in den Ausgabe-Vektor geschrieben.
     *
     * @param in Adresse eines Vectors vom Typ Byte, der den Klartext enthält
     * @param out Adresse eines Vectors vom Typ Byte, in den der Ciphertext geschrieben wird
     * @param mode Verarbeitungsmodus. Verschlüsselung = false, Entschlüsselung = true
     * @return True bei Erfolg, sonst False
     */
    bool process(const vector<byte>& in, vector<byte>& out, bool mode);

    /**
     * Verschlüsselung eines Plaintextes
     *
     * @details
     * Hier wird die zuvor implementierte Funktion process mit den jeweiligen Speicherorten (plain_text und
     * cipher_text) und dem Operationsmodus (Encryption) aufgerufen.
     * @param plain_text Adresse eines Vectors vom Typ Byte, der den Klartext enthält
     * @param cipher_text Adresse eines Vectors vom Typ Byte, in den der Ciphertext geschrieben wird
     * @return True bei Erfolg, sonst False
     */
    bool encrypt(const vector<byte>& plain_text, vector<byte>& cipher_text);

    /**
     * Entschlüsselung eines Ciphertextes
     *
     * @details
     * Hier wird die zuvor implementierte Funktion process mit den jeweiligen Speicherorten (cipher_text und
     * plain_text) und dem Operationsmodus (Decryption) aufgerufen.
     *
     * @param cipher_text Adresse eines Vectors vom Typ Byte, der den Ciphertext enthält
     * @param plain_text Adresse eines Vectors vom Typ Byte, in den der Klartext geschrieben wird
     * @return True bei Erfolg, sonst False
     */
    bool decrypt(const vector<byte>& cipher_text, vector<byte>& plain_text);

    /**
     * Wandelt einen String zu einem Vector um.
     *
     * @param msg String der umgewandelt wird
     * @param block_len Größe der Blöcke
     * @return Vector vom Typ Byte
     */
    static vector<byte> toVector(const string& msg, size_t block_len=16);

    /**
     * Wandelt einen Vector in einen String um.
     * @param vec Vector vom Typ Byte der zu einem String umgewandelt werden soll
     * @return string mit dem inhalt des Vectors.
     */
    static string toString(const vector<byte>& vec);
};


#endif
