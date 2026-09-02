#include <iostream>
#include <vector>
#include <queue>
using namespace std;

const int INF = 0x3f3f3f3f;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, s;
    cin >> n >> m >> s;

    vector<vector<pair<int,int>>> g(n + 1);   // g[u] = {v, w} 列表
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        g[u].push_back({v, w});   // 平行边直接全存，不用取 min
    }

    vector<int> dist(n + 1, INF);
    vector<bool> done(n + 1, false);
    dist[s] = 0;

    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;
    pq.push({0, s});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (done[u]) continue;        // 过期记录，跳过
        done[u] = true;

        for (auto [v, w] : g[u]) {    // 遍历 u 的所有出边（含平行边）
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }

    for (int i = 1; i <= n; i++)
        cout << (dist[i] == INF ?2147483647  : dist[i]) << ' ';
    cout << '\n';
    return 0;
}
