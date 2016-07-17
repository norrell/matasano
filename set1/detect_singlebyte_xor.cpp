#include <iostream>
#include <fstream>
#include <cstddef>
#include <list>

#include "typedefs.h"
#include "singlebyte_xor.h"

static bool comp_score(const Plaintext &text1, const Plaintext &text2)
{
    return text1.score < text2.score;
}

std::list<Plaintext> detect_singlebyte_xor(std::ifstream &file) {
    std::list<Plaintext> candidates;

    std::string line;
    std::list<Plaintext> best;
    int lno = 0;
    for (std::string line; std::getline(file, line); ++lno) {
        best = singlebyte_xor(line, line.size(), lno);
        candidates.merge(best, comp_score);
    }
    candidates.reverse(); /* highest score first */

    return candidates;
}