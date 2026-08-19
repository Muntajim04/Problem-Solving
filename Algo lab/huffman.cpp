#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <memory>

using namespace std;

struct Node {
    char ch;
    shared_ptr<Node> left, right;
    Node(char c = '\0', shared_ptr<Node> l = nullptr, shared_ptr<Node> r = nullptr)
        : ch(c), left(l), right(r) {}
};

// Type alias for priority queue elements: pair<frequency, NodePtr>
using PQElement = pair<int, shared_ptr<Node>>;

// Step 1: Traverse the tree to generate prefix codes
void getCodes(const shared_ptr<Node>& root, const string& code, unordered_map<char, string>& codes) {
    if (!root) return;
    if (!root->left && !root->right) {
        codes[root->ch] = code.empty() ? "0" : code; // Fallback for single unique character
        return;
    }
    getCodes(root->left, code + "0", codes);
    getCodes(root->right, code + "1", codes);
}

int main() {
    string text;
    cout << "Enter text to compress: ";
    if (!getline(cin, text) || text.empty()) return 0;

    // 1. Count character frequencies
    unordered_map<char, int> freq;
    for (char c : text) freq[c]++;

    // 2. Push leaves into min-priority queue (sorted by frequency)
    priority_queue<PQElement, vector<PQElement>, greater<PQElement>> pq;
    for (auto [ch, count] : freq) {
        pq.push({count, make_shared<Node>(ch)});
    }

    // 3. Build Huffman Tree
    if (pq.size() == 1) { // Single unique character edge-case
        auto [f, onlyNode] = pq.top(); pq.pop();
        pq.push({f, make_shared<Node>('\0', onlyNode, nullptr)});
    }
    while (pq.size() > 1) {
        auto [f1, left] = pq.top(); pq.pop();
        auto [f2, right] = pq.top(); pq.pop();
        pq.push({f1 + f2, make_shared<Node>('\0', left, right)});
    }
    shared_ptr<Node> root = pq.top().second;

    // 4. Generate Huffman Codes
    unordered_map<char, string> codes;
    getCodes(root, "", codes);

    cout << "\nHuffman Codes:\n";
    for (auto [ch, code] : codes) {
        cout << "'" << ch << "' : " << code << "\n";
    }

    // 5. Encode
    string encoded = "";
    for (char c : text) encoded += codes[c];
    cout << "\nEncoded Bitstring: " << encoded << "\n";

    // 6. Decode
    string decoded = "";
    auto curr = root;
    for (char bit : encoded) {
        curr = (bit == '0') ? curr->left : curr->right;
        if (!curr->left && !curr->right) {
            decoded += curr->ch;
            curr = root;
        }
    }
    cout << "Decoded Text:      " << decoded << "\n";

    return 0;
}