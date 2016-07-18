
#include <iostream>
#include <cstring> /* size_t */
#include <climits> /* INT_MIN */
#include <list>
#include <vector>
#include <map>

#include "typedefs.h"
#include "utils.h"
#include "singlebyte_xor.h"

static const std::string EN_LETTERS_BY_FREQ = "etaoinshrdlcumwfgypbvkjxqz";
static const std::string MOST_FREQ = EN_LETTERS_BY_FREQ.substr(0, 6);
static const std::string LEAST_FREQ = EN_LETTERS_BY_FREQ.substr(20);

static bool is_printable(byte b)
{
    if ((b >= ' ' && b <= '~') ||
         b == '\t' ||
         b == '\n' ||
         b == '\r' ||
         b == 0x0b ||
         b == 0x0c)
        return true;
    else
        return false;
}

static bool is_letter(byte b)
{
    if ((b >= 'A' && b <= 'Z') || (b >= 'a' && b <= 'z'))
        return true;
    else
        return false;
}

static bool is_digit(byte b)
{
    if (b >= '0' && b <= '9')
        return true;
    else
        return false;
}

static bool is_common_punctuation(byte b)
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

static void reset_symbol_freqs(std::map<byte, int> &symbol_freq)
{
    symbol_freq['a'] = 0;
    symbol_freq['b'] = 0;
    symbol_freq['c'] = 0;
    symbol_freq['d'] = 0;
    symbol_freq['e'] = 0;
    symbol_freq['f'] = 0;
    symbol_freq['g'] = 0;
    symbol_freq['h'] = 0;
    symbol_freq['i'] = 0;
    symbol_freq['j'] = 0;
    symbol_freq['k'] = 0;
    symbol_freq['l'] = 0;
    symbol_freq['m'] = 0;
    symbol_freq['n'] = 0;
    symbol_freq['o'] = 0;
    symbol_freq['p'] = 0;
    symbol_freq['q'] = 0;
    symbol_freq['r'] = 0;
    symbol_freq['s'] = 0;
    symbol_freq['t'] = 0;
    symbol_freq['u'] = 0;
    symbol_freq['v'] = 0;
    symbol_freq['w'] = 0;
    symbol_freq['x'] = 0;
    symbol_freq['y'] = 0;
    symbol_freq['z'] = 0;
    symbol_freq['0'] = 0; /* digits */
    symbol_freq['.'] = 0; /* common punctuation marks in text */
    symbol_freq['*'] = 0; /* uncommon punctuation marks in text */
    symbol_freq['?'] = 0; /* unprintable and/or non-ASCII */
}

static byte to_lower(byte b)
{
    return (b < 'a') ? b + 32 : b;
}

static void update_symbol_freq(std::map<byte, int> &symbol_freq, byte b)
{
    if (is_printable(b)) {
        if (is_letter(b)) {
            symbol_freq[to_lower(b)]++;
        } else if (is_digit(b)) {
            symbol_freq['0']++;
        } else if (is_common_punctuation(b)) {
            symbol_freq['.']++;
        } else {
            symbol_freq['*']++;
            /*
            * Current implementiation: strings containing uncommon
            * punctuation marks such as '{' or '#' are to be discarded.
            */
            return;
        }
    } else {
        symbol_freq['?']++;
        /*
         * Current implementiation: strings with unprintable and/or
         * non-ASCII characters are to be discarded, hence return
         * as soon as one such character has been found.
         */
        return;
    }
}

static std::list<std::pair<char, int>> map_to_list(const std::map<byte, int> &symbol_freq)
{
    std::list<std::pair<char, int>> list;
    for (auto it = symbol_freq.begin(); it != symbol_freq.end(); ++it) {
        if (is_letter(it->first)) {
            std::pair<char, int> pair = std::make_pair(it->first, it->second);
            list.push_back(pair);
        }
    }

    return list;
}

static bool comp_letter_freq(const std::pair<char, int> &pair1, std::pair<char, int> &pair2)
{
    return pair1.second < pair2.second;
}

static std::string sort_letters_by_freq(const std::map<byte, int> &symbol_freq)
{
    std::list<std::pair<char, int>> list = map_to_list(symbol_freq);
    list.sort(comp_letter_freq);

    std::string s = "";
    for (auto p : list) {
        s += p.first;
    }

    return s;
}

static int evaluate_letter_freq(const std::map<byte, int> &map)
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
    for (int i = 0; i < 6; ++i) {
        std::size_t found = MOST_FREQ.find(letter_freq[i]);
        if (found != std::string::npos)
            score += 10;

        //found = LEAST_FREQ.find(letter_freq[i+ 20]);
        //if (found != std::string::npos)
        //    score += 10;
    }

    return score;
}

static int get_vowel_amount(std::map<byte, int> &symbol_freq)
{
    return (symbol_freq['a'] +
            symbol_freq['e'] +
            symbol_freq['i'] +
            symbol_freq['o'] +
            symbol_freq['u']);
}

static int get_consonant_amount(std::map<byte, int> &symbol_freq)
{
    return (symbol_freq['b'] +
            symbol_freq['c'] +
            symbol_freq['d'] +
            symbol_freq['f'] +
            symbol_freq['g'] +
            symbol_freq['h'] +
            symbol_freq['j'] +
            symbol_freq['k'] +
            symbol_freq['l'] +
            symbol_freq['m'] +
            symbol_freq['n'] +
            symbol_freq['p'] +
            symbol_freq['q'] +
            symbol_freq['r'] +
            symbol_freq['s'] +
            symbol_freq['t'] +
            symbol_freq['v'] +
            symbol_freq['w'] +
            symbol_freq['x'] +
            symbol_freq['y'] +
            symbol_freq['z']);
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
static int calc_score(std::map<byte, int> &symbol_freq, size_t bufsize)
{
    int score = 0;

    /*
     * Discard strings containing unprintable and/or non-ASCII
     * characters and/or unusual punctuation.
     */
    if (symbol_freq['?'] || symbol_freq['*'])
        return INT_MIN;

    /*
     * Penalize high percentage of digits
     */
    if ((double) symbol_freq['0'] / (double) bufsize > 0.4) {
        score -= 10;
    }

    /*
     * Penalize high percentage of punctuation (including white space)
     */
    if ((double) symbol_freq['.'] / (double) bufsize > 0.3) {
        score -= 30;
    }

    /*
     * Penalize too low/high percentage of vowels vs consonants.
     * The ratio of consonants to vowels in English is
     * 62.54 % vs 37.46 % (Hartsuiker, Phonological Encoding and
     * Monitoring in Normal and Pathological Speech, p. 123)
     */
    int letters_amount = get_vowel_amount(symbol_freq) +
                         get_consonant_amount(symbol_freq);
    double vowels_prcnt = (double) get_vowel_amount(symbol_freq) / (double) letters_amount;
    if (vowels_prcnt < 0.35 || vowels_prcnt > 0.42)
        score -= 40;

    /*
     * Favour matching most/least frequest letters
     */
    score += evaluate_letter_freq(symbol_freq);

    return score;
}

static bool comp_score(const Plaintext &text1, const Plaintext &text2)
{
    return text1.score < text2.score;
}

/**
 * Receives a vector of encrypted bytes.
 * Returns a list of the plaintext candidates with the highest scores.
 */
static std::list<Plaintext> singlebyte_xor(const std::vector<byte> &cipher, int lno)
{
    std::list<Plaintext> best;
    size_t size = cipher.size();
    std::map<byte, int> symbol_freq;
    std::vector<byte> plain(size, (byte) 0);
    int maxscore = INT_MIN;

    for (unsigned int key = 0; key < 256; ++key) {
        reset_symbol_freqs(symbol_freq);

        for(unsigned int i = 0; i < size; ++i) {
            plain[i] = cipher[i] ^ (byte) key;
            update_symbol_freq(symbol_freq, plain[i]);
        }

        int score = calc_score(symbol_freq, size);

        if (score > INT_MIN && score >= maxscore) {
            maxscore = score;
            Plaintext t = {score, (int) key, lno, std::string(plain.begin(), plain.end())};
            best.push_back(t);
        }
    }

    best.sort(comp_score);
    best.reverse(); /* highest score at the beginning */

    return best;
}

#if 0
std::list<Plaintext> singlebyte_xor(const char *cipherhex, size_t strlen_nonull, int lno)
{
    std::vector<byte> cipher = hexstring_to_bin(cipherhex, strlen_nonull);
    size_t size = cipher.size();

    return singlebyte_xor(cipher, size, lno);
}
#endif

std::list<Plaintext> singlebyte_xor(const std::string &hexstr, int lno)
{
    std::vector<byte> cipher = hex_to_bin(hexstr);
    return singlebyte_xor(cipher, lno);
}
