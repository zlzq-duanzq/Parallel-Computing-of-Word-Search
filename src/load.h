#ifndef LOAD_H
#define LOAD_H

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <set>

using namespace std;

const int d4x[4] = {-1,0,1,0}, d4y[4] = {0,1,0,-1};
const int d8x[8] = {-1,0,1,0,-1,-1,1,1}, d8y[8] = {0,1,0,-1,-1,1,-1,1};

void printBoard(vector<vector<char>> &board){
    for(int i = 0; i < board.size(); i++){
        for(int j = 0; j < board[0].size(); j++){
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

void readBoard(vector<vector<char>> &board, string filename){
    fstream newfile;
    newfile.open(filename, ios::in);
    int row_idx = 0;
    if (newfile.is_open()){
        string line;
        while(getline(newfile, line)){
            for(int i = 0; i < line.size(); i += 2){
                board[row_idx][i / 2] = line[i];
            }
            row_idx++;
        }
        newfile.close();
    }
    //printBoard(board);
}

void printWords(vector<string> &words){
    for(int i = 0; i < words.size(); i++){
            cout << words[i] << " ";
    }
    cout << endl;
}

void readWordList(vector<string> &words, string filename){
    fstream newfile;
    newfile.open(filename, ios::in);
    int row_idx = 0;
    if (newfile.is_open()){
        string line;
        while(getline(newfile, line)){
            words[row_idx] = line;
            row_idx++;
        }
        newfile.close();
    }
    //printWords(words);
}

void writeResult(set<string> result, string filename){
    ofstream outFile(filename);
    for(string str: result){
        outFile << str << endl;
    }
    outFile.close();
}

#endif  // LOAD_H