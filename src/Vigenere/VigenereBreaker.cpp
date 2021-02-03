#include <cassert>
#include <map>
#include <iomanip>
#include <set>
#include <string>
#include <vector>
#include "VigenereBreaker.h"


int VigenereBreaker::gcd(int a, int b) const {
    int r;

    do {
        r = a % b;
        a = b;
        b = r;
    } while (r != 0);

    return a;

}


int VigenereBreaker::gcd(const vector<int> &v) const {
    int r, i;

    assert(v.size() > 0);

    if (v.size() == 1) {
        return v[0];
    }
    else {

        r = gcd(v[0], v[1]);
        i = 2;
        while ((r > 1) && (i < v.size())) {
            r = gcd(r, v[i]);
            i++;
        }
        return r;
    }
}


int VigenereBreaker::kasiskiTest
        (
                const vector<byte> &cipher_text,
                int ngram_len
        ) {
    /*************************************************************************
     * Aufgabe 4.
     *************************************************************************/
    //Aufgabe 4 a)
    string ciphertext_str;
    ostringstream oss;

    //Using output stream to write all characters into a buffer and create a string from it
    for(auto it : cipher_text)
        oss << (char) (it + 'A');
    ciphertext_str = oss.str();

    //Aufgabe 4 b)
    map<string, vector<int>> ngram_table;

    //iterate over all positions in ciphertext to find all ngrams
    for(int i = 0; i <= ciphertext_str.length() - ngram_len; i++)  {
        string current_ngram = ciphertext_str.substr(i, ngram_len);

        //Skip current ngram if its already in the table
        if(ngram_table.find(current_ngram) != ngram_table.end())
            continue;

        ngram_table.insert({current_ngram, vector<int>()});

        //Search for each occurrence of current ngram in ciphertext to store its position
        for(int j = 0; j <= ciphertext_str.length() - ngram_len; j++)
            if(ciphertext_str.substr(j, ngram_len) == current_ngram)
                ngram_table[current_ngram].push_back(j);
    }

    //Aufgabe 4 c)
    //Struct containing metadata for kasiski test; ngram used as key
    struct kasiski_meta {
        kasiski_meta()=default;
        kasiski_meta(vector<int> v1) : positions(v1) {  //calculate relative positions
            for (auto pos : positions) rel_positions.push_back(pos - v1.front());
        }
        vector<int> positions;
        vector<int> rel_positions;
        int gcd = 0;
    };

    map<string, kasiski_meta> ngram_valid;  //contains ngrams with 3 or more occurrences
    map<int, int> gcds; //key: gcd, value: number of occurrences

    //Search and store ngrams with 3 or more occurrences
    for(auto current_ngram : ngram_table) {
        if(current_ngram.second.size() > 2) {
            //Copy valid ngrams to map containing all necessary information
            ngram_valid.insert({current_ngram.first, kasiski_meta(current_ngram.second)});
            ngram_valid[current_ngram.first].gcd = gcd(ngram_valid[current_ngram.first].rel_positions);
        }
    }

    //Store each gcd and its number of occurrences
    for(auto ngram : ngram_valid) {
        if(gcds.find(ngram.second.gcd) != gcds.end())
            gcds[ngram.second.gcd]++;
        else gcds.insert({ngram.second.gcd, 1});

        oss.str(string());
        oss.clear();

        //Output current ngram with positions and its gcd
        copy(ngram.second.positions.begin(), ngram.second.positions.end(), ostream_iterator<int>(oss, " "));
        cout << ngram.first << ": " << oss.str() << "-> gcd: " << ngram.second.gcd << endl;
    }

    int gcd_occurrences = 0, gcd_max_value = 0, gcd_2nd_max_value = 0;

    cout << "\nResult:" << endl;
    for(auto gcd : gcds)    {
        //search for the gcd with most occurrences
        if(gcd.second >= gcd_occurrences) {
            gcd_2nd_max_value = gcd_max_value;
            gcd_max_value = gcd.first;
            gcd_occurrences = gcd.second;
        }
        printf("gcd %2d: number of occurrences = %d\n", gcd.first, gcd.second);
    }

    //Check if 2 GCDs appear the same number of times
    if(gcd_max_value > gcd_2nd_max_value)   return gcd_max_value;
    //Only search until n-gram length is 5
    else if(ngram_len + 1 < 6) {
        cout << "\nNo clear result found. Repeat the test with " << ngram_len + 1 << "-gram\n\n" << endl;
        return this->kasiskiTest(cipher_text, ngram_len + 1);
    }

    return 0;
}



float VigenereBreaker::coincidenceIndex(const vector<byte> &text) {
    /*************************************************************************
     * Aufgabe 6.
     *************************************************************************/

    int occurencesOfLetters[26] = {0};

    //Count all occurences of letters in the given text
    //Letter a -> occurencesOfLetters[0]
    //The position of the letter in the array corresponds to the position of the letter in the alphabet
    for(int i = 0; i < text.size(); i++){
        occurencesOfLetters[text[i]]++;
    }

    int upperPartFormula = 0;

    //Upper part of the formula given in the slides from the lecture
    for (int j = 0; j < 26; j++){
        upperPartFormula += occurencesOfLetters[j] * (occurencesOfLetters[j] - 1);
    }

    double result = upperPartFormula / double(text.size() * (text.size() - 1));
    return result;
}


bool VigenereBreaker::coincidenceTest
        (
                const vector<byte> &cipher_text,
                int cols,
                float threshold
        ) {
    /*************************************************************************
     * Aufgabe 7.
     *************************************************************************/

    vector<byte> colOfText[cols];
    bool allGreaterThanThreshold = true;

    //Divide the text into n (cols) columns
    for(int i = 0; i < cipher_text.size(); i++){
        colOfText[i % cols].push_back(cipher_text[i]);
    }

    cout << cols << " columns: ";

    //Calculate the coincidence indexes for every column and print them
    for(int j = 0; j < cols; j++){
        double coinIndex = coincidenceIndex(colOfText[j]);
        cout << fixed << setprecision(8) << coinIndex << " ";
        if(coinIndex < threshold){
            allGreaterThanThreshold = false;
        }
    }

    //Calculate if all of the indexes are greather than the threshhold
    if(allGreaterThanThreshold){
        cout << "<===";
    }
    cout << endl;

    return allGreaterThanThreshold;
}

int VigenereBreaker::mutualCoinIndex
        (
                const vector<byte> &cipher_text,
                int cols,
                int col_i,
                int col_j,
                float threshold
	 ) {
    /*************************************************************************
     * Aufgabe 9.
     *************************************************************************/

    vector<byte> colOfText[cols];

    //Divide the text into n (cols) columns
    for(int i = 0; i < cipher_text.size(); i++){
        colOfText[i % cols].push_back(cipher_text[i]);
    }


    //Calculate the occurences of letters in the text in the column i (col_i)
    int occurencesOfLettersColI[26] = {0};

    //Letter a -> occurencesOfLetters[0]
    //The position of the letter in the array corresponds to the position of the letter in the alphabet
    for(int j = 0; j < colOfText[col_i].size(); j++){
        occurencesOfLettersColI[colOfText[col_i][j]]++;
    }

    //Calculate the occurences of letters in the text in the column j (col_j)
    int occurencesOfLettersColJ[26] = {0};

    for(int j = 0; j < colOfText[col_j].size(); j++){
        occurencesOfLettersColJ[colOfText[col_j][j]]++;
    }

    cout << "  " << col_i << ",  " << col_j << ":";

    //Map first -> MutualIndex
    //Map second -> Shift
    map<double, int> maxMutualIndex;
    int ctr = 0;
    //Iterating through all 26 shifts
    for (int g = 0; g < 26; g++){
        //Calculating the formula from the lecture for computing the mutual index
        int sum = 0; ctr++;
        for (int i = 0; i < 26; i++){
            sum += occurencesOfLettersColI[i] * occurencesOfLettersColJ[(i-g + 26) % 26];
        }
        double mutualIndex = sum / double(colOfText[col_i].size() * colOfText[col_j].size());
        maxMutualIndex.insert(make_pair(mutualIndex, g));

        cout << right << setw(4) << g << ": "  << fixed << setprecision(3) << mutualIndex;
        if(ctr % 7 == 0) cout << "\n        ";

    }

    cout << endl;

    //Getting the last element of the map. last element -> max Mutual Index
    map<double, int>::iterator itr;
    itr = maxMutualIndex.end();
    --itr;

    //Output formating
    if(itr->first >= threshold){
        int shiftWithMaxIndex = 26 - itr->second;
        cout << "    max:" << right << setw(4) << itr->second << ": " << itr->first << " => key[" << col_j << "] = key[" << col_i << "] + " << shiftWithMaxIndex << endl << endl;
        return shiftWithMaxIndex;
    }else{
        cout << "    max: < " << threshold << endl << endl;
        return -1;
    }
}

