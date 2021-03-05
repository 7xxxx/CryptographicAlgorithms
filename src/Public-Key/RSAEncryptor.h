/*
 * RSAEncryptor.h
 */

#ifndef RSAENCRYPTOR_H_
#define RSAENCRYPTOR_H_

#include "integer.h"

using namespace CryptoPP;

class RSAEncryptor {
private:
	Integer n;
	Integer e;

public:
    /**
     * Konstruktor RSA Encryptor
     *
     * @details Der Konstruktor des Encryptors erhält den öffentlichen Schlüssel (n und e) als Parameter und sichert diese
     *
     * @param n Ein Teil des öffentlichen Schlüssels
     * @param e Ein Teil des öffentlichen Schlüssels
     */
	RSAEncryptor(const Integer& n, const Integer& e);
	virtual ~RSAEncryptor();

    /**
     * Verschlüsseln der übergebenen Nachricht
     *
     * @details Die Methode verschlüsselt die übergebene Nachricht x und sichert diese in y. Zu erst wird die
     * PublicAlgorithmBox referenziert, um Zugriff auf die dort enthaltenen Methoden zu erhalten. Die Verschlüsselung
     * der Nachricht x ist eine Modulare Exponentiation der Parameter x, e und n (wobei n = p * q ist). Die Formel für die Verschlüsselung lautet
     * x ^ e mod n.
     *
     * @param x Zu verschlüsselnde Nachricht
     * @param y Parameter um die verschlüsselte Nachricht zu sichern
     *
     * @return Gibt True zurück falls x < n ist, andernfalls False
     */
	bool compute(const Integer& x, Integer& y) const;
};

#endif /* RSAENCRYPTOR_H_ */
