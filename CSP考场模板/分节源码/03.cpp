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

void bfs(int s,vector<vector<int>> &g,vector<int> &dist){
    dist.assign(g.size(),-1);
    queue<int> q;
    dist[s] = 0;
    q.push(s);
    while(!q.empty()){
        int u = q.front();
        q.pop();
        for(int v : g[u]){
            if(dist[v] == -1){
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
}
// vector<int> dist;
// bfs(s,g,dist);
// cout << dist[t]; // 这里约定不可达输出 -1，实际看题目要求
