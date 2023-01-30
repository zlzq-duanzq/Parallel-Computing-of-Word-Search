#include "load.h"
#include <omp.h>
using namespace std;

omp_lock_t boardlock, resultlock;    

struct TrieNode {
    TrieNode *children[26];
    string word;
    omp_lock_t lock;
    TrieNode() : word("") {
        for (int i = 0; i < 26; i++) {
            children[i] = nullptr;
        }
        omp_init_lock(&lock);
    }
};

set<string> findWords(vector<vector<char>> &board, vector<string> &words);
TrieNode *buildTrie_pp(vector<string> &words);
TrieNode *buildTrie(vector<string> &words);
void dfs(vector<vector<char>> &board, int i, int j, TrieNode *p, set<string> &result);


set<string> findWords(vector<vector<char>> &board, vector<string> &words) {
    double beginTime, endTime;
    beginTime = omp_get_wtime();

    TrieNode *root = buildTrie(words);
    endTime = omp_get_wtime();
    cout << "buildTrie Time: " << endTime - beginTime << endl;

    beginTime = omp_get_wtime();
    root = buildTrie_pp(words);
    endTime = omp_get_wtime();
    cout << "buildTrie in parallel Time: " << endTime - beginTime << endl;

    
    set<string> result;
    cout << "for collapse(2)" << endl;
#pragma omp parallel for collapse(2)
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[0].size(); j++) {
            dfs(board, i, j, root, result);
        }
    }
    return result;
}

TrieNode *buildTrie_pp(vector<string> &words) {
    TrieNode *root = new TrieNode();
#pragma omp parallel 
	{
#pragma omp for
        for (int j = 0; j < words.size(); j++) {
            string word = words[j];
            TrieNode *curr = root;
            for (int i = 0; i < word.length(); i++) {
                char c = word[i] - 'a';

                omp_set_lock(&curr->lock);
                if (curr->children[c] == nullptr) {
                    curr->children[c] = new TrieNode();
                }
                omp_unset_lock(&curr->lock);
                curr = curr->children[c];
            }
            omp_set_lock(&curr->lock);
            curr->word = word;
            omp_unset_lock(&curr->lock);
        }
    }
    return root;
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
        omp_set_lock(&resultlock);
        result.insert(p->word);
        omp_unset_lock(&resultlock);

        omp_set_lock(&p->lock);
        p->word = "";
        omp_unset_lock(&p->lock);
    }

    omp_set_lock(&boardlock);
    board[i][j] = '#';
    omp_unset_lock(&boardlock);

    for(int d = 0; d < 8; d++){
        int next_i = i + d8x[d];
        int next_j = j + d8y[d];
        if(next_i >= 0 && next_i < board.size() - 1 && next_j >= 0 && next_j < board[0].size() - 1){
            dfs(board, next_i, next_j, p, result);
        }
    }
    omp_set_lock(&boardlock);
    board[i][j] = c;
    omp_unset_lock(&boardlock);
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
    omp_init_lock(&boardlock);
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
