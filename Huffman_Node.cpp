#include "Huf_Node.h"
#include <iostream>
#include <queue>
#include <map>
#include <unordered_map>

struct Lowest_priority
{
    bool operator()(const Huffman_Node* left, const Huffman_Node* right) const
    {
        //// если left больше, то меняем
        return left->w > right->w;

        // 1. если вставляется промежут. узел - меняем
        // 2. если вставляем обычный узел, слева что?
        //     промежуточный - не меняем
        //     обычный       - меняем, если 
        //return (right->c == 255 ? (true): (left->c==255?(false):(left->w>right->w) ) );\
        
    }
};

Huffman_Node* Huffman_Node::build_tree(/*std::vector<Huffman_Node*> freqs, */std::string s)
{
#pragma region TXT
    // count freq
    std::map<char, int> freq;

    for (int i = 0; i < s.size(); i++) {
        auto it = freq.emplace(s[i], 1);
        
        if (!it.second) {
            it.first->second++;
        }
    }

    std::priority_queue <Huffman_Node*, std::vector<Huffman_Node*>, Lowest_priority> queue;

    // push freqs to queue
    for (auto it = freq.begin(); it!=freq.end(); it++){
        queue.push(new Huffman_Node(it->first, it->second));
    }
    //
#pragma endregion

    while (queue.size() > 1){
        Huffman_Node* x = queue.top();
        queue.pop();

        Huffman_Node* y = queue.top();
        queue.pop();

		Huffman_Node* z = new Huffman_Node(-1, x->w + y->w);
        z->left = x;
        z->right = y;
        x->parent = z;
        y->parent = z;
        queue.push(z);
    }
    return queue.top();
}

std::string Huffman_Node::get_code(unsigned char ch, std::string path)
{
    if (c == ch) {
        return path;
    }
    if (left) {
        std::string p = left->get_code(ch, path + '0');
        if (!p.empty()) {
            return p;
        }
    }
    if (right) {
        std::string p = right->get_code(ch, path + '1');
        if (!p.empty()) {
            return p;
        }
    }
    return "";
}
std::string Huffman_Node::encode(const std::string& txt)
{
    std::string res("");
    
    for (int i = 0; i < txt.size(); i++) {
        res += get_code(txt[i], "");
    }

    return res;
}

std::string Huffman_Node::decode(const std::string& encoded)
{
    std::string res("");

    Huffman_Node* ptr = this;

    for (int i = 0; i < encoded.size(); i++) {
        if (encoded[i] == '0') {
            if (ptr->left->c == 255) ptr = ptr->left;
            else res += ptr->left->c, ptr=this;
        }
        if (encoded[i] == '1') {
            if (ptr->right->c == 255) ptr = ptr->right;
            else res += ptr->right->c, ptr=this;
        }
    }

    return res;
}