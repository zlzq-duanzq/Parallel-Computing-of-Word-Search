// g++ -o find serial_dfs.cpp -std=c++11
// ./find 100 100 board_100.txt 100 wordList_100.txt
// ./find 1000 1000 board_1000.txt 100 wordList_100.txt

#include <chrono>
#include "load.h"
// #include <omp.h>
using namespace std;

bool dfs(vector<vector<char>>& board, int r, int c, int m, int n, int len, string& word){
    if(len == word.size()) return true;
    if(r < 0 || c < 0 || r >= m || c >= n || board[r][c] == '#' || board[r][c] != word[len]) return false;
    char tmp = board[r][c];
    board[r][c] = '#';

    bool found = false;
    for(int d = 0; d < 8; d++){
        int next_i = r + d8x[d];
        int next_j = c + d8y[d];
        if(next_i >= 0 && next_i < board.size() - 1 && next_j >= 0 && next_j < board[0].size() - 1){
            bool next_found = dfs(board, next_i, next_j, m, n, len + 1, word);
            found = found || next_found;
        }
    }
    board[r][c] = tmp;
    return found;
}

bool exist(vector<vector<char>>& board, string word) {
    if(board.empty()) return false;
    int m = board.size(), n = board[0].size();
    for(int i = 0; i < m; i++)
        for(int j = 0; j < n; j++)
            if(board[i][j] == word[0] && dfs(board, i, j, m, n, 0, word)) return true;
    return false;
}

set<string> findWords(vector<vector<char>> &board, vector<string> &words) {
    set<string> result;
    for(string word: words){
        if(exist(board, word)){
            result.insert(word);
        }
    }
    return result;
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
