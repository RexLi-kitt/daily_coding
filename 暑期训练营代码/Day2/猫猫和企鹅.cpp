#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int main() {
    int n, d;
    cin >> n >> d;

    // 1. 构建邻接表（无向图）
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // 2. BFS 从节点 1 开始
    vector<int> dist(n + 1, -1);
    queue<int> q;
    dist[1] = 0;
    q.push(1);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : adj[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }

    // 3. 统计距离 ≤ d 的节点数
    int cnt = 0;
    for (int i = 2; i <= n; i++) {
        if (dist[i] != -1 && dist[i] <= d) {
            cnt++;
        }
    }
    cout << cnt;
    return 0;
}