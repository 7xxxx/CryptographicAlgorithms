#ifndef _VigenereCipher_h_
#define _VigenereCipher_h_

#include "ClassicCipher.h"

/**
 * @class A class for the affine cipher.
 * @author Christoph Karg
 *
 * A class for the  Vigenere  cipher.
 */
class VigenereCipher : public ClassicCipher {
public:
  /**
   * Decrypt a cipher text.
   * @details To decrypt a ciphertext with the vigenere cipher each character of the ciphertext gets shifted by the
   * index of the corresponding character of the key. The index is determined by the relative position from the
   * first character in the alphabet. The range is from 0 to 25, for example 'A' is on index 0, 'B' on index 1 and so on.
   * The last character is 'Z' on index 25. If the calculation gets past 25, we start again at 0. Let's imagine a
   * ciphertext "GHI" and a key "DEF". To decrypt 'G' on index 6 with the key 'D' which is index 3, we have to
   * calculate 6 - 3 = 3. The character on index 3 of the alphabet is 'D'. The same calculation has to be done with all
   * characters of the ciphertext and the resulting plaintext will be "DDD". If the ciphertext is longer than the
   * plaintext, the key gets repeated until all characters are encrypted.
   *
   * In the code we iterate over each character of the ciphertext, add 26 to make sure that we don't calculate a
   * negative number and finally subtract the corresponding character of the key from it. To make sure that the key gets
   * repeated, the key index is calculated modulus the key length. Finally the result is also calculated modulus 26,
   * to make sure that the resulting index can be converted to a character of the alphabet.
   * @param cipher_text array to store cipher text
   * @param cipher_len length of the cipher text array
   * @param key key
   * @param key_len key length
   * @param plain_text plain text to be decrypted
   * @param plain_len length of the plain text
   *
   * @return number of decrypted bytes
   */
  virtual int decrypt(const vector<byte>& cipher_text, 
                      const vector<byte>&  key,
                      vector<byte>& plain_text);

   /**
   * Encrypt a plain text.
   * @details To encrypt a text with the vigenere cipher each character of the plaintext gets shifted by the index
   * of the corresponding character of the key. The index is determined by the relative position from the first
   * character in the alphabet. For example A is on index 0, B on index 1 and so on. The last character is 'Z' on
   * index 25. If the calculation gets past 25, we start again at 0. Let's imagine a plaintext "ABC"
   * and a key "DEF". To encrypt 'A' on index 0 with the key 'D' which is index 3, we calculate 0 + 3 = 3.
   * The character on index 3 of the alphabet is 'D'. The same calculation has to be done with all characters of the
   * plaintext and the resulting ciphertext will be "DFH". If the plaintext is longer than the ciphertext, the key gets
   * repeated until all characters are encrypted.
   *
   * In the code we iterate over each character of the plaintext and add the corresponding character of the key to it.
   * To make sure that the key gets repeated, the key index is calculated modulus the key length. Finally the result
   * is also calculated modulus 26, to make sure that the resulting character is in the alphabet.
   *
   * @param plain_text plain text to be encrypted
   * @param plain_len length of the plain text
   * @param key key
   * @param key_len key length
   * @param cipher_text array to store cipher text
   * @param cipher_len length of the cipher text array
   *
   * @return number of encrypted bytes
   */
  virtual int encrypt( const vector<byte>& plain_text,
                       const vector<byte>& key, 
                       vector<byte>& cipher_text);

}; // VigenereCipher

#endif
