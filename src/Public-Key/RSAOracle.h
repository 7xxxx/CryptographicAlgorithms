/*
 * RSAOracle.h
 */

#ifndef RSAORACLE_H_
#define RSAORACLE_H_

#include "integer.h"

using namespace std;
using namespace CryptoPP;

class RSAOracle {
private:
	Integer n;
	Integer d;

public:
	RSAOracle(Integer& p, Integer& q, Integer& d);
	virtual ~RSAOracle();

    /**
     * Berechnung des höchstwertigsten Bits der Zahl des Klartexts.
     *
     * @details
     * Die Berechnung erfolgt mittels der modularen Exponentation die der Entschlüsselung entspricht. Ist das Ergebnis der
     * Entschlüsselung kleiner als n / 2 und gleichzeitig größer oder gleich 0, so muss das Bit 0 sein.
     * Ist das Ergebnis dagegen größer als n / 2 und kleiner oder gleich n - 1, so muss das Bit 1 sein.
     *
     * @param y Geheimtext
     *
     * @return Wert des Höchstwertigen Bits (0 oder 1)
     */
	bool half(const Integer& y) const;

	bool parity(const Integer& y) const;

};

#endif /* RSAORACLE_H_ */
