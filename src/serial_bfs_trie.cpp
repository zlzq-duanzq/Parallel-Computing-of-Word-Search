// g++ -o find serial_bfs_trie.cpp -std=c++11
// ./find 100 100 board_100.txt 100 wordList_100.txt

#include "load.h"
#include <stack>
// #include <omp.h>
using namespace std;

int m, n;

struct TrieNode {
    TrieNode *children[26];
    string word;
    TrieNode() : word("") {
        for (int i = 0; i < 26; i++) {
            children[i] = nullptr;
        }
    }
};

struct State{
    int row;
    int col;
    TrieNode *node;
    char* board;
};

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

void bfs(vector<vector<char>>& board, TrieNode *root, set<string> &result){
    queue<State> bfs_queue;
    // stack<State> bfs_queue;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            State init;
            init.row = i;
            init.col = j;
            init.node = root;
            init.board = new char[m * n];
            for(int k = 0; k < m * n; k++){
                init.board[k] = board[k/n][k%n];
            }
            bfs_queue.push(init);
        }
    }
    // int iteration = 10;
    // for(int i = 0; i < iteration; i++){
    while(bfs_queue.size() > 0){
        int size = bfs_queue.size();
        if(size == 0) break;

        for (int i = 0; i < size; i++) {
            State current = bfs_queue.front();
            // State current = bfs_queue.top();
            bfs_queue.pop();
            
            TrieNode *node = current.node;
            // char c = board[current.row][current.col];
            char c = current.board[current.row * n + current.col];
            if (c == '#' || !node->children[c - 'a']) 
                continue;

            node = node->children[c - 'a'];

            if (node->word.size() > 0) {
                result.insert(node->word);
                node->word = "";
            }

            for(int i = 0; i < 8; i++){
                State next;
                next.row = current.row + d8x[i];
                next.col = current.col + d8y[i];
                if(next.row >= 0 && next.row < m && next.col >= 0 && next.col < n){
                    next.node = node;
                    next.board = current.board;
                    // next.board = new char[m * n];
                    // for(int i = 0; i < m * n; i++)
                    //     next.board[i] = current.board[i];
                    next.board[current.row * n + current.col] = '#';
                    bfs_queue.push(next);
                }
            }
        }
    }

}


set<string> findWords(vector<vector<char>> &board, vector<string> &words) {
    set<string> result;
    TrieNode *root = buildTrie(words);
    bfs(board, root, result);
    return result;
}

int main(int argc, char** argv){


    m = atoi(argv[1]);
    n = atoi(argv[2]);
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
