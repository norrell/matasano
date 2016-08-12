#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <cstddef>
#include <cstdlib>
#include <cstring> /* size_t */
#include <climits> /* INT_MIN */

#include "hexbin.h"
#include "IO.h"
#include "vigenere.h"

static const std::string EN_LETTERS_BY_FREQ = "etaoinshrdlcumwfgypbvkjxqz";
static const std::string MOST_FREQ = EN_LETTERS_BY_FREQ.substr(0, 6);
static const std::string LEAST_FREQ = EN_LETTERS_BY_FREQ.substr(20);

/*****************************/
/* Score plaintext candidate */
/*    (Set 1, challenge 3)   */
/*****************************/

static bool is_printable(unsigned char b)
{
    return ((b >= ' ' && b <= '~') ||
             b == '\t' ||
             b == '\n' ||
             b == '\r' ||
             b == 0x0b ||
             b == 0x0c);
}

static bool is_letter(unsigned char b)
{
    return ((b >= 'A' && b <= 'Z') || (b >= 'a' && b <= 'z'));
}

static bool is_digit(unsigned char b)
{
    return (b >= '0' && b <= '9');
}

static bool is_common_punctuation(unsigned char b)
{
    switch (b) {
    case ' ':
    case '!':
    case '\"':
    case '\'':
    case '(':
    case ')':
    case ',':
    case '-':
    case '.':
    case ':':
    case ';':
    case '?':
    //case '`': /* not used in English */
    case '\t':
    case '\n':
    case '\r':
        return true;
    default:
        return false;
    }
}

static void reset_sym_freqs(std::map<unsigned char, int>& sym_freq)
{
    sym_freq['a'] = 0;
    sym_freq['b'] = 0;
    sym_freq['c'] = 0;
    sym_freq['d'] = 0;
    sym_freq['e'] = 0;
    sym_freq['f'] = 0;
    sym_freq['g'] = 0;
    sym_freq['h'] = 0;
    sym_freq['i'] = 0;
    sym_freq['j'] = 0;
    sym_freq['k'] = 0;
    sym_freq['l'] = 0;
    sym_freq['m'] = 0;
    sym_freq['n'] = 0;
    sym_freq['o'] = 0;
    sym_freq['p'] = 0;
    sym_freq['q'] = 0;
    sym_freq['r'] = 0;
    sym_freq['s'] = 0;
    sym_freq['t'] = 0;
    sym_freq['u'] = 0;
    sym_freq['v'] = 0;
    sym_freq['w'] = 0;
    sym_freq['x'] = 0;
    sym_freq['y'] = 0;
    sym_freq['z'] = 0;
    sym_freq['0'] = 0; /* digits */
    sym_freq['.'] = 0; /* common punctuation marks in text */
    sym_freq['*'] = 0; /* uncommon punctuation marks in text */
    sym_freq['?'] = 0; /* unprintable and/or non-ASCII */
}

static unsigned char to_lower(unsigned char b)
{
    return (b < 'a') ? b + 32 : b;
}

static void update_sym_freq(std::map<unsigned char, int> & sym_freq, unsigned char b)
{
    if (is_printable(b)) {
        if (is_letter(b)) {
            sym_freq[to_lower(b)]++;
        } else if (is_digit(b)) {
            sym_freq['0']++;
        } else if (is_common_punctuation(b)) {
            sym_freq['.']++;
        } else {
            sym_freq['*']++;
        }
    } else {
        sym_freq['?']++;
    }
}

static std::list<std::pair<char, int>> map2list(const std::map<unsigned char, int> & sym_freq)
{
    std::list<std::pair<char, int>> list;
    for (auto it = sym_freq.begin(); it != sym_freq.end(); ++it) {
        if (is_letter(it->first)) {
            std::pair<char, int> pair = std::make_pair(it->first, it->second);
            list.push_back(pair);
        }
    }

    return list;
}

static bool comp_letter_freq(const std::pair<char, int> & p1, std::pair<char, int> & p2)
{
    return p1.second < p2.second;
}

static std::string sort_letters_by_freq(const std::map<unsigned char, int> & sym_freq)
{
    std::list<std::pair<char, int>> list = map2list(sym_freq);
    list.sort(comp_letter_freq);

    std::string s = "";
    for (auto p : list)
        s += p.first;

    return s;
}

static int evaluate_letter_freq(const std::map<unsigned char, int> & map)
{
    /*
     * Current implementation is not entirely correct.
     * Letters with the same frequency are placed in subsequent
     * positions even though they should be in the "same" position.
     * Hence their position in the string doesn't really say
     * much about the actual relative frequencies.
     */
    int score = 0;

    std::string letter_freq = sort_letters_by_freq(map);

    /*
     * Check for matches only within the 6 most frequent letters
     */
    for (int i = 0; i < MOST_FREQ.size(); ++i) {
        std::size_t found = MOST_FREQ.find(letter_freq[i]);
        if (found != std::string::npos)
            score += 10;

        //found = LEAST_FREQ.find(letter_freq[i+ 20]);
        //if (found != std::string::npos)
        //    score += 10;
    }

    return score;
}

static int get_vowel_amount(std::map<unsigned char, int> & sym_freq)
{
    return (sym_freq['a'] +
            sym_freq['e'] +
            sym_freq['i'] +
            sym_freq['o'] +
            sym_freq['u']);
}

static int get_consonant_amount(std::map<unsigned char, int> & sym_freq)
{
    return (sym_freq['b'] +
            sym_freq['c'] +
            sym_freq['d'] +
            sym_freq['f'] +
            sym_freq['g'] +
            sym_freq['h'] +
            sym_freq['j'] +
            sym_freq['k'] +
            sym_freq['l'] +
            sym_freq['m'] +
            sym_freq['n'] +
            sym_freq['p'] +
            sym_freq['q'] +
            sym_freq['r'] +
            sym_freq['s'] +
            sym_freq['t'] +
            sym_freq['v'] +
            sym_freq['w'] +
            sym_freq['x'] +
            sym_freq['y'] +
            sym_freq['z']);
}

/**
 * 1. If a string contains at least one unprintable character, discard it.
 * 2. If a string contains an unusual punctuation mark such as '#', discard it.
 * 3. If a string contains too much punctuation, lower the score.
 * 4. If a string contains too many digits, lower the score.
 * 5. If a string contains too high/low a percentage of vowels, lower the score.
 * 6. For each of the first 6 most/least frequent letters in string matching the six
 *    most/least frequent English letters, increase the score.
 * N.B.: the current score increments/decrements are quite arbitrary.
 */
static int calc_score(std::map<unsigned char, int> & sym_freq, size_t bufsize)
{
    int score = 0;

    /*
     * Discard strings containing unprintable and/or non-ASCII
     * characters and/or unusual punctuation.
     */
    if (sym_freq['?'] || sym_freq['*'])
        return INT_MIN;

    /*
     * Penalize high percentage of digits
     */
    if ((double) sym_freq['0'] / (double) bufsize > 0.4) {
        score -= 10;
    }

    /*
     * Penalize high percentage of punctuation (including white space)
     */
    if ((double) sym_freq['.'] / (double) bufsize > 0.3) {
        score -= 30;
    }

    /*
     * Penalize too low/high percentage of vowels vs consonants.
     * The ratio of consonants to vowels in English is
     * 62.54 % vs 37.46 % (Hartsuiker, Phonological Encoding and
     * Monitoring in Normal and Pathological Speech, p. 123)
     */
    int letters_amount = get_vowel_amount(sym_freq) +
                         get_consonant_amount(sym_freq);
    double vowels_prcnt = (double) get_vowel_amount(sym_freq) / (double) letters_amount;
    if (vowels_prcnt < 0.35 || vowels_prcnt > 0.42)
        score -= 40;

    score += evaluate_letter_freq(sym_freq);

    return score;
}

#if 0
static bool comp_score(const Plaintext &text1, const Plaintext &text2)
{
    return text1.score < text2.score;
}
#endif

/**
 * Receives a vector of encrypted bytes.
 * Returns the key with the best score.
 */
static unsigned char singlebyte_xor(const std::vector<unsigned char> & cipher, int lno = 0) // TODO
{
    size_t size = cipher.size();
    std::map<unsigned char, int> sym_freq;
    std::vector<unsigned char> plain(size, (unsigned char) 0);
    int maxscore = INT_MIN;
    unsigned char bestkey = 0;

    for (unsigned int key = 0; key < 256; ++key) {
        reset_sym_freqs(sym_freq);

        for(unsigned int i = 0; i < size; ++i) {
            plain[i] = cipher[i] ^ (unsigned char) key;
            update_sym_freq(sym_freq, plain[i]);
        }

        int score = calc_score(sym_freq, size);

        if (score > INT_MIN && score > maxscore) {
            maxscore = score;
            bestkey = (unsigned char) key;
        } else if (score > INT_MIN && score == maxscore) {
            std::cout << "Keys with same score!" << std::endl;
        }
    }

    return bestkey;
}

#if 0
//std::list<Plaintext> singlebyte_xor(const std::string &hexstr, int lno)
unsigned char singlebyte_xor(const std::string& hexstr, int lno)
{
    std::vector<unsigned char> cipher = hex2bin(hexstr);
    return singlebyte_xor(cipher, lno);
}
#endif

/************************/
/*    Break vigenere    */
/* (Set 1, challenge 6) */
/************************/

/**
 * Uses algorithm by Wegner (1960)
 */
static int calc_hamming_dist(unsigned char c1, unsigned char c2)
{
    int dist = 0;
    unsigned char xord = c1 ^ c2;
    while (xord != 0) {
        dist++;
        xord &= xord - 1;
    }

    return dist;
}

#if 0
static int hamming_dist(const std::string & s1,
                        const std::vector<unsigned char> & s2)
{
    if (s1.size() != s2.size()) {
        std::cerr << "Strings must be of equal length" << std::endl;
        return -1;
    }

    int dist = 0;
    for (int i = 0; i < s1.size(); ++i) {
        dist += calc_hamming_dist((unsigned char) s1[i], (unsigned char) s2[i]);
    }

    return dist;
}
#endif

static int hamming_dist(const std::vector<unsigned char> & v1,
                        const std::vector<unsigned char> & v2)
{
    if (v1.size() != v2.size()) {
        std::cerr << "Arrays must be of equal length" << std::endl;
        return -1;
    }

    int dist = 0;
    for (int i = 0; i < v1.size(); ++i) {
        dist += calc_hamming_dist(v1[i], v2[i]);
    }

    return dist;
}

static size_t find_keylen(std::vector<unsigned char> & v)
{
    double hmin = (double) INT_MAX;
    size_t keylenmin = 2;
    size_t keylenmax = 40;
    size_t bestlen;

    std::list<std::pair<size_t, double>> minkeylens;

    std::vector<unsigned char>::iterator it = v.begin();
    for (size_t try_keylen = keylenmin; try_keylen <= keylenmax; ++try_keylen) {
        std::vector<unsigned char> a(it, it + try_keylen);
        std::vector<unsigned char> b(it + try_keylen, it + 2 * try_keylen);
        std::vector<unsigned char> c(it + 2 * try_keylen, it + 3 * try_keylen);
        std::vector<unsigned char> d(it + 3 * try_keylen, it + 4 * try_keylen);

        double hnorm1 = ((double) hamming_dist(a, b) / (double) try_keylen);
        double hnorm2 = ((double) hamming_dist(b, c) / (double) try_keylen);
        double hnorm3 = ((double) hamming_dist(c, d) / (double) try_keylen);
        double hnorm4 = ((double) hamming_dist(a, c) / (double) try_keylen);
        double hnorm5 = ((double) hamming_dist(b, d) / (double) try_keylen);
        double hnorm6 = ((double) hamming_dist(a, d) / (double) try_keylen);

        double hnorm = (hnorm1 + hnorm2 + hnorm3 + hnorm4 + hnorm5 + hnorm6) / 6.0;

        if (hnorm < hmin) {
            hmin = hnorm;
            bestlen = try_keylen;
        }
    }

    return bestlen;
}

std::string vigenere::find_key(std::vector<unsigned char> & v)
{
    if (v.empty())
        return "";

    size_t keylen = find_keylen(v);
    size_t whole_blocks = v.size() / keylen;
    size_t remaining_bytes = v.size() % keylen;

    std::string keystr(keylen, '?');

    for (int ik = 0; ik < keylen; ++ik) {
        /*
         * 'block' should have room for all the necessary bytes until
         * the end of the file. If the file size is not divisible
         * by 'keylen', the blocks for the first 'remaining_bytes'
         * of the key need one extra byte, otherwise not.
         */
        std::vector<unsigned char> block(whole_blocks + 1, 0);
        if (!(ik < remaining_bytes))
            block.resize(whole_blocks);

        for (int ib = 0, ic = ik;
             ib < block.size() && ic < v.size();
             ib++, ic += keylen)
            block[ib] = v[ic];

        unsigned char key = singlebyte_xor(block);
        keystr[(int) ik] = (char) key;
    }

    std::cout << "Found key '" << keystr << "'" << std::endl;

    return keystr;
}

std::string vigenere::find_key(const std::string & filename)
{
    std::vector<unsigned char> contents = from_file(filename);
    if (contents.empty()) {
        std::cout << "No data read from file" << std::endl;
        return "";
    }

    std::string keystr = vigenere::find_key(contents);

    return keystr;
}

void vigenere::break_it(const std::string & in, const std::string & out)
{
    std::string key = vigenere::find_key(in);
    /*
     * ! as it is, the file is read into memory TWICE !
     */
    vigenere::decrypt(in, key, out);
}

/**********************************************/
/* Simple encryption and decryption functions */
/*            (Set 1, challenge 5)            */
/**********************************************/

static void repeatkey_xor(std::vector<unsigned char> & v, // in-place
                          const std::vector<unsigned char> & key)

{
    for (int i = 0, j = 0; i < v.size(); ++i, j = i % key.size())
        v[i] ^= key[j];
}

/**
 * Encrypt a character string with Vigenere.
 * \return hexstring representation of the encrypted plaintext, ready
 *         for printing to file.
 */
std::string vigenere::encrypt(const std::string & str,
                              const std::string & key)
{
    std::vector<unsigned char> v(str.begin(), str.end());
    std::vector<unsigned char> k(key.begin(), key.end());

    repeatkey_xor(v, k);
    return bin2hex(v);
}

/**
 * Encrypt a file with Vigenere and write to file.
 * TODO what should be written to file? raw bytes or hex string?
 */
void vigenere::encrypt(const std::string & in,
                       const std::string & key,
                       const std::string & out)
{
    std::vector<unsigned char> p = from_file(in);
    if (p.empty()) {
        std::cerr << "No data read from file" << std::endl;
        return;
    }

    std::vector<unsigned char> k(key.begin(), key.end());
    repeatkey_xor(p, k);
    to_file(p, out);
}

/**
 * Decrypt a hexstring with the given keylen and produce the
 * corresponding character string.
 */
std::string vigenere::decrypt(const std::string & hexstr,
                              const std::string & key)
{
    std::vector<unsigned char> c = hex2bin(hexstr);
    std::vector<unsigned char> k(key.begin(), key.end());
    repeatkey_xor(c, k);
    return std::string(c.begin(), c.end());
}

/**
 * Decrypt a file with the given key and write the result to another file.
 */
void vigenere::decrypt(const std::string & in,
                       const std::string & key,
                       const std::string & out)
{
    std::vector<unsigned char> c = from_file(in);
    if (c.empty()) {
        std::cerr << "No data read from file" << std::endl;
        return;
    }

    std::vector<unsigned char> k(key.begin(), key.end());
    repeatkey_xor(c, k);
    to_file(c, out);
}


/************************/
/*   Detect Vigenere    */
/* (Set 1, challenge 4) */
/************************/

#if 0
static bool comp_score(const Plaintext &text1, const Plaintext &text2)
{
    return text1.score < text2.score;
}

std::list<Plaintext> detect_singlebyte_xor(std::ifstream &file) {
    std::list<Plaintext> candidates;

    std::string line;
    std::list<Plaintext> best;
    int lno = 0;
    for (std::string line; std::getline(file, line); ++lno) {
        best = singlebyte_xor(line, lno);
        candidates.merge(best, comp_score);
    }
    candidates.reverse(); /* highest score first */

    return candidates;
}
#endif