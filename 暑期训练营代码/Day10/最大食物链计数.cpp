#include <iostream>
#include <vector>
#include <queue>
using namespace std;

const int MOD = 80112002;
const int MAXN = 5005;

vector<int> graph[MAXN];
int indeg[MAXN], outdeg[MAXN];
int dp[MAXN];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    for (int i = 0; i < m; ++i) {
        int a, b;
        cin >> a >> b;
        graph[a].push_back(b);
        ++outdeg[a];
        ++indeg[b];
    }

    queue<int> q;
    for (int i = 1; i <= n; ++i) {
        if (indeg[i] == 0) {
            dp[i] = 1;
            q.push(i);
        }
    }

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : graph[u]) {
            dp[v] = (dp[v] + dp[u]) % MOD;
            if (--indeg[v] == 0) {
                q.push(v);
            }
        }
    }

    int ans = 0;
    for (int i = 1; i <= n; ++i) {
        if (outdeg[i] == 0) {
            ans = (ans + dp[i]) % MOD;
        }
    }

    cout << ans << '\n';
    return 0;
}