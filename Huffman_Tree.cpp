#include "Huffman_Tree.h"
#include <map>

void Alphabet::initialize_alphabet(const std::string& txt)
{
    // count freq
    std::map<char, int> freqs;

    for (int i = 0; i < txt.size(); i++) {
        auto it = freqs.emplace(txt[i], 1);

        if (!it.second) {
            it.first->second++;
        }
    }
    //
    // push freqs to its_alphabet
    for (auto it = freqs.begin(); it != freqs.end(); it++) {
        its_alphabet.emplace_back(it->first, it->second);
    }
    //
}
void Alphabet::initialize_alphabet(std::ifstream& stream)
{
    std::map<char, int> freqs;
    if (!stream.is_open()) return;
    
    while (!stream.eof()) {
		char c;
        stream.read(&c, sizeof(unsigned char));
        auto [it, b] = freqs.emplace(c, 1);

        if (!b) {
            it->second++;
        }
    }

    for (auto it = freqs.begin(); it != freqs.end(); it++) {
        its_alphabet.emplace_back(it->first, it->second);
    }
}

void Alphabet::sort()
{
    auto cmp = [](const Symbol& l, const Symbol& r) ->bool {
        return l.w < r.w;
    };
    std::sort(its_alphabet.begin(), its_alphabet.end(), cmp);
}


void test::initialize_alphabet_txt()
{
    Alphabet a("para papa pam");

    std::ifstream str;
    str.open("C:\\in.txt");
    Alphabet b(str);
}