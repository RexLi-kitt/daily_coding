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

void dijkstra(int s,vector<vector<pair<int,long long>>> &g,
              vector<long long> &dist){
    dist.assign(g.size(),INF);
    vector<bool> done(g.size(),false);
    priority_queue<pair<long long,int>,
                   vector<pair<long long,int>>,greater<>> pq;
    dist[s] = 0;
    pq.push({0,s}); // 距离在前，点的编号在后
    while(!pq.empty()){
        auto [d,u] = pq.top();
        pq.pop();
        if(done[u]){
            continue;
        }
        done[u] = true;
        for(auto [v,w] : g[u]){
            if(dist[u] + w < dist[v]){
                dist[v] = dist[u] + w;
                pq.push({dist[v],v});
            }
        }
    }
}
// 调用：vector<long long> dist; dijkstra(s,g,dist);
// dist[t] == INF 表示不可达，按题意输出。
// 边权也须小于 INF，保证 dist[u]+w 不溢出。
// 记录路径：更新成功时 pre[v]=u；从终点回溯，再 reverse。
