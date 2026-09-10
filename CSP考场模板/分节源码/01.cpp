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

void dfs(int u,vector<vector<int>> &g,vector<bool> &vis){
    vis[u] = true;
    // 在这里处理当前点 u
    for(int v : g[u]){
        if(!vis[v]){
            dfs(v,g,vis);
        }
    }
}
// 调用：vector<bool> vis(n + 1,false); dfs(s,g,vis);
// 求无向图连通块：枚举 i=1..n，没访问过就 cnt++，dfs(i,g,vis)。

// 点很多、递归可能太深时，用栈代替递归
void dfs_stack(int s,vector<vector<int>> &g,vector<bool> &vis){
    stack<int> st;
    st.push(s);
    vis[s] = true;
    while(!st.empty()){
        int u = st.top();
        st.pop();
        for(int v : g[u]){
            if(!vis[v]){
                vis[v] = true;
                st.push(v);
            }
        }
    }
}
