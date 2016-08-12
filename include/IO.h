#ifndef IO_H
#define IO_H

#include <string>
#include <vector>
#include "typedefs.h"

std::vector<unsigned char> from_file(const std::string& filename);
void to_file(const std::vector<unsigned char> & v, const std::string & filename);

#endif /* IO_H */