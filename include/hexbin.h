#ifndef HEXBIN_H
#define HEXBIN_H

#include <vector>
#include <cstddef> /* size_t*/
#include <string>

//std::vector<byte> hexstring_to_bin(const char *hexstr, size_t str_len_wo_null);
std::string bin2hex(const std::vector<unsigned char> & bin);
std::vector<unsigned char> hex2bin(const std::string & hexstr);

#endif /* HEXBIN_H */