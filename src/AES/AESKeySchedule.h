#ifndef AES_AESKEYSCHEDULE_H
#define AES_AESKEYSCHEDULE_H

#include <string>
#include <vector>
#include "datatypes.h"
#include "AESMath.h"
#include "AESState.h"

using namespace std;

class AESKeySchedule {
private:
    bool debug_mode;
    const AESMath *math;
    //word *key_schedule;
    vector<word> key_schedule;
    //anzahl 32bit woerter in schluessel 4=128;6=192;8=256
    size_t nk;
    //anzahl der runden; abh. von nk -> 4=10;6=12;8=14
    size_t nr;
    //anzahl der 32bit woerter im klartext. Bei aes immer 4, da blocksize=128
    size_t nb;
    //rundenkonstanten
    vector<word> r_con;

    /**
     * Durchführung einer zyklische Linksverschiebung um 8 Bit auf dem word w.
     *
     * @details
     * Zuerst werden 8 Bit des Worts nach links verschoben, sodass das höchstwertige Byte auf 0 gesetzt wird die restlichen
     * Bytes die nach links geshifteten werte entalten. Anschliessend werden 24 bit des Worts nach rechts
     * verschoben, wodurch die Bytes 0, 1 und 2 auf 0 gesetzt werden und das 3 Byte die Werte enthält, welche durch den
     * vorherigen linksshift aus dem Wort geschoben wurden. Anschließend werden beide Wörter verodert, wodurch alle
     * bytes korrekt gesetzt werden.
     *
     * @param w Word für die zyklische Linksverschiebung
     *
     * @return Gibt das Ergebnis der zyklischen Linksverschiebung zurück
     */
    word rotWord(word w) const;

    /**
     * Anwendung der Sbox auf das word w
     *
     * @details
     * Hierbei wird das übergebene word w, byte für byte mit der Funktion sBox ersetzt. Es wird die zuvor berechnete
     * SBox zur Ersetzung der Werte genutzt.
     *
     * @param w Word für die Substitution mit der SBox
     *
     * @return Gibt das Ergebnis der Substitution zurück
     */
    word subWord(word w) const;

public:
    /**
     * Konstruktor AESKeySchedule
     *
     * @details
     * Im Konstruktor des AESKeySchedule werden die Rundenkonstanten berechnet. Für die Berechnung der Rundenkonstanten
     * wird eine Schleife 10x durchlaufen. In der ersten Iteration ist die Rundenkonstante 1, sobald die Iteration über
     * 1 hinausläuft, gibt es eine Fallunterscheidung ob das vorherige Ergebnis kleiner 0x80 ist, wenn dies der Fall
     * ist dann ist das Ergebnis 2 * r_con[i-1]. Ist das vorherige Ergebnis größer gleich 0x80, so ist das Ergebnis
     * 2 * r_con[i-1] XOR 0x11b.
     */
    AESKeySchedule(const AESMath &aesmath, bool debug_mode=false);
    virtual ~AESKeySchedule();

    /**
     * Berechnung der Schlüsselexpansion auf Basis des übergebenen Schlüssels key.
     *
     * @details
     * Zu Beginn muss wird die Schlüssellänge berechnet und anhand derer die Rundenanzahl bestimmt. Da die Blockgröße
     * bei AES 4 Byte beträgt, wird die Rundenanzahl entweder auf 10, 12 oder 14 festgelegt, abhängig ob der Schlüssel
     * 4, 6 oder 8 Byte groß ist. Anschließend werden jeweils 4 Byte des Schlüssels zu einem Schlüsselwort zusammengefügt,
     * indem das Schlüsselbyte mit 0 verodert wird und anschließend um eine Position nach links geschoben wird.
     * Innerhalb der Schleife findet nun die Expansion statt. Da die ersten nk Wörter des Schlüssels unverändert übernommen
     * werden, beginnt die Schleife bei i = Schlüsselgröße und läuft solange i kleiner als die blockgröße * (Rundenzahl + 1).
     * Bei einer Schlüsselgröße von 4 byte würde die Schleife also von 4 bis 43 laufen, da 4 * (10 + 1) = 44.
     * Während des Schleifendurchlaufs wird nun unterschieden zwischen 3 fällen, wobei fall 1 immer durchgeführt wird und
     * bei Fall 2 und Fall 3 eine Transformation des Worts vor Berechnung von Fall 1 ausgeführt wird:
     *
     * Fall 1 Normaler durchlauf, wird immer ausgeführt:
     * Das expandierte Wort entspricht der Formel: w[i]=w[i-1] XOR w[i-nk]
     *
     * Fall 2 Die Position des Worts ist ein vielfaches der Schlüsselgröße:
     * Zuerst wird das Wort w[i-1] mithilfe der zyklischen Linksverschiebung um 8 bit (rotWord) transformiert. Anschließend
     * wird das Ergebnis des Shifts mithilfe der SBox substituiert. Danach wird die aktuelle Rundenkonstante bestimmt und
     * an die niederwertigste Position geshiftet um abschließend mit dem Ergebnis der Substitution XORed zu werden.
     * Dies entspricht der Formel: w[i-1] = subWord(rotWord(w[i-1])) XOR (r_con[i / nk] << 24)
     *
     * Fall 3 Der Schlüssel ist größer als 6 Byte und i-4 ist ein vielfaches von Schlüsselgröße:
     * w[i-1] wird durch die entsprechenden Werte der SBox substituiert. Formel: w[i-1]=subWord(w[i-1])
     *
     * Abschließend wird der Vector key_schedule mit den Berechneten Worten befüllt und die Expansion war erfolgreich.
     *
     * @param key Schlüssel
     *
     * @return True bei erfolgreicher Berechnung, sonst false
     */
    bool setKey(const vector<byte>& key);

    /**
     * Gibt den Rundenschlüssel an der übergebenen Runde zurück
     *
     * @param round Gibt die Runde an
     *
     * @return Gibt den Rundenschlüssel zurück
     */
    const word* getRoundKey(size_t round);

    /**
     * Formatiert den Rundenschlüssel an der übergebenen Runde
     *
     * @param round Gibt die Runde an
     *
     * @return Gibt den Formatierten Rundenschlüssel als String zurück
     */
    string formatRoundKey(size_t round);

    /**
     * Gibt die Anzahl der Runden zurück
     *
     * @return Gibt die Anzahl der Runden zurück
     */
    size_t getNrOfRounds() const;

};

#endif //AES_AESKEYSCHEDULE_H
