#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <iterator>

std::vector<unsigned char> from_file(const std::string & filename) {
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "Could not open file " << filename << std::endl;
            return {}; // list initialization instead of std::vector<unsigned char>();
    }

    /*
     * The istreambuf_iterator method is slower than more direct methods.
     * In this case I've preferred it for its simplicity.
     */
    std::istreambuf_iterator<char> start(in), end;
    std::vector<unsigned char> v(start, end);
    in.close();

    return v;

#if 0
    in.seekg(0, std::ios::end);
    filelen = in.tellg();
    unsigned char *buf = (unsigned char *) calloc(filelen, 1);
    in.seekg(0, std::ios::beg);
    in.read((char *) buf, filelen);

    return buf;
#endif
}

void to_file(const std::vector<unsigned char> & v, const std::string & filename) {
    std::ofstream out(filename, std::ios::out | std::ios::trunc | std::ios::binary);
    if (!out.is_open()) {
        std::cerr << "Could not open file " << filename << std::endl;
        return;
    }

    out.write((char *) &v[0], v.size());
    out.close();
}