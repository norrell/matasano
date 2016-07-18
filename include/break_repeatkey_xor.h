#ifndef BREAK_REPEATKEY_XOR_H
#define BREAK_REPEATKEY_XOR_H

#include <string>

std::string break_repeatkey_xor(const std::string &filename);
void repeatkey_xor_decrypt_file(const std::string &cipherfile, const std::string &key);

#endif /* BREAK_REPEATKEY_XOR_H */