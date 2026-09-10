#include<iostream>
#include<vector>
#include<queue>
#include<stack>
#include<deque>
#include<algorithm>
#include<string>
#include<functional>
#include<utility>
using namespace std;
const long long INF = (1LL << 62);

void bfs(vector<string> &grid,vector<pair<int,int>> &src,
         vector<vector<int>> &dist){
    int n = grid.size();
    int m = 0;
    if(n > 0){
        m = grid[0].size();
    }
    dist.assign(n,vector<int>(m,-1));
    queue<pair<int,int>> q;
    for(auto [x,y] : src){
        if(grid[x][y] != 'X' && dist[x][y] == -1){
            dist[x][y] = 0;
            q.push({x,y});
        }
    }
    int dx[4] = {1,-1,0,0};
    int dy[4] = {0,0,1,-1};
    while(!q.empty()){
        auto [x,y] = q.front();
        q.pop();
        for(int i = 0;i < 4;i ++){
            int nx = x + dx[i];
            int ny = y + dy[i];
            if(nx < 0 || nx >= n || ny < 0 || ny >= m){
                continue;
            }
            if(grid[nx][ny] == 'X' || dist[nx][ny] != -1){
                continue;
            }
            dist[nx][ny] = dist[x][y] + 1;
            q.push({nx,ny});
        }
    }
}
