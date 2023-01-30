    vector<vector<char>> board{
        {'o','a','a','n'},
        {'e','t','a','e'},
        {'i','h','k','r'},
        {'i','f','l','v'}
    };
    vector<string> words{"oath","pea","eat","rain", "henry", "sandy", "pan"};


    if (i > 0) dfs(board, i - 1, j, p, result);
    if (j > 0) dfs(board, i, j - 1, p, result);
    if (i < board.size() - 1) dfs(board, i + 1, j, p, result);
    if (j < board[0].size() - 1) dfs(board, i, j + 1, p, result);

Shuffle the queue