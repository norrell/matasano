#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <cstddef> /* size_t*/
#include <string>

#include "typedefs.h"

//std::vector<byte> hexstring_to_bin(const char *hexstr, size_t str_len_wo_null);
std::string bin_to_hex(const std::vector<byte> &bin);
std::vector<byte> hex_to_bin(const std::string &hexstr);
int hamming_distance(const std::string &s1, const std::string &s2);
int hamming_distance(const std::vector<byte> &v1, const std::vector<byte> &v2);
std::string b64_encode_text(const std::string &str);
std::string b64_encode(const std::string &hexstr);
std::string b64_encode(std::ifstream &file);
std::string b64_decode(const std::string &str);
void b64_decode_file(const std::string &from, const std::string &to);

#endif /* UTILS_H */