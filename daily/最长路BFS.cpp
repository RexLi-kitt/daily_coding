#include<iostream>
#include<queue>
#include<vector>
using namespace std;
const int INF = -1145141;
void bfs(vector<vector<pair<int,int>>> &g,vector<int> &dp){
    queue<int> q;
    dp[1] = 0;
    q.push(1);
    while(!q.empty()){
        int cur = q.front();
        q.pop();
        for(auto[v,w] : g[cur]){
            if(dp[v] == INF){
                dp[v] = dp[cur] + w;
                q.push(v);
            }
            else if(dp[v] < dp[cur] + w){
                dp[v] = dp[cur] + w;
                q.push(v);

            }
        }
    }
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
    vector<bool> vis(n + 1,false);
    vector<int> dp(n + 1,INF);
    bfs(g,dp);
    if(dp[n] == INF){
        cout << -1;
    }
    else{
        cout << dp[n];
    }
}