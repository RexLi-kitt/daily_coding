#include<iostream>
#include<vector>
#include<stack>
using namespace std;
vector<vector<int>> graph(100005);
vector<int> ans(100005);
void add(int u,int v){
    graph[v].push_back(u);
}
void dfs(int u){
    stack<int> q;
    q.push(u);
    ans[u] = u;
    while(!q.empty()){
        int v = q.top();q.pop();
        for(int j : graph[v]){
            if(!ans[j]){
                ans[j] = ans[v];
                q.push(j);  
            }
            else if(ans[j] > ans[v]){
                continue;
            }
        }
    }
}
int main(){
    int n,m;
    cin >> n >> m;
    for(int i = 0;i < m;i ++){
        int u,v;
        cin >> u >> v;
        add(u, v);
    }
    for(int i = n;i >= 1;i --){
        if(ans[i] == 0) dfs(i);
    }
    for(int i = 1;i <= n;i ++){
        cout << ans[i] <<" ";
    }
}