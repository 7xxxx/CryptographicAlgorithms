#ifndef VigenereBreaker_h_
#define VigenereBreaker_h_

#include <vector>
#include "VigenereCipher.h"
#include <sstream>
#include <iterator>
#include <iomanip>


/**
 * @class A class containing analysis tools to break
 * the Vigenere cipher
 * @author Christoph Karg
 */
class VigenereBreaker : public VigenereCipher {
protected:

  /**
   * Compute greatest common divisor of two integers a and b
   * 
   * @param a first integer
   * @param b second integer
   *
   * @return gcd(a,b)
   */
  int gcd(int a, int b) const;

  /** 
   * Compute the greatest common divisor of a vector of integers.
   *
   * The vector must be of size > 0.
   *
   * @param v vector containing at least 1 integer
   *
   * @return greatest common divisor of all integers in v
   */
  int gcd(const vector<int>& v) const;

public:

  /**
   * Perform Kasiski test to detect key length
   * @details The idea behind the kasiski test is that sequences of n characters that repeat in the ciphertext are
   * usually the same sequences in the plaintext and got encrypted with the same n characters of the key. That happens
   * because the key is relatively short compared to the ciphertext and gets repeated to fit the length of the
   * ciphertext. The distances between those sequences are usually a multiple of the key length. By calculating the
   * greatest common divisor (gcd) of the distances between the appearances of those n-grams relatively to the first
   * appearance, we can find out the key length. The best results can be computed with n=3 but if the result is not
   * clear the test gets repeated with n+1.
   *
   * To gather all information that are needed for the test the first step is to convert the ciphertext to
   * a string object to use basic string operations. The next step is to iterate over each character of the given
   * ciphertext to find all n-grams (n=3 by default) and store them together with their positions in the ciphertext.
   * Then all n-grams that occur 3 or more times have to be found and their relative positions to the first occurrence
   * of the n-gram has to be calculated and stored. The relative position is calculated by subtracting the index of
   * the first occurrence of the n-gram from the position of the actual n-gram. Finally the gcd of all relative
   * positions is calculated and stored. To find out the actual key length, the GCDs of the valid n-ngrams are compared
   * and the one with the most occurrences is probably the key length.
   * If the result is not clear (two GCDs appear equally often), the test is repeated with n = n+1.
   * If the n gets bigger than 5, the test will be aborted and return 0.
   * @param cipher_text cipher text to be analysed
   * @param ngram_len length of the n-grams to be checked
   *
   * @return key length (0 = no length computed)
   */
  int kasiskiTest(const vector<byte>& cipher_text,
                  int ngram_len=3);

  /**
   * Compute the coincidence index for a given text
   *
   * @details The coincidence index is an indicator to show the difference between an english text or a text
   * with random characters. If the coincidence index is above 0.65, the given text is probably an english text.
   *
   * Procedure to calculate the coincidence index:
   * First step is to count all appearances of letters inside the given text, and save them in an array. The
   * position of the letters inside the array corresponds to the position of the letters in the alphabet for easier
   * working with the data structure. Continue calculating the sum of the given formula from the lectures (page 19).
   * Finally dividing through n * (n-1) where n is the text size, and then returning the coincidence index.
   *
   * @param text text to be analysed
   *
   * @return coincidence index of text
   */
  float coincidenceIndex(const vector<byte>& text);

  /**
   * Compute the coincidence indices of a given number of
   * columns and check whether all values lie above a 
   * given threshold
   *
   * @details At first the cipher text needs to be divided into n (cols) columns. Subsequently iterating through the
   * columns and calculating the coincidence index with the method "coincidenceIndex()" and checking if all of the
   * columns lie above the given threshold. The output coincidence indexes are rounded to 8 decimal places for better
   * visual representation. Also added an indicator for the print statement, if all columns lie above the threshold.
   *
   * @param cipher_text cipher_text to be analysed
   * @param cols number of columns
   * @param threshold threshold value
   *
   * @return true <=> indices of all columns are >= threshold
   */
  bool coincidenceTest(const vector<byte>& cipher_text,
                       int cols, 
                       float threshold=0.065);

  /**
   * Compute the mutual coincidence index table of two columns
   * and return the relative shift g such that key[j] = key[i] - g
   * if the respective index is >= threshold
   *
   * @details The mutual coincidence index is a method for reducing the possible keys for decrypting the given text.
   * With the help of the mutual coincidence index it is possible to bring down the number of possible keys to 26.
   * Therefore a brute force of these 26 keys is very easy to implement and to finally decrypt the given cipher text.
   *
   * Procedure to calculate the possible keys:
   * At first the cipher text needs to be divided into n (cols) columns. Subsequently calculating the
   * occurrences of letters in the columns i and j (col_i, col_j) in the divided cipher text. Finally iterating
   * through the occurrences and calculating the sum of the given formula from the lectures (page 28). The mutual index
   * and the shift g is saved inside a map (map<double, int>) with the first element being the mutual index and the
   * second being the shift. The output mutual indexes are rounded to 3 decimal places. Also added output formatting
   * for easier analysis of the mutual indexes.
   *
   * @param cipher_text cipher_text to be analysed
   * @param cols number of columns
   * @param col_i first column
   * @param col_j second column
   * @param threshold threshold value
   *
   * @return shift g between key[i] and key[j], if respective index 
   * is >= threshold. -1, otherwise.
   */
  int mutualCoinIndex(const vector<byte>& cipher_text,
                      int cols, 
                      int i,
                      int j,
                      float threshold=0.065);

  /**
   * Analyse a given cipher text.
   * @param cipher_text cipher text to be analysed
   */
  void analyse(const vector<byte>& cipher_text);

};

#endif
