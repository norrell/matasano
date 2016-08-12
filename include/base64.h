#ifndef BASE64_H
#define BASE64_H

#include <string>
#include <fstream>

std::string b64enc(const std::vector<unsigned char>& v);
std::string b64enc(const std::string& str);
std::string b64enc_hex(const std::string& hexstr);
std::string b64enc(std::ifstream& file);
std::string b64dec(const std::vector<unsigned char>& v);
std::string b64dec(const std::string& str);
void b64dec(const std::ifstream& from, const std::ofstream& to);
void b64dec(const std::string& from, const std::string& to);

#endif /* BASE64_H */