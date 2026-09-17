#include <bits/stdc++.h>
using namespace std;

const int MAXN = 10005;

int n, m;
vector<int> graph[MAXN];
int color[MAXN]; // -1 未染色，0 / 1 表示两种颜色
int cnt[2];

bool dfs(int u, int c) {
    color[u] = c;
    cnt[c]++;

    for (int v : graph[u]) {
        if (color[v] == -1) {
            if (!dfs(v, c ^ 1)) {
                return false;
            }
        } else if (color[v] == c) {
            return false;
        }
    }

    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;

    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    memset(color, -1, sizeof(color));

    int ans = 0;

    // 图可能不连通，因此每个连通块都要单独 DFS
    for (int i = 1; i <= n; i++) {
        if (color[i] != -1) {
            continue;
        }

        cnt[0] = 0;
        cnt[1] = 0;

        if (!dfs(i, 0)) {
            cout << "Impossible\n";
            return 0;
        }

        ans += min(cnt[0], cnt[1]);
    }

    cout << ans << '\n';
    return 0;
}
