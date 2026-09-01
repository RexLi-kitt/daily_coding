#include<vector>
#include<iostream>
#include<algorithm>
using namespace std;
// 更适合稠密图
int prim(int n,vector<vector<int>>& g){
    const int inf = 1e9;
    vector<int> minCost(n + 1,inf);
    vector<bool> inTree(n + 1,false);
    minCost[1] = 0;
    int total = 0;
    for(int i = 1;i <= n;i ++){
        int u = -1;
        for(int v = 1;v <= n;v ++){
            if(!inTree[v] && (u == -1 || minCost[v] < minCost[u])){
                u = v;
            }
        }
        if(minCost[u] == inf) return -1;
        inTree[u] = true;
        total += minCost[u];
        for (int v = 1; v <= n; v++)           // 用新点更新其他树外点
            if (!inTree[v] && g[u][v] < minCost[v])
                minCost[v] = g[u][v];
    }
    return total;
}
int main(){
    int n,m;
    cin >> n >> m;
    vector<vector<int>> g(n + 1,vector<int>(n + 1));
    for(int i = 1;i <= n;i ++){
        for(int j = 1;j <= n;j ++){
            g[i][j] = 1e9 + 5;
        }
    }
    for(int i = 0;i < m;i ++){
    int u,v,w;
    cin >> u >> v >> w;
        if(w < g[u][v]){              // 只保留这对点之间的最小权边
            g[u][v] = w;
            g[v][u] = w;
        }
    }
    int ans = prim(n,g);
    if(ans == -1){
        cout << "orz";
    }
    else{
        cout << ans;
    }
}