#include "load.h"
#include <omp.h>
using namespace std;

omp_lock_t resultlock;

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

    #pragma omp parallel for
    for(string word: words){
        if(exist(board, word)){
            omp_set_lock(&resultlock);
            result.insert(word);
            omp_unset_lock(&resultlock);
        }
    }
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
    omp_init_lock(&resultlock);

    int m = atoi(argv[1]), n = atoi(argv[2]);
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
