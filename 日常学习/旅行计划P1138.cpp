#include<iostream>
#include<vector>
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
    int length = n;
    vector<int> last(n + 1);
    for(int i = 1;i <= n;i ++){
        int u = whole_road[i];
        last[u] = length - i;
    }
    for(int i = 1;i <= n;i ++){
        cout << last[i] << endl;
    }
}