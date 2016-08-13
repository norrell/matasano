#include <iostream>
#include <cstdio>
#include <cstring>

#include "aes128.h"

int main(int argc, char *argv[])
{
#if 0
    /* Key expansion */
    std::cout << "Checking KeyExpansion routine..." << std::endl;
    const unsigned char key[16] = {0x2b, 0x7e, 0x15, 0x16,
                                   0x28, 0xae, 0xd2, 0xa6,
                                   0xab, 0xf7, 0x15, 0x88,
                                   0x09, 0xcf, 0x4f, 0x3c};
    unsigned char w[44][4]; // 44 for AES128
    KeyExpansion(key, w, 16);
#endif
    const unsigned char key[16] = {0x00, 0x01, 0x02, 0x03,
                                   0x04, 0x05, 0x06, 0x07,
                                   0x08, 0x09, 0x0a, 0x0b,
                                   0x0c, 0x0d, 0x0e, 0x0f};

    std::cout << "Checking Cipher routine..." << std::endl;
    const unsigned char plaintext[16] = {0x00, 0x11, 0x22, 0x33,
                                         0x44, 0x55, 0x66, 0x77,
                                         0x88, 0x99, 0xaa, 0xbb,
                                         0xcc, 0xdd, 0xee, 0xff};
    std::vector<unsigned char> p;
    p.assign(plaintext, plaintext + sizeof(plaintext));

    std::vector<unsigned char> cipher(16, 0);
    aes128_ecb_encrypt(p, cipher, key);
    for (int i = 0; i < 16; ++i)
        printf("%.2hhx", cipher[i]);

    printf("\n");

    std::cout << "Checking InvCipher routine..." << std::endl;
    const unsigned char encrypted[16] = {0x69, 0xc4, 0xe0, 0xd8,
                                         0x6a, 0x7b, 0x04, 0x30,
                                         0xd8, 0xcd, 0xb7, 0x80,
                                         0x70, 0xb4, 0xc5, 0x5a};
    std::vector<unsigned char> e;
    e.assign(encrypted, encrypted + sizeof(encrypted));

    std::vector<unsigned char> plain(16, 0);
    aes128_ecb_decrypt(e, plain, key);
    for (int i = 0; i < 16; ++i)
        printf("%.2hhx", plain[i]);

    printf("\n");
    return 0;
}