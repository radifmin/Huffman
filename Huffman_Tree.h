#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <array>
#include <algorithm>

class Alphabet
{
	class Symbol
	{
	public:
		char c;
		int w;
		int id;

		Symbol(char c, int w, int id) :c{ c }, w{ w }, id(id){}
		Symbol(const Symbol& other) :c{ other.c }, w{ other.w }, id(other.id){}
		bool operator<(const Symbol& other) const { return w < other.w; }
		bool operator>(const Symbol& other) const { return w > other.w; }
		//Symbol& operator=(const Symbol& other) { *const_cast<unsigned char*>(&c) = other.c; *const_cast<int*>(&w) = other.w; return *this; }
	};

	std::vector<Symbol> its_alphabet;
	const std::vector<Symbol>& operator()() const { return its_alphabet; }
	//void sort();

	void emplace_back(char c, int w);
	void initialize_alphabet(const std::string txt);
	void initialize_alphabet(std::ifstream& stream);
	std::string print_alphabet() const; // for testing

public:
	Alphabet(const std::string& txt) { initialize_alphabet(txt); /*sort();*/ }
	//Alphabet(std::ifstream& stream) { initialize_alphabet(stream); /*sort();*/ }
	const Symbol& operator[](int i) const { return its_alphabet[i]; }
	int size() const { return its_alphabet.size(); }
};

class Huffman_Tree
{
	class Huffman_Node
	{
	private:
		std::array<int, 3> node{ -1,-1,-1 }; // left, right, parent

	public:
		int left() const { return node[0]; }
		int right() const { return node[1]; }
		int parent() const { return node[2]; }
		void set_left(int i) { node[0] = i; if (i < -1) throw std::exception("Huffman_Node::invalid node id."); }
		void set_right(int i) { node[1] = i; if (i < -1) throw std::exception("Huffman_Node::invalid node id."); }
		void set_parent(int i) { node[2] = i; if (i < -1) throw std::exception("Huffman_Node::invalid node id."); }

		Huffman_Node(int left = -1, int right = -1, int parent = -1) :node{ left, right, parent } {}
		bool is_leaf() const { return left() + right() < -1; } // if is_leaf --> Symbol found
	};

	std::vector<Huffman_Node> tree;
	void emplace_back(int left, int right);
	void set_back_as_parent(int id);

public:
	Huffman_Tree(const Alphabet& a);
	int size() const { return tree.size(); };
	Huffman_Node operator[](const int i) const { return tree[i]; }
	void insert_node(int l, int r);

	std::string print_tree(const Alphabet&) const; // for testing
};

class Forest
{
	struct Root {
		int w;
		int id;
		Root(int w_, int id_) : w{ w_ }, id{ id_ }{}
		Root(const Root& r1, const Root& r2, int id) :w{ r1.w + r2.w }, id{ id }{}
		Root& operator=(const Root& other)
		{
			*const_cast<int*> (&w) = other.w;
			*const_cast<int*> (&id) = other.id;
			return *this;
		}
	};

	std::vector<Root> forest;

	bool is_sorted() const;
	void insert_sorted(Root&&);
	Root grab_root_back();
	std::string print_forest(const Alphabet& a) const; // for testing

public:
	Forest(const Alphabet&);
	Huffman_Tree build_tree(const Alphabet&);
};

class Huf_Coder
{
public:
	Huf_Coder(Alphabet&& a) :alphabet{ std::move(a) }, tree { std::move( Forest(alphabet).build_tree(alphabet) ) }{}
	void encode(std::string filename, std::string to) const;
	void decode(std::string filename, std::string to) const;

private:
	Alphabet alphabet;
	Huffman_Tree tree;
};


struct test
{
	static void forest();
};