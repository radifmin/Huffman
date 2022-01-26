#pragma once
#include <string>
#include <vector>

class Huffman_Node
{
public:
	const unsigned char c;
	const int w;

	Huffman_Node* left{ nullptr };
	Huffman_Node* right{ nullptr };
	Huffman_Node* parent{ nullptr };
	Huffman_Node(unsigned char c, int w) : c{ c }, w{ w } {}


	bool operator<(const Huffman_Node& right) const
	{
		if (w < right.w)
		{
			return true;
		}
		return false;
	}
	static Huffman_Node* build_tree(/*std::vector<Huffman_Node*>, */std::string s);


	std::string encode(const std::string&);
	std::string decode(const std::string&);
private:
	std::string get_code(unsigned char, std::string path);
};


