/*
 * RSADecryptor.h
 */

#ifndef RSADECRYPTOR_H_
#define RSADECRYPTOR_H_

#include "integer.h"

using namespace CryptoPP;

class RSADecryptor {
private:
	Integer p;
	Integer q;
	Integer d;
	Integer n;

public:
    /**
     * Konstruktor RSA Decryptor
     *
     * @details Der Konstruktor des Decryptors erhält den privaten Teil des Schlüssels (p, q, d) als Parameter und sichert diese
     *
     * @param p Ein Teil des privaten Schlüssels
     * @param q Ein Teil des privaten Schlüssels
     * @param d Ein Teil des privaten Schlüssels
     */
	RSADecryptor(const Integer& p, const Integer& q, const Integer& d);
	virtual ~RSADecryptor();

    /**
     * Entschlüsseln der übergebenen Nachricht mithilfe der Modularen Exponentiation
     *
     * @details Die Methode entschlüsselt die übergebene Nachricht y und sichert diese in x. Zu erst wird die
     * PublicAlgorithmBox referenziert, um Zugriff auf die dort enthaltenen Methoden zu erhalten. Die Entschlüsselung
     * der Nachricht y ist eine Modulare Exponentiation der Parameter y, d und n (wobei n = p * q ist). Die Formel für
     * die Entschlüsselung lautet y ^ d mod n.
     *
     * @param y Die zu entschlüsselnde Nachricht
     * @param x Parameter um die entschlüsselte Nachricht zu sichern
     *
     * @return Gibt True zurück falls y < n ist, andernfalls False
     */
	bool compute(const Integer& y, Integer& x) const;

    /**
     * Entschlüsseln der übergebenen Nachricht mithilfe des Chinesischen Restsatzes
     *
     * @details Die Methode entschlüsselt die übergebene Nachricht y und sichert diese in x. Diese Berechnung wird
     * mithilfe des Chinesischen Restsatzes durchgeführt. Der Chinesische Restsatz erlaubt es, die Zahlen der
     * Modularen Exponentiation zu verkleinern, und verspricht dadurch einen erheblichen Performance-Boost.
     *
     * Zunächst werden zwei Modulare Exponentiation durchgeführt. Diese werden jedoch mit den Faktoren (p und q)
     * von n durchgeführt. Anschließend werden mithilfe des Erweiterten Euklidischen Algorithmus, die Parameter d, mp
     * und mq berechnet die für die Entschlüsselung mit dem Chinesischen Restsatzes erheblich sind, ermittelt.
     * (Seite 30 Vorlesungsfolien).
     *
     * @param y Die zu entschlüsselnde Nachricht
     * @param x Parameter um die entschlüsselte Nachricht zu sichern
     *
     * @return Gibt True zurück falls y < n ist, andernfalls False
     */
	bool crt(const Integer& y, Integer& x) const;

    /**
     * Entschlüsseln der übergebenen Nachricht mithilfe der Garner-Methode
     *
     * @details Die Methode entschlüsselt die übergebene Nachricht y und sichert diese in x. Diese Berechnung wird
     * mithilfe der Garner-Methode durchgeführt. Die Garner-Methode berechnet zunächst ähnlich wie bei dem
     * Chinesischen Restsatz, zwei Modulare Exponentiationen mit den Faktoren (p und q) von n. Durch diese
     * Verkleinerung der Faktoren wird die Effizienz erhöht. Anschließend muss noch das Multiplikative Inverse von q
     * berechnet werden. (Seite 54 Vorlesungsfolien).
     *
     * @param y Die zu entschlüsselnde Nachricht
     * @param x Parameter um die entschlüsselte Nachricht zu sichern
     *
     * @return Gibt True zurück falls y < n ist, andernfalls False
     */
	bool garner(const Integer& y, Integer& x) const;
};

#endif /* RSADECRYPTOR_H_ */
