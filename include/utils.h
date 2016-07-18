#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <cstddef> /* size_t*/
#include <string>

#include "typedefs.h"

std::vector<byte> hexstring_to_bin(const char *hexstr, size_t str_len_wo_null);
std::vector<byte> hexstring_to_bin(const std::string &hexstr, size_t str_len_wo_null);
int hamming_distance(const std::string &s1, const std::string &s2);
std::string b64_encode(const std::string &str);
std::string b64_encode(std::ifstream &file);
std::string b64_decode(const std::string &str);

#endif /* UTILS_H */