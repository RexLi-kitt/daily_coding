#include<iostream>
#include<vector>
#include<queue>
using namespace std;
vector<int> topo(int n,vector<vector<int>> &adj){
    vector<int> indeg(n + 1);
    for(int i = 1;i <= n;i ++){
        for(int u : adj[i]){
            indeg[u] ++;
        }
    }

    queue<int> q;
    for(int i = 1;i <= n;i ++){
        if(indeg[i] == 0){
            q.push(i);
        }
    }

    vector<int> res;
    while (!q.empty()){
        int u = q.front();
        q.pop();
        res.push_back(u);
        for(int v : adj[u]){
            if(-- indeg[v] == 0){
                q.push(v);
            }
        }
    }
    return res;
}   
int main(){
    int n,m;
    cin >> n >> m;
    vector<vector<int>> adj(n + 1);
    for(int i = 0;i < m;i ++){
        int u,v;
        cin >> u >> v;
        adj[u].push_back(v);
    }
    vector<int> whole_road = topo(n,adj);
    vector<int> dp(n + 1,1);
    for(int u : whole_road){
        for(int v : adj[u]){
            dp[v] = max(dp[v],dp[u] + 1);
        }
    }
    for(int i = 1;i <= n;i ++){
        cout << dp[i] << endl;
    }
}