#ifndef _DESCipher_h_
#define _DESCipher_h_

#include "BlockCipher.h"

class DESCipher : public BlockCipher {
private:
  static byte ip[64];

  static byte fp[64];

  static byte ev[48];

  static byte pc1[];

  static byte pc2[];

  static byte sbox[8][64];

  static byte pp[32];

  static byte total_rot[16];

  byte key_schedule[16][6];

public: 

  DESCipher();

  ~DESCipher();

    /**
     * Berechnung der 16 Verschiebetabellen für die Rundenschlüssel für den DES
     *
     * @details Die Methode beschreibt die Berechnung der Verschiebetabellen von allen Runden, sowie die Berechnung
     * der Rundenschlüssel selbst. Die fertig berechneten Rundenschlüssel werden in der Variable key_schedule
     * gesichert.
     *
     * Vorgehen zur Berechnung der Verschiebetabellen:
     * Der vom Nutzer eingegebene Schlüssel mit der Länge von 64-Bit wird zunächst mithilfe der permutate Methode
     * gekürzt auf 56-Bit. Hierbei werden die Paritybits entfernt, die bei der Berechnung der eigentlichen Schlüssel
     * nicht gebraucht werden. Anschließend wird der gekürzte 56-Bit Schlüssel aufgeteilt in zwei gleich große Teile.
     * Diese jeweils 28-Bit großen Teilschlüssel durchlaufen nun abhängig von der Runde einen bzw. mehrere left shifts.
     * Diese Left-Shift Tabelle, die die Anzahl an left shifts angibt lässt sich in der Variable total_rot näher
     * betrachten. Die geshifteten Schlüssel werden anschließend wieder zusammengefügt, und mithilfe der PC2
     * Verschiebetabelle werden die entgültigen Schlüssel berechnet.
     *
     * Das oben beschriebene Vorgehen berechnet nur die Verschiebetabellen. Die eigentlichen Schlüssel werden nun
     * mithilfe von den Verschiebetabellen und der Methode permutate berechnet.
     *
     * @param key Der vom Nutzer eingegebene Schlüssel
     * @param encmode Variable zur Bestimmung ob es sich um die Ver- oder Entschlüsselung handelt
     *
     */
  void computeKeySchedule(const byte *key, bool encmode=true);

    /**
       * Gibt den Wert der an der definierten Position in den SBoxen steht, als Byte zurück.
       *
       * @param id Angabe welche SBox benutzt werden soll
       * @param line Angabe der Zeile
       * @param col Angabe der Spalte
       *
       * @return gibt den Wert der SBox als Byte zurück
       *
       */
  byte computeSBox(byte id, byte line, byte col);

    /**
     * Das übergebene Array wird um n Positionen mit einem Rotational left shift modifiziert.
     *
     * @param array Zeiger auf das Array das verschoben werden soll
     * @param size Größe des Arrays
     * @param n Anzahl der left shifts im Array
     *
     */
  void leftShiftArray(byte *array, int size, int n);

    /**
     * Entschlüsseln eines Ciphertextes
     *
     * @details Der übergebene Ciphertext wird in gleich große Blöcke aufgeteilt, und der Methode
     * processBlock übergeben. Ebenfalls wird hier die Methode computeKeySchedule mit den Übergabeparametern: key
     * und false für den encmode aufgerufen, um alle Rundenschlüssel berechnen zu können.
     *
     * @param cipher_text Übergebener Ciphertext des Nutzers
     * @param cipher_len Länge des Ciphertextes
     * @param key Übergebener Schlüssel des Nutzers
     * @param key_len Länge des Schlüssels
     * @param plain_text Bytearray zum sichern des Plaintextes
     * @param plain_len Größe des Bytearrays
     *
     * @return Die länge des Plaintextes
     */
  virtual int decrypt(const byte* cipher_text, int cipher_len, 
                      const byte* key, int key_len,
                      byte* plain_text, int plain_len);

    /**
   * Verschlüsseln eines Plaintextes
   *
   * @details Der übergebene Plaintext wird in gleich große Blöcke aufgeteilt, und der Methode
   * processBlock übergeben. Ebenfalls wird hier die Methode computeKeySchedule mit den Übergabeparametern: key
   * und true für den encmode aufgerufen, um alle Rundenschlüssel berechnen zu können.
   *
   * @param plain_text Übergebener Plaintext des Nutzers
   * @param plain_len Länge des Plaintextes
   * @param key Übergebener Schlüssel des Nutzers
   * @param key_len Länge des Schlüssels
   * @param cipher_text Bytearray zum sichern des Ciphertextes
   * @param cipher_len Größe des Bytearrays
   *
   * @return Die länge des Ciphertextes
   */
  virtual int encrypt(const byte* plain_text, int plain_len, 
                      const byte* key, int key_len,
                      byte* cipher_text, int cipher_len);
    /**
       * Im processBlock wird ein Block des Klartextes übergeben und als Geheimtext wieder zurückgegeben.
       *
       * @details Vorgehen des processBlock:
       * Der übergebene Klartext wird inital mit einer Eingangspermutation die in der Variable ip gesichert ist,
       * modifiziert. Anschließend wird der Text in zwei gleich große Teile aufgeteilt. Diese werden folgend
       * an die Methode feistel übergeben. Die feistel Methode wird nun 16 mal ausgeführt. Nach diesen 16 Runden,
       * werden die beiden Teile wieder zusammengeführt. Allerdings ist darauf zu achten, dass beim zusammenführen,
       * die Linke und die Rechte Seiten vertauscht sind. Abschließend wird die finale Permutation ausgeführt und
       * der Text wird in der variable out_block gesichert und zurückgegeben.
       *
       * @details
       *
       * @param in_block Eingabenblock zur Ver- oder Entschlüsselung
       * @param out_block Ausgabenblock
       *
       */
  void processBlock(const byte* in_block, byte* out_block);

    /**
     * Die Funktion repräsentiert die Rundendurchläufe eines Feistel Netzwerk welches gewährleistet, dass die Blockchiffre
     * umkehrbar ist und sowohl Verschlüsselung als auch Entschlüsselung in einem Schaltkreis realisierbar sind.  Dabei
     * stellt ein Aufruf der Funktion feistel() einen Rundendurchlauf der Feistel Chiffre dar. In jedem Durchlauf wird
     * zu Beginn der rechte Eingabeblock in den linken Ausgabeblock geschrieben, da dieser unverändert zurückgegeben wird.
     * Anschließend wird mithilfe der functionF() der rechte Eingabeblock "chiffriert" (mehr dazu in functionF()).
     * Die Ausgabe der functionF() wird dann mit dem linken Eingabeblock "XORed" und als rechter Ausgabeblock zurückgegeben.
     *
     * \image html feistel_cipher.png
     * @param l_in Die linken 32 Bit des Blocks; l0
     * @param r_in Die rechten 32 Bit des Blocks; r0
     * @param key Der jeweilige Rundenschlüssel
     * @param l_out Die linken 32 Bit der Ausgabe. Entspricht r_in.
     * @param r_out Die rechten 32 Bit der Ausgabe. Entspricht l_in XOR f(r_in)
     */
  void feistel(const byte* l_in, 
               const byte* r_in, 
               const byte* key, 
               byte* l_out, 
               byte* r_out);

    /**
     * Diese Methode repräsentiert die Funktion F des Feistel Netzwerks, welche für Chiffrierung des rechten Eingabeblocks
     * mit dem aktuellen Rundenschlüssel zuständig ist. Um den rechten Eingabeblock (32 bit) mit dem aktuellen
     * Rundeschlüssel (48 bit) "XORen" zu können, muss dieser auf 48 bit expandiert werden. Diese berechnung wird mittels
     * permutate() und dem Array ev[48] durchgeführt. Das Ergebnis der Expansion wird anschließend mit dem Rundenschlüssel
     * "XORed". Nun liegt der Eingabeblock chiffriert vor, ist mit 48 bit jedoch zu lang und muss deshalb auf 32 bit "geschrumpft"
     * werden. Dazu werden sogenannte Substitutionsboxen (S-Boxen) verwendet, mithilfe derer jeweils 6 bit des chiffrierten
     * Blocks auf 4 bit aus den S-Boxen abgebildet wird. Zur Berechnung des Substitus verwenden wir das 1. und 6. Bit
     * als Zeilenindex und die Bits 2 bis 5 als Spaltenindex. Die bits extrahieren wir in ein Bitset, da dieses mithilfe der
     * Methode to_ulong() die Bits zu einer ganzzahl konvertiert. Abschließend wird die Methode computeSBox() mit den
     * Indizes der Reihe und der Spalte aufgerufen und gibt den transformierten 4 bit Block zurück. Das Ergebnis der Substitution
     * soll in einem Array vom Typ Byte gespeichert werden, weshalb die Ausgabebits der S-Boxen mit geradzahigem Index
     * jeweils um 4 Bit nach links geshifted werden. Anschließend werden die niederen und die oberen 4 Bit Bitweise
     * "ORed" wodurch wir jeweils einen gültigen Byte-Block erhalten. Abschließend erfolgt eine weitere Transposition
     * der 32 Ausgabebits mithilfe von permutate() und das ergebnis wird in r_out gespeichert.
     *
     * \image html function_f.png
     * @param r_in rechter Eingabeblock der Methode feistel()
     * @param key aktueller Rundenschlüssel
     * @param r_out resultat eines durchlaufs der functionF()
     */
  void functionF(const byte* r_in, 
                 const byte* key, 
                 byte* r_out);

    /**
    * Die Methode getBit() ermöglicht den Zugriff auf ein bestimmtes Bit in einem Byte Array. Dazu wird zuerst das
    * jeweilige Byte im Array berechnet, indem man den Index durch 8 teilt. Anschließend bestimmt man die relative Position
    * im jeweiligen Byte, indem man die Position Modulo 8 rechnet und somit einen Index zwischen 0 und 7 erhält. Da die
    * niederwertigen 4 Bit auf der "rechten" Seite des Bytes stehen, muss der Index von 7 abgezogen werden, woraus die
    * Bitposition resultiert. Abschließend wird das byte b mit dem Wert 1 initialisiert und dieses Bit um den zuvor
    * berechneten Index nach links verschoben und dann mit der Position im Byte Array mittels Bitweisem AND verglichen.
    * Ist das Bit an dieser Position 1, gibt die Methode true (1) zurück, ansonsten false (0).
    *
    * @param array Byte Array, in dem das Bit gefunden werden soll
    * @param array_len länge des Arrays
    * @param pos Position des Bits im Array
    * @return true wenn Bit gesetzt, sonst false
    */
  bool getBit(const byte* array, int array_len, int pos) const;

    /**
     * Diese Methode ordnet die Bits im Bytefeld in_array gemäß der Tabelle im Array p um und speichert das Ergebnis
     * im Array out_array. Diese Methode wird benötigt, um sowohl die im DES Algorithmus benötigten Permutationen als auch
     * die Expansionen und Substitutionen durchzuführen. Dazu wird über alle Elemente im Array p iteriert und mithilfe
     * der Methode setBit() und getBit() das korrespondierende Bit an die in Array p angegebene Position gesetzt. Da
     * alle vorgegebenen Tabellen bei Index 1 beginnen, muss der jeweilige Wert beim Aufruf von getBit() um - 1 verringert
     * werden (p[i] - 1).
     *
     * @param p Tabelle, gemäß derer die Werte umgeordnet werden
     * @param p_len größe der Tabelle in Byte
     * @param in_array Bytefeld, dessen Bits umsortiert werden sollen
     * @param in_len größe des Bytefelds in Byte
     * @param out_array Bytefeld, in dem die umsortierten Bits gespeichert werden
     * @param out_len größe des Bytefelds in Byte
     */
  void permutate(const byte* p, int p_len, 
                 const byte* in_array, int in_len, 
                 byte* out_array, int out_len) const;
    /**
    * Methode, die ein Byte Array als Bitvector formatiert ausgibt. Um die Anzahl der Bits im Array zu berechnen, wird
    * die größe des Arrays mit 8 multipliziert. Anschließend wird über die Anzahl der Bits iteriert und in jedem durchlauf
    * ein Bit ausgegeben. Jedes mal wenn ein Block der größe block_len ausgegeben wurde, wird zusätzlich ein Leerzeichen
    * ausgegeben.
    *
    * @param bytefield Array, das ausgegeben werden soll
    * @param len Größe des Arrays in Byte
    * @param block_len Größe eines Blocks
    */
  void printBitField(const byte* bytefield, int len, int block_len=8) const;

    /**
    * Die Methode setBit() ermöglicht das setzen eines bestimmten Bits in einem Byte Array mit dem Wert value. Dazu wird
    * zuerst das jeweilige Byte im Array berechnet, indem man den Index durch 8 teilt. Anschließend bestimmt man die
    * relative Position im jeweiligen Byte, indem man den Position Modulo 8 rechnet und somit einen Index zwischen 0 und
    * 7 erhält. Da die niederwertigen 4 Bit auf der "rechten" Seite des Bytes stehen, muss der Index von 7 abgezogen
    * werden, woraus die Bitposition resultiert. Abschließend wird das byte b mit dem Wert 1 initialisiert und dieses Bit
    * um den zuvor berechneten Index nach links verschoben. Um das Bit zu setzen, wird zuerst geprüft, ob das zu setzende
    * Bit 0 oder 1 ist. Ist das Bit 1, wird das gesamte Byte des Arrays mit dem Byte b "ORed", wodurch alle Bits im
    * Array unverändert übernommen werden und das Bit an der gewünschten Position auf 1 gesetzt wird. Ist das zu setzende
    * Bit 0, wird das Byte b zuerst mit 0xFF "XORed", wodurch alle Bits außer dem gewünschten auf den Wert 1 gesetzt werden.
    * Zuletzt wird das gesamte Byte des Arrays mit dem Byte b "ANDed", wodurch alle bereits gesetzten Bits im Array
    * den Wert 1 behalten und nur das zu setzende Bit den Wert 0 erhält.
    *
    * @param array Byte Array, in dem das Bit ersetzt werden soll
    * @param array_len größe des Arrays
    * @param pos Position des Bits im Array
    * @param value Wert, auf den das Bit gesetzt werden soll (True = 1, False = 0)
    */
  void setBit(byte* array, int array_len, int pos, bool value) const;

};

#endif
