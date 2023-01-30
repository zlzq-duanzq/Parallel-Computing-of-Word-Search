// g++ -o test getBoard.cpp -std=c++11

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <fstream>

using namespace std;

void printFile(string filename){
    fstream newfile;
    newfile.open(filename, ios::in);
    if (newfile.is_open()){
        string line;
        while(getline(newfile, line)){
            cout << line << endl;
        }
        newfile.close();
    }
}

int main(){
    srand(time(0));

    int m = 50;
    int n = 50;
    string filename = "board_50.txt";
    vector<vector<char>> board;
    ofstream myfile;
    myfile.open (filename);

    for(int i = 0; i < m; i++){
        vector<char> row;
        for(int j = 0; j < n; j++){
            char c = (rand() % 26) + 'a';
            row.push_back(c);
        }
        board.push_back(row);
    }
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            myfile << board[i][j] << " ";
        }
        myfile << endl;
    }
    myfile.close();

    return 0;
}