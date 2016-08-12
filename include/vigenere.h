#ifndef VIGENERE_H
#define VIGENERE_H

#include <string>
#include <fstream>
#include <vector>
#include <cstddef>

namespace vigenere {
    /**
     * Encrypts a character string using the given key.
     * Returns a hexstring representation of the encrypted array.
     */
    std::string encrypt(const std::string & str, const std::string & key);

    /**
     * Encrypts an input file using the given key and writes the encrypted
     * data to the given output file.
     */
    void encrypt(const std::string & in, const std::string & key, const std::string & out);

    /**
     * Finds the key used to encrypt the given file.
     */
    std::string find_key(const std::string & filename);
    std::string find_key(std::vector<unsigned char> & v);

    /**
     * Decrypts the hexstring representation of a byte array using the given key.
     * Returns a character string with the plaintext.
     */
    std::string decrypt(const std::string & hexstr, const std::string & key);

    /**
     * Decrypts an input file using the given key and writes the decrypted
     * data to the given output file.
     */
    void decrypt(const std::string & in, const std::string & key, const std::string & out);

    /**
     * Finds the key used to encrypt the given file, decrypts it and writes the
     * encrypted data to the given output file.
     */
    void break_it(const std::string & in, const std::string & out);
}

#endif /* VIGENERE_H */