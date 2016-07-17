#ifndef SINGLEBYTE_XOR_H
#define SINGLEBYTE_XOR_H

#include <cstring>
#include <list>
#include <vector>
#include <string>

#include "typedefs.h"

std::list<Plaintext> singlebyte_xor(const char *hexstr, size_t str_size_wo_null, int lno = 0);
std::list<Plaintext> singlebyte_xor(std::string hexstr, size_t str_size_wo_null, int lno = 0);

#endif /* SINGLEBYTE_XOR_H */