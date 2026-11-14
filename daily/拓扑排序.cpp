#include<iostream>
#include<vector>
#include<queue>
using namespace std;
//只适用于DAG
vector<int> topo(int n, vector<vector<int>>& adj){
    vector<int> indeg(n + 1, 0);
    for(int u = 1;u <= n;u ++){
        for(int v : adj[u]){
            indeg[v] ++;
        }
    }

    queue<int> q;
    for(int i = 1;i <= n;i ++){
        if(indeg[i] == 0) q.push(i);
    }
    
    vector<int> res;
    while(!q.empty()){
        int u = q.front(); q.pop();
        res.push_back(u);
        for(int v : adj[u]){
            if(--indeg[v] == 0){
                q.push(v);
            }
        }
    }
    return res;
}
int main(){

}