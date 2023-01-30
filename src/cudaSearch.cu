#include "load.h"
#include <omp.h>
using namespace std;

struct State{
    int row;
    int col;
    TrieNode *p;
    char* board;
};

struct TrieNode {
    TrieNode *children[26];
    string word;
    TrieNode() : word("") {
        for (int i = 0; i < 26; i++) {
            children[i] = nullptr;
        }
    }
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

__global__ void backtrackKernel(int *boards,
        const int numBoards,
        int *emptySpaces,
        int *numEmptySpaces,
        int *finished,
        int *solved) {
        }

void cudaBacktrackKernel(const unsigned int blocks,
        const unsigned int threadsPerBlock,
        int *boards,
        const int numBoards,
        int *emptySpaces,
        int *numEmptySpaces,
        int *finished,
        int *solved) {

    backtrackKernel<<<blocks, threadsPerBlock>>>
        (boards, numBoards, emptySpaces, numEmptySpaces, finished, solved);
}

__global__ void cudaBFSKernel(int *old_boards,
        int *new_boards,
        int total_boards,
        int *board_index,
        int *empty_spaces,
        int *empty_space_count) {
            
        }
void callBFSKernel(const unsigned int blocks, 
                        const unsigned int threadsPerBlock,
                        int *old_boards,
                        int *new_boards,
                        int total_boards,
                        int *board_index,
                        int *empty_spaces,
                        int *empty_space_count) {
    cudaBFSKernel<<<blocks, threadsPerBlock>>>
        (old_boards, new_boards, total_boards, board_index, empty_spaces, empty_space_count);
}



int main(int argc, char** argv){
    if (argc < 8){
        printf("More arguments needed\n");
        exit(-1);
    }

    m = atoi(argv[1]);
    n = atoi(argv[2]);
    string board_filename = argv[3];
    int num_words = atoi(argv[4]);
    string word_filename = argv[5];
    
    const unsigned int threadsPerBlock = atoi(argv[6]);
    const unsigned int maxBlocks = atoi(argv[7]);

    vector<vector<char>> board(m , vector<char> (n));
    readBoard(board, board_filename);

    vector<string> words(num_words);
    readWordList(words, word_filename);

    TrieNode *root = buildTrie(words);
    
    // Initialize the CUDA timer
    float elapsedTime = 0;
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start);

    // TODO
    

    // Stop the CUDA timer
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&elapsedTime, start, stop);
}