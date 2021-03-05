/*
 * RabinEncryptor.h
 */

#ifndef RABINENCRYPTOR_H_
#define RABINENCRYPTOR_H_

#include "integer.h"

using namespace CryptoPP;

class RabinEncryptor {
private:
	Integer n;
	Integer offset;
	Integer padding;

public:
    /**
     * Konstruktor Rabin Encryptor
     *
     * @details Der Konstruktor des Encryptors erhält den öffentlichen Schlüssel (n) und das
     * zu benutzende padding als Parameter und sichert diese
     *
     * @param n Öffentlicher Schlüssel
     * @param padding Padding das an die zu verschlüsselnde Nachricht angehängt wird
     */
	RabinEncryptor(const Integer& n, const Integer& padding=Integer("987654321"));
	virtual ~RabinEncryptor();

    /**
     * Verschlüsseln der übergebenen Nachricht
     *
     * @details Die Methode verschlüsselt die übergebene Nachricht x und sichert diese in y. Die Formel für die Verschlüsselung
     * lautet x ^ 2 mod n.
     *
     * @param x Zu verschlüsselnde Nachricht
     * @param y Parameter um die verschlüsselte Nachricht zu sichern
     */
	bool compute(const Integer& x, Integer& y);

    /**
     * Verschlüsseln der übergebenen Nachricht mit Padding
     *
     * @details Die Methode verschlüsselt die übergebene Nachricht x und sichert diese in y. Der Unterschied
     * zur compute() Funktion besteht darin, dass hier das Padding mithilfe eines String streams angehängt wird.
     * Anschließend wird die Nachricht mit Padding and die compute() Funktion übergeben.
     *
     * @param x Zu verschlüsselnde Nachricht
     * @param y Parameter um die verschlüsselte Nachricht zu sichern
     */
	bool compute2(const Integer&x, Integer& y);
};

#endif /* RABINENCRYPTOR_H_ */
