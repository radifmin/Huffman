#include "Huffman_Tree.h"
#include <map>
#include <algorithm>
#include <iostream>
#include <stack>

void Alphabet::emplace_back(char c, int w)
{
    its_alphabet.emplace_back(c, w, its_alphabet.size());
}
void Alphabet::initialize_alphabet(const std::string txt)
{
    std::ifstream stream;
    stream.open(txt);
    initialize_alphabet(stream);
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

    // kostuli
    std::vector<std::pair<int, char>> tmp;
    for (auto it = freqs.begin(); it != freqs.end(); it++) {
        tmp.emplace_back(it->second, it->first);
    }
    std::sort(tmp.begin(), tmp.end());
    // push freqs to its_alphabet
    for (int i = tmp.size()-1; i >= 0; i--) {
        emplace_back(tmp[i].second, tmp[i].first);
    }
    //
    //std::cout<<print_alphabet();
    stream.seekg(0, std::ios::beg);
}
std::string Alphabet::print_alphabet() const
{
    std::string result{ "The current alphabet is:\n" };
    for (const auto& symbol : (*this)())
        result += "Symbol: " + std::string{ symbol.c } + "; Weight: " + std::to_string(symbol.w) + "; Node id: " + std::to_string(symbol.id)+";\n";
    return result + '\n';
}

void Huffman_Tree::emplace_back(int left, int right)
{
    tree.emplace_back(left, right, -1);
}
Huffman_Tree::Huffman_Tree(const Alphabet& a)
{
    for (int i = 0; i < a.size(); i++)
        emplace_back(-1, -1);
    //std::cout << print_tree(a);
}
void Huffman_Tree::insert_node(int r1, int r2)
{
    emplace_back(r1, r2);
    set_back_as_parent(r1);
    set_back_as_parent(r2);
}
void Huffman_Tree::set_back_as_parent(int id)
{
    tree[id].set_parent(tree.size() - 1);
}
std::string Huffman_Tree::print_tree(const Alphabet& a) const
{
    std::string res("Tree:\n");

    for (int i = 0; i < tree.size(); i++)
    {
        const auto& t = tree[i];
        res += "Left: " + std::to_string(t.left()) + "; Right: " + std::to_string(t.right()) + "; Parent: " + std::to_string(t.parent());
        if (t.is_leaf())
        {
            res += "; Symbol: ";
            //res += a[i].c;
        }
        res += ";\n";
    }

	return res +'\n';
}

std::string Forest::print_forest(const Alphabet& a) const
{
    std::string res("Forest:\n");

    res += "Sorted: " + std::to_string(is_sorted()) + '\n';
    for (int i = 0; i < forest.size(); i++) {
        const auto& root = forest[i];
        res += "Weight: " + std::to_string(root.w) + "; Node id: " + std::to_string(root.id);
        // if is symbol
        if (root.id < a.size())
        {
            res += "; Symbol: ";
            res += a[root.id].c;
        }
        res += ";\n";
    }

    return res +'\n';
}
bool Forest::is_sorted() const
{
    for (int i = 1; i < forest.size(); i++)
        if (forest[i - 1].w < forest[i].w)
            return false;
    return true;
}
void Forest::insert_sorted(Root&& r)
{
    auto cmp = [](const Root& s1, const Root& s2) -> bool {return s1.w > s2.w; };
    const auto insert_pos(std::lower_bound(forest.begin(), forest.end(), r, cmp));
    forest.insert(insert_pos, std::move(r));
}
Forest::Root Forest::grab_root_back()
{
    Root r{ std::move(forest.back()) };
    forest.pop_back();
    return r;
}
Forest::Forest(const Alphabet& a)
{
    for (int i = 0; i < a.size(); i++)
        insert_sorted(Root{ a[i].w, a[i].id });

    //std::cout<<print_forest(a);
}
Huffman_Tree Forest::build_tree(const Alphabet& a)
{
	Huffman_Tree tree{ a };
    while (forest.size() > 1)
    {
        Root r1{ std::move(grab_root_back()) };
        Root r2{ std::move(grab_root_back()) };
        insert_sorted(std::move(Root{ r1.w + r2.w, tree.size() }));
        tree.insert_node(r1.id, r2.id);
    }
	//std::cout << tree.print_tree(a);
    return tree;
}

void Huf_Coder::encode(std::string filename, std::string to) const
{
    std::ifstream fin;
    fin.open(filename);

    std::ofstream fout;
    fout.open(to);
    
	while (true) {
		char c;
		fin.read(&c, sizeof(char));
        if (fin.eof()) break;
		for (int j = 0; j < alphabet.size(); j++) {
			const auto& symbol = alphabet[j];
			if (c == symbol.c) {
				std::stack<char, std::vector<char>> st;

				int id = symbol.id;
				int p_id = tree[id].parent();
				while (p_id != -1) {
					if (id == tree[p_id].left()) st.push('0');
					else if (id == tree[p_id].right()) st.push('1');
					id = p_id;
					p_id = tree[id].parent();
				}
				while (!st.empty()) {
					fout << (st.top());
					st.pop();
				}
			}
		}
	}

    fout.close();
    fin.close();
}
void Huf_Coder::decode(std::string filename, std::string to) const
{
    std::ifstream fin;
    fin.open(filename);

    std::ofstream fout;
    fout.open(to);

	int ptr = tree.size() - 1;
    while(true) {
        if (tree[ptr].is_leaf()) {
			fout << alphabet[ptr].c;
            ptr = tree.size() - 1;
        }

		char c;
        fin.read(&c, sizeof(char));
        if (fin.eof()) break;
        if (c == '0') {
            ptr = tree[ptr].left();
        }
        else if (c == '1') {
            ptr = tree[ptr].right();
        }
    }

    fout.close();
    fin.close();
}
//std::string Huf_Coder::encode(std::string txt) const
//{
//    std::string result("");
//
//    for (int i = 0; i < txt.size(); i++) {
//        for (int j = 0; j < alphabet.size(); j++) {
//            const auto& symbol = alphabet[j];
//            if (txt[i] == symbol.c) {
//				std::stack<char, std::vector<char>> st;
//
//                int id = symbol.id;
//				int p_id = tree[id].parent();
//                while (p_id != -1) {
//                    if (id == tree[p_id].left()) st.push('0');
//                    else if (id == tree[p_id].right()) st.push('1');
//                    id = p_id;
//                    p_id = tree[id].parent();
//                }
//                while (!st.empty()) {
//                    result += st.top();
//                    st.pop();
//                }
//            }
//        }
//    }
//
//    return result;
//}
//std::string Huf_Coder::decode(std::string encoded) const
//{
//    std::string result("");
//
//	int ptr = tree.size() - 1;
//    for (int i = 0; i < encoded.size(); i++) {
//        if (tree[ptr].is_leaf()) {
//            result += alphabet[ptr].c;
//            ptr = tree.size() - 1;
//        }
//        if (encoded[i] == '0') {
//            ptr = tree[ptr].left();
//        }
//        else if (encoded[i] == '1') {
//            ptr = tree[ptr].right();
//        }
//    }
//    result += alphabet[ptr].c;
//
//    return result;
//}


void test::forest()
{
    std::string in("C:\\in.txt");
    std::string out("C:\\out.txt");
    Alphabet a(in);
    Huf_Coder coder(std::move(a));

    coder.encode(in, out);
    coder.decode(out, "C:\\out_decoded.txt");

    std::fstream in_file(in);
    in_file.seekg(0, std::ios::end);
    int in_size=in_file.tellg()*sizeof(char)*8;
    in_file.close();
    std::cout << "Size of original text: " << std::to_string(in_size) << '\n';

    std::fstream out_file(out);
    out_file.seekg(0, std::ios::end);
    int out_size = out_file.tellg();
    out_file.close();
    std::cout << "Size of encoded text: " << std::to_string(out_size) << '\n';
}