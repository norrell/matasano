#include <vector>
#include <cstddef>

#include "pkcs7.h"

/**
 * \param k block size
 */
void pkcs7::pad(std::vector<unsigned char> & plaintext, size_t k)
{
    size_t l = plaintext.size();
    plaintext.resize(l + (k - (l % k)), k - (l % k));
}

std::vector<unsigned char> pkcs7::pad(std::string & plaintext, size_t k)
{
    std::vector<unsigned char> p(plaintext.begin(), plaintext.end());
    pkcs7::pad(p, k);
    return p;
}
