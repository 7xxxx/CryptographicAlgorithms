/*
 * RabinDecryptor.h
 */

#ifndef RABINDECRYPTOR_H_
#define RABINDECRYPTOR_H_

#include <vector>
#include "integer.h"

using namespace CryptoPP;
using namespace std;

class RabinDecryptor {
private:
	Integer p;
	Integer q;
	Integer padding;
	Integer offset;

public:
    /**
     * Konstruktor Rabin Decryptor
     *
     * @details Der Konstruktor des Decryptors erhält den privaten Teil des Schlüssels (p, q) und das Padding
     * als Parameter und sichert diese
     *
     * @param p Ein Teil des privaten Schlüssels
     * @param q Ein Teil des privaten Schlüssels
     * @param padding Padding das von der entschlüsselten Nachricht entfernt werden musss
     */
	RabinDecryptor(const Integer& p, const Integer& q, const Integer& padding=Integer("987654321"));
	virtual ~RabinDecryptor();

    /**
     * Entschlüsseln der übergebenen Nachricht
     *
     * @details Die Methode entschlüsselt die übergebene Nachricht y und sichert diese im xv. Zu erst wird die
     * PublicAlgorithmBox referenziert, um Zugriff auf die dort enthaltenen Methoden zu erhalten. Ein Teil der Entschlüsselung
     * der Nachricht y ist die Modulare Exponentiation der Parameter y, p, q. Mithilfe der Modularen Exponentiation
     * können alle vier Möglichen Ergebnisse für das Rabin System berechnet werden (Seite 36 Vorlesungsfolien). Die vier
     * Lösungen werden mithilfe des Chinesischen Restsatzes berechnet.
     *
     * @param y Die zu entschlüsselnde Nachricht
     * @param xv Vektor mit den vier möglichen Lösungen
     */
	bool compute(const Integer& y, vector<Integer>& xv);

    /**
     * Entschlüsseln der übergebenen Nachricht
     *
     * @details Die Methode entschlüsselt die übergeben Nachricht y und sichert diese in x. Hier handelt es sich um eine
     * überladene Methode. Diese ruft lediglich die zuvor implementierte compute() Methode mit einem Integer Vektor auf.
     *
     * @param y Die zu entschlüsselnde Nachricht
     * @param x
     */
	bool compute(const Integer& y, Integer& x);

    /**
    * Entschlüsseln der übergebenen Nachricht mit Padding
    *
    * @details Die Methode entschlüsselt die übergebene Nachricht y und sichert diese im xv. Zu erst wird die
    * Nachricht an die Compute Funktion übergeben, um sie zu entschlüsseln. Anschließend wird bei jeder Funktion
    * nach dem Padding gesucht und am Ende der Nachricht entfernt, sofern vorhanden. Sobald die Lösung mit Padding
    * gefunden wurde wird diese in x gesichert.
    *
    * @param y Die zu entschlüsselnde Nachricht
    * @param x Entschlüsselte Nachricht ohne Padding
    */
	bool compute2(const Integer& y, Integer& x);
};

#endif /* RABINSYSTEM_H_ */
