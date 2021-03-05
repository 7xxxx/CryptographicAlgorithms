#ifndef _PublicKeyAlgorithmBox_h_
#define _PublicKeyAlgorithmBox_h_

#include <vector>
#include "integer.h"

using namespace std;
using namespace CryptoPP;

class PublicKeyAlgorithmBox {
public:

    /**
     * Berechnung Konvergenten eines Kettenbruchs
     *
     * @details Die Methode berechnet die Konvergenten des Kettenbruchs. Hierfür wird der Euklidische Algorithmus verwendet.
     * Anschließend werden die Konvergenten des Kettenbruchs berechnet. Die Werte für Cj und Dj werden anhand von
     * Fallunterscheidungen berechnet (Seite 66 Vorlesungsfolien). So wird bei C und D nach den Fällen j = 0, j = 1 und
     * j >= 2 unterschieden. Für jeden Fall gibt es fest definierte Formeln, die in den jeweils zugehörigen Vektoren
     * c und d gesichert werden.
     *
     * @param a Zähler des Bruchs
     * @param b Nenner des Bruchs
     * @param c Vektor zum sichern des Kettenbruchs
     * @param d Vektor zum sichern des Kettenbruchs
     */
	unsigned int computeConvergents(const Integer& a, const Integer& b,
			vector<Integer>& c, vector<Integer>& d);

    /**
     * Berechnung GCD und Kettenbruch
     *
     * @details Die Methode Berechnet den GCD(a,b) und die Werte für den Kettenbruch a / b. Diese Werte werden in den
     * entsprechenden Vektoren gesichert.
     *
     * @param a Natürliche Zahl
     * @param b Natürliche Zahl
     * @param c Vektor für das Sichern der Werte für den Kettenbruch
     * @param d Vektor für das Sichern der Werte für den Kettenbruch
     *
     * @return GCD von a und b
     */
	Integer euklid(const Integer& a, const Integer& b, vector<Integer>& q);

	/** Erweiterter Euklidscher Algorithmus zur bestimmung des ggT sowie des multiplikativen inversen.
	 * @details
	 * Neben der Berechnung des größten gemeinsamen Teilers zweier Zahlen berechnet der erweiterte
	 * Euklidische Algorithmus auch, ob diese ein multiplikatives Inverses besitzen.
	 *
	 * @param a Natürliche Zahl 1
	 * @param b Natürliche Zahl 2
	 * @param d ggT(a, b)
	 * @param x Parameter 1 sichern
	 * @param y Parameter 2 sichern
	 * @return Gibt true zurück wenn die zahlen a und b teilerfremd sind
	 */
	bool EEA(const Integer& a, const Integer& b, Integer& d,
			Integer& x, Integer& y);

    /**
     * Modulare Exponentiation
     *
     * @details Berechnet eine Modulare Exponentiation anhand der folgenden Formel a ^ b mod n. Diese wird anhand der
     * Binärdarstellung effizient berechnet.
     *
     * @param a Die Basis für die Modulare Exponentiation
     * @param b Der Exponent für die Modulare Exponentiation
     * @param n Der Modulo für die Modulare Exponentiation
     *
     * @return Das Ergebnis der Modularen Exponentiation
     */
	Integer modularExponentation(const Integer& a, const Integer& b,
			const Integer& n);

    /**
     * Berechnung Multiplikatives Inverses
     *
     * @details Für die Berechnung der Multiplikativen Inversen einer Zahl wird hier der Erweiterte Euklidische
     * Algorithmus zur Hilfe gezogen. Sobald das Ergebnis für d aus dem EEA ungleich 1 zurückgegeben wird, ist klar,
     * dass die Zahl kein Multiplikatives Inverses besitzt. Sollte das nicht der Fall sein so kann das Multiplikative
     * Inverse aus der Funktion in der Variable a_inv gesichert werden.
     *
     * @param a Zahl zur Berechnung des Multiplikativen Inversen
     * @param n Modulo
     * @param a_inv Variable für das Multiplikative Inverse
     *
     * @return Falls a ein Multikplikatives Inverses besitzt, soll True zurückgegeben werden
     */
	bool multInverse(const Integer& a, const Integer& n, Integer& a_inv);

    /**
     * Witness Primzahl-Test
     *
     * @details Die Witness Funktion belegt ob a nachweisen kann, dass n keine Primzahl ist. Hierfür wird eine
     * abgewandelte Form der Modularen Exponentiation verwendet. Und zwar berechnet der Algorithmus die Modulare
     * Exponentiation von a ^ n-1 mod n, und sucht nach Lösungen für die Gleichung x ^ 2 kongruent zu 1 mod n.
     *
     * @param a Parameter für die Festlegung ob n eine Primzahl ist
     * @param n Zu testende Primzahl
     *
     * @return True wenn n keine Primzahl ist
     */
	bool witness(const Integer& a, const Integer& n);

    /**
     * Miller Rabin Primzahltest
     *
     * @details Der Miller Rabin Primzahltest testet die Zahl n, ob diese keine Primzahl oder eine Primzahl mit einer
     * Fehlerwahrscheinlichkeit von 1 - 2 ^ -s ist. Hierfür wird für jeden Durchlauf, eine Zufallszahl gewürfelt,
     * und mit der Witness() Funktion getestet ob n eine Primzahl ist. Sobald die Witness Funktion true zurückliefert
     * ist klar, dass die Zahl keine Primzahl ist.
     *
     * @param n Zu testende Primzahl
     * @param s Anzahl an Durchläufen. Hiervon hängt die Fehlerwahrscheinlichkeit ab
     *
     * @return False, wenn n keine Primzahl
     */
	bool millerRabinTest(Integer& n, unsigned int s);

    /**
     * Erstellt eine Zufallszahl im Zahlenbereich
     *
     * @details Generiert mithilfe des Nonblocking RNG der Crypto++ Bibliothek, eine Zufallszahl im übergebenen
     * Zahlenbereich.
     *
     * @param from Untere Grenze für die Zufallszahl
     * @param to Obere Grenze für die Zufallszahl
     *
     * @return Gibt die Zufallszahl zurück
     */
    Integer randomInteger(const Integer& from, const Integer& to);

    /**
     * Erstellt eine Zufallszahl
     *
     * @details Generiert mithilfe des Nonblocking RNG der Crypto++ Bibliothek, eine Zufallszahl mit der übergebenen
     * Bitlänge.
     *
     * @param size Länge (Bits) der Zufallszahl
     *
     * @return Gibt die Zufallszahl zurück
     */
    Integer randomInteger(const size_t size);

    /**
     * Berechnung Primzahl
     *
     * @details Die Methode berechnet mithilfe der randomInteger() Funktion eine Zufallszahl. Diese wird mit dem
     * Miller Rabin Test auf eine Primzahl getestet. Sollte diese Zahl einen positiven Primzahltest zurückgeben
     * wird diese in der Variable p gesichert und die Anzahl an Versuchen zurückgegeben.
     *
     * @param p Variable zum sichern der Primzahl
     * @param bitlen Länge an Bits der Zufallszahl
     * @param s Anzahl an Durchläufen. Hiervon hängt die Fehlerwahrscheinlichkeit ab
     *
     * @return Gibt die Anzahl an Versuche zurück, bis eine Primzahl berechnet wurde
     */
	unsigned int randomPrime(Integer &p, unsigned int bitlen, unsigned int s);

    /**
     * Primzahl nach Rabin
     *
     * @details Die Methode berechnet eine Primzahl mithilfe der randomInteger() Funktion. Anschließend muss hier
     * der Miller Rabin Test prüfen ob die gewürfelte Zahl eine Primzahl ist. Ist dies der Fall, hat die Rabin Primzahl
     * noch eine Eigenschaft, und zwar muss diese noch kongruent 3 mod 4 sein. Sind diese Eigenschaften gegeben ist die
     * erwürfelte Zahl mit einer Fehlerwahrscheinlichkeit von 1 - 2 ^ -s eine Rabin Primzahl.
     *
     * @param p Variable zum sichern der Primzahl
     * @param bitlen Länge an Bits der Zufallszahl
     * @param s Anzahl an Durchläufen. Hiervon hängt die Fehlerwahrscheinlichkeit ab
     */
	unsigned int randomRabinPrime(Integer &p, unsigned int bitlen,
			unsigned int s);

    /**
     * Berechnung Quadratwurzel
     *
     * @details Die Methode Berechnet die Quadratwurzel der Formel y mod p, falls p kongruent 3 mod 4. Bei der
     * Berechnung der Quadratwurzel wird hier die SquareRoot() Funktion der Integer Klasse verwendet. Das Ergebnis
     * wird im Vektor v gesichert.
     *
     * @param y Parameter 1
     * @param p Modulo
     * @param v Vektor zum sichern des Ergebnises
     *
     * @return True, falls p kongruent 3 mod 4
     */
	bool modPrimeSqrt(const Integer& y, const Integer& p, vector<Integer>& v);

    /**
     * Berechnung Quadratwurzel
     *
     * @details Die Methode berechnet eine ganzzahlige nicht negative Quadratwurzel von x und sichert diese in der
     * Variable s. Für die Berechnung der Quadratwurzel wird die Binäre Suche zur Hilfe genommen.
     *
     * @param x Quadratwurzel aus x
     * @param s Variable für die Quadratwurzel
     *
     * @return True, falls eine ganzzahlige nicht negative Quadratwurzel gefunden wurde
     */
	bool sqrt(const Integer& x, Integer& s) const;

    /**
     * Generierung RSA-Schlüssel
     *
     * @details Die Methode berechnet alle wichtigen Faktoren für einen RSA-Schlüssel. Mithilfe der randomPrime()
     * Funktion werden zwei Primzahlen (p und q) für den privaten Schlüssel errechnet. Anschließend muss solange nach
     * einem e gesucht werden, dass diese zwei Eigenschaften einhält. Zum einen muss die Zahl e einen GCD(e, phi(n)) = 1
     * haben, aber auch ein multiplikatives Inverses besitzen. Dieses Multiplikative Inverse wird in der Variable d gesichert.
     *
     * @param p ein Teil des privaten Schlüssels
     * @param q ein Teil des privaten Schlüssels
     * @param e Zufallszahl, wobei der GCD(e, phi(n)) = 1 sein muss
     * @param d e ^ -1 mod phi(n)
     * @param bitlen Die Bitlänge der Zufallszahlen p und q
     * @param s Anzahl an Durchläufen. Hiervon hängt die Fehlerwahrscheinlichkeit ab
     */
	void generateRSAParams(Integer& p, Integer& q, Integer& e, Integer& d,
			unsigned int bitlen = 256, unsigned int s = 30);
};

#endif
