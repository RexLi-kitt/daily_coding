#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
using namespace std;

const int MAXN = 100005;

struct Edge {
    int to, w;
};

vector<Edge> graph[MAXN];
int out_deg[MAXN];
int in_deg[MAXN];
double prob[MAXN]; // P[u] = probability of reaching u from 1

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout << fixed << setprecision(2);

    int n, m;
    cin >> n >> m;

    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        graph[u].push_back({v, w});
        out_deg[u]++;
        in_deg[v]++;
    }

    // Topological sort (Kahn's algorithm)
    queue<int> q;
    q.push(1);
    prob[1] = 1.0;

    double expected = 0.0;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        if (out_deg[u] == 0) continue;

        for (const auto &e : graph[u]) {
            int v = e.to;
            double w = e.w;
            double p = prob[u] / out_deg[u];
            prob[v] += p;
            expected += p * w;
            if (--in_deg[v] == 0) {
                q.push(v);
            }
        }
    }

    cout << expected << "\n";
    return 0;
}