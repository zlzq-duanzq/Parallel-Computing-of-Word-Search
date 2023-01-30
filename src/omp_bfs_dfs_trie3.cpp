// g++ -o find omp_bfs_dfs3.cpp -std=c++11
// ./find 100 100 board_100.txt 100 wordList_100.txt

#include "load.h"
#include <stack>
#include <cstring>
#include <omp.h>
using namespace std;

int m, n;
omp_lock_t queuelock;
omp_lock_t resultlock;

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

void dfs(char *board, int i, int j, TrieNode *node, set<string> &result) {
    char c = board[i * n + j];
    if (c == '#' || !node->children[c - 'a']) return;
    node = node->children[c - 'a'];
    if (node->word.size() > 0) {
        omp_set_lock(&resultlock);
        result.insert(node->word);
        node->word = "";
        omp_unset_lock(&resultlock);
    }

    board[i * n + j] = '#';
    for(int d = 0; d < 8; d++){
        int next_i = i + d8x[d];
        int next_j = j + d8y[d];
        if(next_i >= 0 && next_i < m - 1 && next_j >= 0 && next_j < n- 1){
            dfs(board, next_i, next_j, node, result);
        }
    }
    board[i * n + j] = c;
}

void bfs(char *board, TrieNode *root, set<string> &result, queue<State>& bfs_queue){

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            State init;
            init.row = i;
            init.col = j;
            init.node = root;
            init.board = new char[m * n];
            // for(int k = 0; k < m * n; k++){
            //     init.board[k] = board[k/n][k%n];
            // }
            memcpy(init.board, board, m * n);
            //omp_set_lock(&queuelock);
            bfs_queue.push(init);
            //omp_unset_lock(&queuelock);
        }
    }
    int iteration = 2;
    for(int i = 0; i < iteration; i++){
        int size = bfs_queue.size();
        if(size == 0) break;

        #pragma omp parallel for
        for (int i = 0; i < size; i++) {
            //omp_set_lock(&queuelock);
            State current = bfs_queue.front();
            bfs_queue.pop();
            //omp_unset_lock(&queuelock);

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
                    next.board[current.row * n + current.col] = '#';
                    //omp_set_lock(&queuelock);
                    bfs_queue.push(next);
                    //omp_unset_lock(&queuelock);
                }
            }
        }
    }
}


set<string> findWords(vector<vector<char>> &board, vector<string> &words) {
    set<string> result;
    TrieNode *root = buildTrie(words);
    queue<State> bfs_queue;

    char *char_board = new char[m * n];
    for(int k = 0; k < m * n; k++){
        char_board[k] = board[k/n][k%n];
    }

    double beginTime, endTime;
    beginTime = omp_get_wtime();

    bfs(char_board, root, result, bfs_queue);

    endTime = omp_get_wtime();
    cout << "BFS Time: " << endTime - beginTime << endl;


    beginTime = omp_get_wtime();
    int size = bfs_queue.size();
    
    #pragma omp parallel
	{
        #pragma omp for
        for (int i = 0; i < size; ++i) {
            omp_set_lock(&queuelock);
            State current = bfs_queue.front();
            bfs_queue.pop();
            omp_unset_lock(&queuelock);
            dfs(current.board, current.row, current.col, current.node, result);
        }
    }
    endTime = omp_get_wtime();
    cout << "DFS Time: " << endTime - beginTime << endl;
    return result;
}

int main(int argc, char** argv){
    #pragma omp parallel
    {
        int p = omp_get_num_threads();
    	int id = omp_get_thread_num();
        //printf("id: %d\n", id);
    #pragma omp single
		printf("Number of threads: %d\n", omp_get_num_threads());
    }

    omp_init_lock(&queuelock);
    omp_init_lock(&resultlock);

    m = atoi(argv[1]);
    n = atoi(argv[2]);
    string board_filename = argv[3];
    int num_words = atoi(argv[4]);
    string word_filename = argv[5];
    
    vector<vector<char>> board(m , vector<char> (n));
    readBoard(board, board_filename);

    vector<string> words(num_words);
    readWordList(words, word_filename);

    double beginTime, endTime;
    beginTime = omp_get_wtime();

    set<string> result = findWords(board, words);

    endTime = omp_get_wtime();
    cout << "Time: " << endTime - beginTime << endl;

    for(string val: result){
        cout << val << endl;
    }
    // string filename = "result_" + board_filename.substr(0,board_filename.size()-4) + "_" + word_filename;
    // writeResult(result, filename);
}
