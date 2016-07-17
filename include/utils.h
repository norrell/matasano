#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <cstddef> /* size_t*/
#include <string>

#include "typedefs.h"

std::vector<byte> hexstring_to_bin(const char *hexstr, size_t str_len_wo_null);
std::vector<byte> hexstring_to_bin(std::string hexstr, size_t str_len_wo_null);

#endif /* UTILS_H */