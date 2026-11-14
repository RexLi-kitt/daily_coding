#include<iostream>
#include<vector>
#include<queue>
using namespace std;
vector<int> topo(vector<vector<pair<int,int>>> &g,int n){
    vector<int> indeg(n + 1);
    for(int i = 1;i <= n; i++){
        for(auto&[v,w] : g[i]){
            indeg[v] ++;
        }
    }
    queue<int> q;
    for(int i = 1;i <= n;i ++){
        if(indeg[i] == 0){
            q.push(i);
        }
    }
    vector<int> ans;
    while (!q.empty()){
        int u = q.front();
        q.pop();
        ans.push_back(u);
        for(auto[v,w] : g[u]){
            indeg[v]--; 
            if(indeg[v] == 0){
                q.push(v);
            }
        }
    }
    return ans;
}
int main(){
    int n,m;
    cin >> n >> m;
    vector<vector<pair<int,int>>> g(n + 1);
    for(int i = 0;i < m;i ++){
        int u,v,w;
        cin >> u >> v >> w;
        g[u].push_back({v,w});
    }
    vector<int> topo_q = topo(g,n);
    vector<int> dp(n + 1,-10000000);
    dp[1] = 0;
    for(int u : topo_q){
        if(dp[u] == -10000000) continue;
        for(auto[v,w] : g[u]){
            dp[v] = max(dp[v], dp[u] + w);
        }
    }
    if(dp[n] == -10000000){
        cout << -1;
        return 0;
    }
    cout << dp[n];
}