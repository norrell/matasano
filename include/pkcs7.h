#include <vector>
#include <string>
#include <cstddef>

namespace pkcs7 {
    /**
     * INPUT is the plaintext message
     * OUTPUT is the padded message
     */
    void pad(std::vector<unsigned char> & plaintext, size_t k);
    std::vector<unsigned char> pad(std::string & plaintext, size_t k);
}