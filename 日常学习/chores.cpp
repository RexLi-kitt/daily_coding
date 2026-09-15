#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>
using namespace std;
vector<int> topo(int n,vector<vector<int>> &adj){
    vector<int> indeg(n + 1,0);
    for(int i = 1;i <= n;i ++){
        for(int u : adj[i]){
            indeg[u] ++;
        }
    }
    queue<int> q;
    for(int i = 1;i <= n;i ++){
        if(indeg[i] == 0) q.push(i);
    }
    vector<int> res;
    while(!q.empty()){
        int u = q.front();q.pop();
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
    int n;
    cin >> n;
    vector<bool> done(n + 1,false);
    vector<int> data(n + 1);
    vector<vector<int>> adj(n + 1);
    vector<int> t(n + 1);
    for(int i = 0;i < n;i ++){
        int id, time;
        cin >> id >> time;
        t[id] = time;
        int nei = 1;
        while(nei != 0){
            cin >> nei;
            if(nei != 0)adj[id].push_back(nei);
        }
    }
    vector<int> toppo = topo(n,adj);
    reverse(toppo.begin(),toppo.end());
    //dp[i] 完成第i个任务的最短时间
    vector<int> dp(n + 1);
    dp[0] = 0;
    done[0] = true;
    for(int i = 0;i < n;i ++){
        int cur = toppo[i];
        for(int u : adj[cur]){
            if(done[u]){
                dp[cur] = max(dp[cur],dp[u]);
            }
        }
        dp[cur] += t[cur];
        done[cur] = true;
    }
    int ans = 0;
    for(int i = 1;i <= n;i ++){
        ans = max(ans, dp[i]);
    }
    cout << ans;
}