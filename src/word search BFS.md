### word search BFS

```c++
class Solution {
public:
    bool bfs(vector<vector<char>>& b, int r, int c, string word){
      queue<pair<pair<int, int>, vector<pair<int, int>>>> q;
      int i, j, flag;
      int m = b[0].size();
      int n = b.size();
      pair<int, int> p;
      
      p = make_pair(r,c);
      
      char f;
      vector<pair<int, int>> v;
      
      v.push_back(make_pair(r,c));
      
      q.push(make_pair(p, v));
      pair<pair<int, int>, vector<pair<int, int>>> pt;
      
      while(!q.empty()){
        pt = q.front(); q.pop();
       
        r = pt.first.first;
        c = pt.first.second;
        v = pt.second;
        
        
        if(v.size()==word.size()){
          cout<<r<<c<<"  "<<v.size()<<endl;
          return true;
        }
        if(v.size() < word.size()){  

          if(r + 1 < n && b[r+1][c] == word[v.size()]){
            p = make_pair(r+1, c);
             flag = 0;
             for(j=0;j<v.size();j++){
               if(v[j] == p){
                 flag = 1;
                 break;
               }
              }
              if(flag==0){
                v.push_back(p);
                 q.push(make_pair(p, v));
                v.pop_back();
              }
          }
          if(c +1 < m && b[r][c+1] == word[v.size()]){
            p = make_pair(r, c+1);
              flag = 0;
             for(j=0;j<v.size();j++){
               if(v[j] == p){
                 flag = 1;
                 break;
               }
              }
              if(flag==0){
                v.push_back(p);
                 q.push(make_pair(p, v));
                v.pop_back();
              }
            }
          if(r - 1 >=0 && b[r-1][c] == word[v.size()]){
            p = make_pair(r-1,c);
            flag = 0;
             for(j=0;j<v.size();j++){
               if(v[j] == p){
                 flag = 1;
                 break;
               }
              }
              if(flag==0){
                v.push_back(p);
                 q.push(make_pair(p, v));
                v.pop_back();
              }
          }
          if(c-1 >= 0 && b[r][c-1] == word[v.size()]){
            flag = 0;
            p = make_pair(r,c-1);
           for(j=0;j<v.size();j++){
             if(v[j] == p){
               flag = 1;
               break;
             }
            }
            if(flag==0){
              v.push_back(p);
               q.push(make_pair(p, v));
              v.pop_back();
            }
          }
        }
        v.clear();
      }
      
      return false;
      
    }
    bool exist(vector<vector<char>>& board, string word) {
      int i, j;
      int m = board.size();
      int n = board[0].size();
      bool val;
    
      for(i=0;i<m;i++){
        for(j=0;j<n;j++){
          if(word[0] == board[i][j]){
            val = bfs(board, i, j, word);
            if(val == true)
              return true;
          }
        }
      }
      
      return false;
      
    }
};
```

```c++
class Solution {
public:
    
    bool startswith(string str, string pat) {
        int len = pat.length();
        if (str.length() < len) return false;
        for (int i = 0; i < len; i++) {
            if (str[i] != pat[i])   return false;  
        }
        return true;
    }
    
    bool exist(vector<vector<char>>& board, string word) {
        if (word.length() == 0) return true;
        if (board.size() == 0 || board[0].size() == 0)  return false;
        vector<pair<int, int>> vec;
        bool dp[board.size()][board[0].size()];
        for (int i = 0; i < board.size(); i++) {
            for (int j = 0; j < board[i].size(); j++) {
                if (board[i][j] == word[0]) {
                    vec.push_back(make_pair(i, j));
                }
                dp[i][j] = false;
            }
        }
        
        for (int i = 0; i < vec.size(); i++) {
            int x = vec[i].first, y = vec[i].second;
            list<pair<pair<int, int>, string>> ver;
            string s;
            s.push_back(board[x][y]);
            ver.push_back(make_pair(make_pair(x, y), s));
            memset(dp, false, sizeof(dp));
            
            while (!ver.empty()) {
                string u = ver.front().second;
                x = ver.front().first.first;
                y = ver.front().first.second;
                //cout << u << "::" << x << "::" << y << endl;
                ver.pop_front();
                if (u == word)  return true;
                if (dp[x][y])   continue;
                
                dp[x][y] = true;
                string s = u;
                if (x > 0 && dp[x - 1][y] == false) {
                    s.push_back(board[x - 1][y]);
                    if (startswith(word, s))    ver.push_back(make_pair(make_pair(x - 1, y), s));
                }
                s = u;
                if (x < board.size() - 1 && dp[x + 1][y] == false) {
                    s.push_back(board[x + 1][y]);
                    if (startswith(word, s))    ver.push_back(make_pair(make_pair(x + 1, y), s));
                }
                s = u;
                if (y > 0 && dp[x][y - 1] == false) {
                    s.push_back(board[x][y - 1]);
                    if (startswith(word, s))    ver.push_back(make_pair(make_pair(x, y - 1), s));
                }
                s = u;
                if (y < board[0].size() - 1 && dp[x][y + 1] == false) {
                    s.push_back(board[x][y + 1]);
                    if (startswith(word, s))    ver.push_back(make_pair(make_pair(x, y + 1), s));
                }
                
            }
        }
                          
        return false;
    }
};
```

```c++
class Solution {
public:
    const int d4x[4] = {-1,0,1,0}, d4y[4] = {0,1,0,-1};
    const int d8x[8] = {-1,0,1,0,-1,-1,1,1}, d8y[8] = {0,1,0,-1,-1,1,-1,1};

    int m, n;
    struct State{
        int row;
        int col;
        int idx;
        // bool *visited;
        vector<bool> visited;
    };
    

    bool exist(vector<vector<char>>& board, string word) {
        // if (word.length() == 0) return true;
        // if (board.size() == 0 || board[0].size() == 0)  return false;
        m = board.size();
        n = board[0].size();
        queue<State> bfs_queue;

        
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (board[i][j] == word[0]) {
                    State init;
                    init.row = i;
                    init.col = j;
                    init.idx = 0;
                    
                    for(int k = 0; k < m * n; k++){
                        init.visited.push_back(false);
                    }
                    // init.visited = new bool[m * n];
                    // for(int k = 0; k < m * n; k++){
                    //     init.visited[k] = false;
                    // }
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
                
            for(int i = 0; i < 4; i++){
                State next;
                next.row = current.row + d4x[i];
                next.col = current.col + d4y[i];
                if(next.row >= 0 && next.row < m && next.col >= 0 && next.col < n){

                    if(!current.visited[next.row * n + next.col] && board[next.row][next.col] == word[current.idx + 1]){

                        next.visited = current.visited;
                        // next.visited = new bool[m * n];
                        // for(int k = 0; k < m * n; k++){
                        //     next.visited[k] = current.visited[k];
                        // }
                        next.visited[next.row * n + next.col] = true;
                        next.idx = current.idx + 1;
                        bfs_queue.push(next);
                    }
                }
            }
        }
        return false;
    
    }
};
```

