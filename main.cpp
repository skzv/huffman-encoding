#include <iostream>
#include <queue>
#include <fstream>

struct Node {
    int frequency;
    char symbol;
    Node* left;
    Node* right;
    bool operator<( const Node& n ) const {
        return frequency < n.frequency;
    }
};

struct symbol_freq_greater_than {
    bool operator()(Node* a, Node* b) const {
        return a->frequency > b->frequency;
    }
};

struct Node* newInternalNode(int frequency, Node* left, Node* right) {
    Node* n = new Node;
    n->frequency = frequency;
    n->left = left;
    n->right = right;

    return n;
}

struct Node* newLeafNode(int frequency, char symbol) {
    Node* n = new Node;
    n->frequency = frequency;
    n->symbol = symbol;
    n->left = nullptr;
    n->right = nullptr;

    return n;
}

std::vector<Node*>& createTrees(char* symbols, int* frequencies, int n) {
    auto* v = new std::vector<Node*>;

    for (int i = 0; i < n; i++) {
        Node* node = newLeafNode(frequencies[i], symbols[i]);
        v->push_back(node);
    }

    return *v;
}

int findLongestWordLength(Node* root) {
    if (root == nullptr) {
        return -1;
    }

    return 1 + std::max(findLongestWordLength(root->left), findLongestWordLength(root->right));
}

int findShortestWordLength(Node* root) {
    if (root == nullptr) {
        return -1;
    }

    return 1 + std::min(findShortestWordLength(root->left), findShortestWordLength(root->right));
}

struct Data {
    int size;
    int* frequencies;
};

Data* readFrequencies() {
    std::ifstream infile("input.txt");

    int n;
    infile >> n;
    int* frequencies = new int[n];

    int f;
    int i = 0;
    while (infile >> f) {
        frequencies[i] = f;
        i++;
    }

    Data* data = new Data;
    data->size = n;
    data->frequencies = frequencies;

    return data;
}

int main() {
//    char symbols[] = {'A', 'B', 'C', 'D', 'E', 'F'};
//    int frequencies[] = {3, 2, 6, 8, 2, 6};

    Data* d = readFrequencies();
    int n = d->size;
    int* frequencies = d->frequencies;

    char* symbols = new char[n];
    for (int i = 0; i < n; i++) {
        symbols[i] = 'A';
    }

    std::vector<Node*> v = createTrees(symbols, frequencies, n);
    std::make_heap(v.begin(), v.end(), symbol_freq_greater_than());

    while (v.size() > 1) {
        std::pop_heap(v.begin(), v.end(), symbol_freq_greater_than());
        Node* t1 = v.back();
        v.pop_back();

        std::pop_heap(v.begin(), v.end(), symbol_freq_greater_than());
        Node* t2 = v.back();
        v.pop_back();

        Node* t3 = newInternalNode(t1->frequency + t2->frequency, t1, t2);
        v.push_back(t3);
        std::push_heap(v.begin(), v.end(), symbol_freq_greater_than());
    }

    int longestWordLength = findLongestWordLength(v.back());
    int shortestWordLength = findShortestWordLength(v.back());
    std::cout << "Longest word length: " << longestWordLength << std::endl;
    std::cout << "Shortest word length: " << shortestWordLength << std::endl;

    return 0;
}

