#ifndef AES_ECB_H
#define AES_ECB_H

#include <vector>
#include <string>

void aes_128_decrypt(std::vector<unsigned char> & in, std::vector<unsigned char> & out, const unsigned char *key);
void aes_128_decrypt(std::vector<unsigned char> & in, std::vector<unsigned char> & out, const std::string & key);
void detect_aes128_ecb(const std::string & filename);
void KeyExpansion(const unsigned char *key, unsigned char w[][4], size_t keylen);

#endif /* AES_ECB_H */