#ifndef AES_AESMATH_H
#define AES_AESMATH_H

#include <string>
#include <vector>
#include "datatypes.h"
#include <cassert>

using namespace std;

class AESMath {
private:
    vector<byte> log_table;
    vector<byte> exp_table;
    vector<byte> sbox;
    vector<byte> inv_sbox;


public:
    /**
     * Die SBox sowie die inverse SBox werden innerhalb des Konstruktors berechnet.
     *
     * @details
     * Berechnung der SBox: Dazu wird über jeden einzelnen Index (von 0-255) iteriert und die SBox an der Position des index mit der
     * linearen Transformation des inversen Elements des aktuellen Index befüllt. Dazu wird die Methode atrans() mit dem
     * inversen Element des Index aufgerufen.
     *
     * Berechnung der inversen SBox: Die inverse SBox wird berechnet, indem man den Inhalt der SBox an Position i
     * als index verwendet und mit dem index i beschreibt.
     */
    AESMath();

    /**
     * Implementierung der Addition über GF[256].
     *
     * @details
     * Diese wird in einem endlichen Feld durch die Addition der Koeffizienten erreicht und kann effizient durch
     * XORen der beiden Bytes durchgeführt werden.
     *
     * @param a Erstes Byte für die Addition
     * @param b Zweites Byte für die Addition
     *
     * @return Gibt das Ergebnis der Addition zurück
     */
    static byte add(byte a, byte b);

    /**
     * Rückgabe eines Wertes in der Tabelle exp_table
     *
     * @details
     * Gibt den Wert an der Stelle i aus der exp_table zurück. Die exp_table wurde im Konstruktor berechnet
     * und enthält alle Werte in einem Byte-Vector. Die Werte der exp_table enthalten die Ergebnisse der
     * Exponentiation 3 hoch i über GF(256).
     *
     * @param i Gibt den Exponenten an
     *
     * @return Gibt das Ergebnis der Exponentiation zurück
     */
    byte exp(byte i) const;

    /**
     * Berechnung der Multiplikativen Inversen eines Elements in GF(256)
     *
     * @details
     * Diese Funktion berechnet das Multiplikative Inverse des übergebenen Elements b. Hier werden
     * die beiden Tabellen log_table und exp_table zur Hilfe genommen. Der Index i ist der log(b). Das inverse Element
     * von b = g hoch i wird als b hoch -1 = g hoch m - 1 - i berechnet, wobei m die Zahl des endlichen Feldes in unserem Fall 256 ist.
     *
     * @param b Für den Wert soll das Multiplikative Inverse berechnet werden
     *
     * @return Gibt das Multiplikative Inverse zurück
     */
    byte inv(byte b) const;

    /**
     * Rückgabe eines Wertes in der Tabelle log_table
     *
     * @details
     * Gibt den Wert an der Stelle i aus der log_table zurück. Die log_table wurde im Konstruktor berechnet
     * und enthält alle Werte in einem Byte-Vector. Die Werte der log_table enthalten die Ergebnisse der
     * Funktion log zur Basis 3 von b über GF(256).
     *
     * @param b Gibt den Wert an, für den die log Funktion berechnet werden soll
     *
     * @return Gibt das Ergebnis der log Funktion zurück
     */
    byte log(byte b) const;

    /**
     * Multiplikation der übergebenen Bytes a und b über GF(256)
     *
     * @details
     * Die Multiplikation kann anhand der im Konstruktor berechneten Werte der log_table und exp_table
     * effizient implementiert werden. Der Aufwand der Funktion besteht lediglich darin die beiden Tabellen
     * zu durchsuchen und das Ergebnis der Multiplikation zurückzugeben.
     *
     * @param a Erstes Byte für die Multiplikation
     * @param b Erstes Byte für die Multiplikation
     *
     * @return Gibt das Ergebnis der Multiplikation zurück
     */
    byte mul(byte a, byte b) const;

    /**
     * Substitution des übergebenen Byte b mit Byte aus der SBox.
     *
     * @details
     * Dazu wird das Byte als Index verwendet, um das Substitut aus der SBox Tabelle
     * zu finden und zurückzugeben.
     *
     * @param b Gibt das byte an, worauf die Substitution angedwendet werden soll
     *
     * @return Gibt das Ergebnis der Substitution zurück
     */
    byte sBox(byte b) const;

    /**
     * Substitution des übergebenen Byte b mit Byte aus der inversen SBox.
     *
     * @details
     * Dazu wird das Byte als Index verwendet, um das Substitut aus der inversen SBox Tabelle
     * zu finden und zurückzugeben.
     *
     * @param b Gibt das byte an, worauf die inverse Substitution angewendet werden soll
     *
     * @return Gibt das Ergebnis der Substitution zurück
     */
    byte invSBox(byte b) const;

    /**
     * Berechnung der linearen Transformation
     *
     * @details
     * Berechnet den Wert der linearen Transformation. Hierfür werden zunächst die Matrix und der x Vektor
     * in einem Byte-Vektor geschrieben, um die Handhabung zu erleichtern. Für die Berechnung der Matrixmultiplikation
     * wird die Bitweise AND (&) funktion, Bitweise XOR (^) funktion sowie die parity funktion verwendet. Diese
     * Funktionen werden in jedem Schleifendurchlauf verwendet um das Resultierende Bit zu berechnen.
     *
     * @param x Gibt das byte an, worauf die lineare Transformation angewendet werden soll
     *
     * @return Gibt das Ergebnis der linearen Transformation zurück
     */
    static byte atrans(byte x);

    /**
     * Multiplikation der übergebenen Bytes a und b anhand der russischen Bauernmultiplikation
     *
     * @details
     * Das Verfahren der russischen Bauernmultiplikation nutzt die Funktionen der Addition, Verdoppelung
     * und Halbierung, da diese sehr effizient implementiert werden können. Hier wird für die Verdoppelung
     * die zuvor implementierte xtime funktion verwendet. Für die Addition wird die implementierte Funktion
     * Add verwendet. Für die Halbierung kann hier ein einfacher right shift verwendet werden.
     *
     * @param a Erstes Byte für die Russische Bauernmultiplikation
     * @param b Erstes Byte für die Russische Bauernmultiplikation
     *
     * @return Gibt das Ergebnis der Russischen Bauernmultiplikation zurück
     */
    static byte rpmul(byte a, byte b);

    /**
     * Prüfung, ob Anzahl der Einsen in Binärdarstellung gerade oder ungerade ist.
     *
     * @details
     * Zur Berechnung wird innerhalb einer Schleife jedes Bit geprüft, ob dieses gesetzt ist. Trifft dies zu, wird der
     * Zähler inkrementiert und die Bits um 1 nach Rechts geshiftet. Nachdem jedes Bit geprüft wurde, wird der Zähler
     * Modulo 2 gerechnet. Ist die Anzahl der Bits gerade, wird 0 zurückgegeben, anderenfalls 1.
     *
     * @param b Zu überprüfendes Byte auf die Anzahl der Einsen
     *
     * @return Gibt 0 bei gerader Anzahl an Einsen, 1 bei ungerader Anzahl zurück
     */
    static byte parity(byte b);

    /**
     * Ausgabefunktion für die Formatierung der Tabellen
     *
     * @details
     * Implementierung der Funktion zur Ausgabe von Tabellen wie z.B. exp_table, log_table, sbox oder
     * inv_sbox. Wird nur für die Debug Ausgaben genutzt.
     *
     * @param table Die auszugebende Tabelle
     */
    static void printTable(const vector<byte>& table);

    /**
     * Implementierung der Verdoppelung (Grundoperation der Bauernmultiplikation)
     *
     * @details
     * Das übergebene Byte b wird verdoppelt, indem die Bits um eine Position nach links geshifted werden.
     * Falls der Grad des Polynoms 8 ist, muss vom Ergebnis des Shifts das irreduzible Polynom 0x011b subtrahiert werden.
     * Da Addition und Subtraktion über GF[256] identisch sind, kann diese Berechnung mit XOR erfolgen. Des Weiteren
     * reicht 0x1b, da nur auf einem Byte operiert wird und das hochwertigste Bit durch XOR 0 werden würde.
     *
     * @param b Byte zur Verdoppelung über GF[256]
     *
     * @return Gibt das Ergebnis der Verdoppelung zurück
     */
    static byte xtime(byte b);

    /**
     * Gibt das übergebene Byte in formatierter Form zurück
     *
     * @param b Das zu formatierende Byte
     *
     * @return Gibt das Formatierte Byte als string zurück
     */
    static string format(byte b);

};


#endif //AES_AESMATH_H
