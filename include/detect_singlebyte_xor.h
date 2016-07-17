#ifndef DETECT_SINGLEBYTE_XOR_H
#define DETECT_SINGLEBYTE_XOR_H

#include <typedefs.h>
#include <fstream>
#include <cstddef>
#include <list>

std::list<Plaintext> detect_singlebyte_xor(std::ifstream &file);

#endif /* DETECT_SINGLEBYTE_XOR_H */