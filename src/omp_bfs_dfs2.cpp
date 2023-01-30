#include "load.h"
#include <omp.h>
using namespace std;

int m, n;
struct State{
    int row;
    int col;
    int idx;
    char* board;
    // bool *visited;
};

bool dfs(char* board, int r, int c, int len, string& word){
    if(len == word.length()) return true;
    if(r < 0 || c < 0 || r >= m || c >= n || board[r * n + c] == '#' || board[r * n + c] != word[len]) return false;
    char tmp = board[r * n + c];
    board[r * n + c] = '#';

    bool found = false;
    for(int d = 0; d < 8; d++){
        int next_i = r + d8x[d];
        int next_j = c + d8y[d];
        if(next_i >= 0 && next_i < m && next_j >= 0 && next_j < n){
            bool next_found = dfs(board, next_i, next_j, len + 1, word);
            found = found || next_found;
        }
    }
    board[r * n + c] = tmp;
    return found;
}

bool bfs(vector<vector<char>>& board, string word){

    // BFS
    //int size_limit = omp_get_num_threads();
    int size_limit = m * n;
    queue<State> bfs_queue;
    bool found = false;
    omp_lock_t queuelock;
    omp_lock_t foundlock;
    omp_init_lock(&queuelock);
    omp_init_lock(&foundlock);

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == word[0]) {
                State init;
                init.row = i;
                init.col = j;
                init.idx = 0;
                
                init.board = new char[m * n];
                for(int k = 0; k < m * n; k++){
                    init.board[k] = board[k/n][k%n];
                }
                // init.board[i * n + j] = '#';
                bfs_queue.push(init);
            }
        }
    }
    
    while (bfs_queue.size() < size_limit) {
        int size = bfs_queue.size();
        if(size == 0) break;

        for (int i = 0; i < size; i++) {
            State current = bfs_queue.front();
            bfs_queue.pop();
            if(current.idx == word.length() - 1){
                return true;
            }
            for(int i = 0; i < 8; i++){
                State next;
                next.row = current.row + d8x[i];
                next.col = current.col + d8y[i];
                if(next.row >= 0 && next.row < m && next.col >= 0 && next.col < n){
        
                    if(current.board[next.row * n + next.col] == word[current.idx + 1]){
                        next.board = new char[m * n];
                        for(int k = 0; k < m * n; k++){
                            next.board[k] = current.board[k];
                        }
                        next.board[next.row * n + next.col] = '#';
                        //delete[] current.board;
                        next.idx = current.idx + 1;
                        bfs_queue.push(next);
                    }
                }
            }
        }
    }

    // DFS
    int size = bfs_queue.size();
    #pragma omp parallel
	{
        #pragma omp for
        for (int i = 0; i < size; ++i) {
            if(found) continue;
            omp_set_lock(&queuelock);
            State current = bfs_queue.front();
            bfs_queue.pop();
            omp_unset_lock(&queuelock);
            bool tmp = dfs(current.board, current.row, current.col, current.idx, word);

            if(tmp){
                omp_set_lock(&foundlock);
                found = true;
                omp_unset_lock(&foundlock);
            }
        }
    }
    return found;
}


set<string> findWords(vector<vector<char>> &board, vector<string> &words) {
    set<string> result;
    for(string word: words){
        if(bfs(board, word)){
            result.insert(word);
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
