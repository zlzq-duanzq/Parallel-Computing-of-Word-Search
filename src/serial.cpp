// g++ -o find serial.cpp -std=c++11
// ./find 100 100 board_100.txt 100 wordList_100.txt
// ./find 50 50 board_50.txt 50 wordList_50.txt
// ./find 100 100 board_100.txt 50 wordList_50.txt
// ./find 500 500 board_500.txt 50 wordList_50.txt

#include <chrono>
#include "load.h"
// #include <omp.h>
using namespace std;

struct TrieNode {
    TrieNode *children[26];
    string word;
    TrieNode() : word("") {
        for (int i = 0; i < 26; i++) {
            children[i] = nullptr;
        }
    }
};

set<string> findWords(vector<vector<char>> &board, vector<string> &words);
TrieNode *buildTrie(vector<string> &words);
void dfs(vector<vector<char>> &board, int i, int j, TrieNode *p, set<string> &result);


set<string> findWords(vector<vector<char>> &board, vector<string> &words) {
    TrieNode *root = buildTrie(words);
    set<string> result;

    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[0].size(); j++) {
            dfs(board, i, j, root, result);
        }
    }
    return result;
}

TrieNode *buildTrie(vector<string> &words) {
    TrieNode *root = new TrieNode();

    for (int j = 0; j < words.size(); j++) {
        string word = words[j];
        TrieNode *curr = root;
        for (int i = 0; i < word.length(); i++) {
            char c = word[i] - 'a';
            if (curr->children[c] == nullptr) {
                curr->children[c] = new TrieNode();
            }
            curr = curr->children[c];
        }
        curr->word = word;
    }
    
    return root;
}

void dfs(vector<vector<char>> &board, int i, int j, TrieNode *p, set<string> &result) {
    char c = board[i][j];
    if (c == '#' || !p->children[c - 'a']) return;
    p = p->children[c - 'a'];
    if (p->word.size() > 0) {
        result.insert(p->word);
        p->word = "";
    }

    board[i][j] = '#';
    for(int d = 0; d < 8; d++){
        int next_i = i + d8x[d];
        int next_j = j + d8y[d];
        if(next_i >= 0 && next_i < board.size() - 1 && next_j >= 0 && next_j < board[0].size() - 1){
            dfs(board, next_i, next_j, p, result);
        }
    }
    board[i][j] = c;
}

int main(int argc, char** argv){
    int m = atoi(argv[1]), n = atoi(argv[2]);
    string board_filename = argv[3];
    int num_words = atoi(argv[4]);
    string word_filename = argv[5];

    vector<vector<char>> board(m , vector<char> (n));
    readBoard(board, board_filename);

    vector<string> words(num_words);
    readWordList(words, word_filename);

    auto start = chrono::high_resolution_clock::now();
    
    set<string> result = findWords(board, words);

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "Time: " << (double)duration.count() / 1000000 << " second" << endl;

    for(string val: result){
        cout << val << endl;
    }
    // string filename = "result_" + board_filename.substr(0,board_filename.size()-4) + "_" + word_filename;
    // writeResult(result, filename);
}
