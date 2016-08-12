#ifndef HEXBIN_H
#define HEXBIN_H

#include <vector>
#include <cstddef> /* size_t*/
#include <string>

#include "typedefs.h"

//std::vector<byte> hexstring_to_bin(const char *hexstr, size_t str_len_wo_null);
std::string bin2hex(const std::vector<byte> &bin);
std::vector<byte> hex2bin(const std::string &hexstr);
int hamming_distance(const std::string &s1, const std::string &s2);
int hamming_distance(const std::vector<byte> &v1, const std::vector<byte> &v2);

#endif /* HEXBIN_H */