// g++ -o find serial_bfs.cpp -std=c++11
// ./find 100 100 board_100.txt 100 wordList_100.txt

#include <chrono>
#include "load.h"
// #include <omp.h>
using namespace std;


int m, n;
struct State{
    int row;
    int col;
    int idx;
    bool *visited;
};

bool exist(vector<vector<char>>& board, string word) {
    queue<State> bfs_queue;
    
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == word[0]) {
                State init;
                init.row = i;
                init.col = j;
                init.idx = 0;
                
                init.visited = new bool[m * n];
                for(int k = 0; k < m * n; k++){
                    init.visited[k] = false;
                }
                init.visited[i * n + j] = true;
                bfs_queue.push(init);
            }
        }
    }
    while(bfs_queue.size() != 0){
        State current = bfs_queue.front();
        bfs_queue.pop();
        //cout << current.row << " "<< current.col << " " << current.idx <<  " " << word.length() - 1 << endl;
        if(current.idx == word.length() - 1)
            return true;
            
        for(int i = 0; i < 8; i++){
            State next;
            next.row = current.row + d8x[i];
            next.col = current.col + d8y[i];
            if(next.row >= 0 && next.row < m && next.col >= 0 && next.col < n){
                if(!current.visited[next.row * n + next.col] && board[next.row][next.col] == word[current.idx + 1]){
                    next.visited = new bool[m * n];
                    for(int k = 0; k < m * n; k++){
                        next.visited[k] = current.visited[k];
                    }
                    next.visited[next.row * n + next.col] = true;
                    //delete[] current.visited;
                    next.idx = current.idx + 1;
                    bfs_queue.push(next);
                }
            }
        }
    }
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
    double beginTime, endTime;
    
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

