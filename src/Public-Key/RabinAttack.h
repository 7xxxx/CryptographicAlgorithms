/*
 * RabinAttack.h
 */

#ifndef RABINATTACK_H_
#define RABINATTACK_H_

#include "integer.h"
#include "RabinDecryptor.h"

using namespace CryptoPP;

class RabinAttack {
public:
	RabinAttack();
	virtual ~RabinAttack();

    /**
     * Angriff auf das Rabin Kryptosystem mittels Berechnung eines Faktors unter zuhilfenahme des öffentlichen Schlüssels.
     *
     * @details
     * Das Rabin Kryptosystem ist anfällig gegen Chosen-Ciphertext Attacken. Ist der Entschlüsselungsexponent bekannt,
     * kann ein Faktor in polynomieller Zeit faktorisiert werden. Zur Berechnung wählt man eine zufällige Zahl
     * zwischen 1 und n-1, wobei n der öffentliche Schlüssel ist. Anschließend wird diese Zahl entschlüsselt und in x gespeichert,
     * sodass der Ciphertext bekannt ist. Daraufhin wird geprüft, ob x Kongruent zu r mod n oder -r mod n ist und damit
     * eine triviale Quadratwurzel darstellt. Trifft dies zu, so wird ein weiterer Schleifendurchlauf gestartet und eine neue
     * Zufallszahl getestet. Sobald eine faktorisierbare Zahl gefunden wurde, bestimmt  man den ggT dieser und teilt den
     * öffentlichen Schlüssel durch den ggT dieser Zahl und erhält einen gültigen Faktor von n.
     *
     * @param n Öffentlicher Schlüssel
     * @param f Berechneter Faktor von n
     * @param max_tries Maximale Anzahl Versuche bis Abbruch
     * @param rabin_dec Instanz der Klasse RabinDecryptor zur Entschlüsselung mithilfe des Rabin Kryptosystems
     *
     * @return
     */
	int factorize(const Integer& n, Integer& f, int max_tries, RabinDecryptor& rabin_dec);
};

#endif /* RABINATTACK_H_ */
