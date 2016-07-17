#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <cstddef> /* size_t*/
#include <string>

#include "typedefs.h"

std::vector<byte> hexstring_to_bin(const char *hexstr, size_t str_len_wo_null);
std::vector<byte> hexstring_to_bin(const std::string &hexstr, size_t str_len_wo_null);
int hamming_distance(const std::string &s1, const std::string &s2);

#endif /* UTILS_H */