/*
 * RSAAttack.h
 */

#ifndef RSAATTACK_H_
#define RSAATTACK_H_

#include "integer.h"
#include "RSAOracle.h"

using namespace CryptoPP;

class RSAAttack {
public:
	RSAAttack();
	virtual ~RSAAttack();

    /**
     * Berechnung eines Faktors des öffentlichen Schlüssels mittels n und phi(n).
     *
     * @details
     * Sind die Werte für n und phi(n) bekannt, so kann man die Faktoren von n effizient berechnen. Dazu stellt man die Formel
     * n = p * q nach q um und setzt diese in phi(n) = (p - 1) * (q - 1) ein. Wird diese Formel nach 0 umgeformt, erhält man
     * die Gleichung 0 = p^2 + (phi(n) - n - 1) * p + n. Setzt man nun a = 1, b = phi(n)-n-1, c = n, so kann man die Faktoren
     * mittels der Mitternachtsformel (pq-Formel), berechnen. Ist die Gleichung lösbar, sind die beiden Ergebnisse die Faktoren p und q.
     *
     * @param n RSA-Modul
     * @param phi_n Totient von n
     * @param p Faktor p
     * @param q Faktor q
     *
     * @return True, wenn Faktoren berechnet wurden, sonst False
     */
	bool factorizeN(const Integer& n, const Integer& phi_n, Integer& p, Integer& q) const;

    /**
     * Berechnung der beiden Faktoren p und q mittels der Wiener Attacke.
     *
     * @details
     * Die Wiener Attacke beruht auf einer ungünstigen Wahl des privaten Schlüsselexponenten. Die Vorraussetzungen für
     * einen erfolgreichen Angriff sind:
     * <ul>
     *  <li> q < p < 2 * q -> p und q liegen nicht zu weit auseinander</li>
     *  <li> 3 * d < n^(1/4) -> d ist entsprechend kleiner als n</li>
     * </ul>
     * Der Ablauf ist nun wie folgt: Zuerst werden die Konvergenten des Kettenbruchs e / n berechnet.
     * Anschließend wird über jede Konvergente iteriert und geprüft, ob diese anhand von phi(n) faktorisierbar ist. Dazu wird
     * phi(n) mittels der Formel phi(n) = (e * d[1] - 1) / c[i] berechnet, wobei c und d der Zähler und Nenner der Konvergente
     * sind. War die Faktorisierung erfolgreich, wurden die Faktoren erfolgreich berechnet.
     *
     * @param n RSA-Modul
     * @param e RSA pubkey Exponent
     * @param p Faktor p von n
     * @param q Faktor q von n
     *
     * @return True wenn Angriff erfolgreich, sonst False.
     */
	bool wienerAttack(const Integer& n, const Integer& e, Integer& p, Integer& q) const;

    /**
     * Berechnung des Klartexts auf Basis des öffentlichen Schlüssels.
     *
     * @details
     * Zur Berechnung macht sich der Algorithmus die multiplikative Eigenschaft und die Half Funktion zu nutze. Zu Beginn
     * wird die Anzahl der Bits von n berechnet. Anschließend wird über die Anzahl an Bits in n iteriert und mittels der
     * Half Funktion das höchstwertigste Bit von y in der Variable h gesetzt sowie der Geheimtext y_tmp mit der Modularen
     * Exponentation (2^e mod n) multipliziert. Nach dem die Bitdarstellung berechnet wurde, wird mittels Binärer Suche
     * ein gültiger Klartext bestimmt. Dazu wird lo auf 0 gesetzt sowie hi auf n. Damit eine Genauigkeit von 12 Nachkommastellen
     * eingehalten werden kann, muss hi jedoch mit 10^12 multipliziert werden. Nach erfolgreicher Berechnung enthält hi den
     * Klartext und muss zum Abschluss wieder durch 10^12 geteilt werden.
     *
     * @param n RSA Modul
     * @param e RSA pubkey Exponent
     * @param y Geheimtext
     * @param rsa_oracle Instanz der Klasse RSAOracle()
     *
     * @return Klartext von y
     */
	Integer halfAttack(const Integer& n, const Integer& e, const Integer& y, RSAOracle& rsa_oracle) const;

};

#endif /* RSAATTACK_H_ */
