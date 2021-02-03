#ifndef AES_AESSTATE_H
#define AES_AESSTATE_H

#include <cassert>
#include <string>
#include "datatypes.h"
#include "AESMath.h"

using namespace std;

class AESState {
private:
    bool debug_mode;
    const AESMath *math;

protected:
    byte state[16];

    /**
     * Gibt die Zelle an der übergebenen Reihe (row) und Spalte (col) zurück
     *
     * @param row Gibt die Reihe der Zelle an
     * @param col Gibt die Spalte der Zelle an
     *
     * @return Gibt das Byte zurück, dass an der gegebenen Stelle steht
     */
    byte getCell(size_t row, size_t col);

    /**
     * Setzt die Zelle an der Stelle der übergebenen Reihe (row) und Spalte (col) mit dem Byte (b)
     *
     * @param row Gibt die Reihe der Zelle an
     * @param col Gibt die Spalte der Zelle an
     */
    void setCell(size_t row, size_t col, byte b);

    /**
     * Diese Methode führt zyklische Linksverschiebung auf einer Zeile des internen Zustands durch.
     *
     * @details
     * Dazu wird die gewünschte Zeile aus dem internen Zustand in ein Array kopiert und die Verschiebungen innerhalb einer
     * Schleife durchgeführt. Dabei werden die Elemente so lange um eine Position verschoben, bis die Anzahl der Shifts
     * erreicht ist.
     *
     * @param row Gibt die zu Rotierende Reihe an
     * @param shift Gibt die Anzahl der Shifts an
     */
    void shiftRow(size_t row, size_t shift);

public:
    AESState(const AESMath& aesmath, bool debug_mode=false);

    /**
     * Initialisierung des AES Zustands
     *
     * @details
     * Hierbei wird mithilfe der memcpy funktion von C++ das übergebene Byte Array (in) in den AES Zustand
     * (state) geschrieben.
     *
     * @param in Byte-Array der als AES State interpretiert werden soll
     */
    void set(const byte* in);

    /**
     * Übergabe des Internen Zustands von AES
     *
     * @details
     * Hierbei wird mithilfe der memcpy funktion von C++ der interne Zustand von AES (state) an das übergebene Array
     * (out) geschrieben.
     *
     * @param out Byte-Array in das der State geschrieben werden soll
     */
    void get(byte* out);

    /**
     * AES Basisoperation Shiftrows
     *
     * @details
     * Hier wird mithilfe der zuvor implementierten Funktion shiftRow, die Reihenverschiebung des Zustands
     * durchgeführt. Hierfür müssen die Reihe 1 um 1, Reihe 2 um 2 und die Reihe 3 um 3 verschoben werden.
     */
    void shiftRows();

    /**
     * Substitution aller Bytes des internen Zustands mit den korrespondierenden Bytes der SBox.
     *
     * @details
     * Dazu wird über jedes Byte des internen Zustands iteriert und dieses mithilfe der Methode sBox() substituiert.
     */
    void subBytes();

    /**
     * AES Basisoperation mixColumns
     *
     * @details
     * Hierbei wird der Zustand spaltenweise bearbeitet. Die Spalten werden mithilfe einer Matrix multipliziert. Diese
     * Multiplikation wird mit der Bitweisen XOR (^) Funktion implementiert. Die Multiplikation rechnet über den Körper
     * GF(2 hoch 8).
     */
    void mixColumns();

    /**
     * AES Basisoperation inverse Shiftrows
     *
     * @details
     * Hier wird mithilfe der zuvor implementierten Funktion shiftRow, die inverse Reihenverschiebung des Zustands
     * durchgeführt. Hierfür müssen die Reihe 1 um 3, Reihe 2 um 2 und die Reihe 3 um 1 verschoben werden.
     */
    void invShiftRows();

    /**
     * Substitution aller Bytes des internen Zustands mit den korrespondierenden Bytes der invertierten SBox.
     *
     * @details
     * Dazu wird über jedes Byte des internen Zustands iteriert und dieses mithilfe der Methode invSBox() substituiert.
     */
    void invSubBytes();

    /**
     * AES Basisoperation inverse mixColumns
     *
     * @details
     * Hierbei wird der Zustand spaltenweise bearbeitet. Diese Funktion beschreibt die inverse Operation
     * zu der mixColumns Funktion. Die Spalten werden mithilfe einer Matrix die für die inverse Operation
     * zuständig ist, multipliziert. Diese Multiplikation wird mit der Bitweise XOR (^) Funktion implementiert.
     * Die Multiplikation rechnet über den Körper GF(2 hoch 8).
     */
    void invMixColumns();

    /**
     * Verknüpfung des Rundenschlüssels mit dem internen Zustand.
     *
     * @details
     * Dazu wird über jeweils 4 Byte des Zustands iteriert und jedes Byte des Zustands mit dem zugehörigen Byte des
     * Schlüssels XORed. Um auf das jeweilige Byte eines Schlüssels zuzugreifen, muss dieses an die niederwertigste Stelle
     * geshiftet werden und kann anschließend mit einem Byte aus dem internen Zustand XORed werden.
     *
     * @param key Rundenschlüssel
     */
    void addKey(const word* key);

    /**
     * Gibt den AES State in formatierter Form zurück
     *
     * @return Gibt den State als formatierten String zurück
     */
    string format() const;
};


#endif //AES_AESSTATE_H
