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

vector<int> topo(vector<vector<pair<int,long long>>> &g,int n){
    vector<int> indeg(n + 1,0);
    for(int i = 1;i <= n;i ++){
        for(auto [v,w] : g[i]){
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
    while(!q.empty()){
        int u = q.front();
        q.pop();
        ans.push_back(u);
        for(auto [v,w] : g[u]){
            indeg[v] --;
            if(indeg[v] == 0){
                q.push(v);
            }
        }
    }
    return ans;
}
// 返回 false 表示有环，dp 通过引用参数带回
bool longest(int s,vector<vector<pair<int,long long>>> &g,
             vector<long long> &dp){
    int n = (int)g.size() - 1;
    vector<int> topo_q = topo(g,n);
    dp.assign(n + 1,-INF);
    if((int)topo_q.size() != n){
        return false;
    }
    dp[s] = 0;
    for(int u : topo_q){
        if(dp[u] == -INF){
            continue;
        }
        for(auto [v,w] : g[u]){
            dp[v] = max(dp[v],dp[u] + w);
        }
    }
    return true;
}
