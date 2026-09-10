#include<iostream>
#include<vector>
#include<queue>
#include<functional>
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

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n,m,s;
    cin >> n >> m >> s;
    vector<vector<pair<int,long long>>> g(n + 1);
    for(int i = 0;i < m;i ++){
        int u,v;
        long long w;
        cin >> u >> v >> w;
        g[u].push_back({v,w});
    }
    vector<long long> dist;
    dijkstra(s,g,dist);
    for(int i = 1;i <= n;i ++){
        if(dist[i] == INF){
            cout << -1;
        }
        else{
            cout << dist[i];
        }
        if(i != n){
            cout << ' ';
        }
    }
    cout << '\n';
    return 0;
}
