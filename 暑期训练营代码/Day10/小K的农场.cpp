#include <bits/stdc++.h>
using namespace std;

const int MAXN = 5005;
const int MAXM = 20005; // 每种类型最多两条边，加上超级源点的 n 条边

struct Edge {
    int to, w, nxt;
} edge[MAXM];
int head[MAXN], tot = 0;

void add_edge(int u, int v, int w) {
    edge[++tot] = {v, w, head[u]};
    head[u] = tot;
}

int n, m;
int dis[MAXN], cnt[MAXN];
bool inq[MAXN];

bool spfa() {
    memset(dis, 0x3f, sizeof(dis));
    memset(cnt, 0, sizeof(cnt));
    memset(inq, 0, sizeof(inq));
    
    queue<int> q;
    // 超级源点 0
    dis[0] = 0;
    inq[0] = true;
    q.push(0);
    cnt[0] = 1;
    
    while (!q.empty()) {
        int u = q.front(); q.pop();
        inq[u] = false;
        for (int i = head[u]; i; i = edge[i].nxt) {
            int v = edge[i].to;
            int w = edge[i].w;
            if (dis[v] > dis[u] + w) {
                dis[v] = dis[u] + w;
                if (!inq[v]) {
                    q.push(v);
                    inq[v] = true;
                    cnt[v]++;
                    if (cnt[v] >= n + 1) { // 包括超级源点共 n+1 个点
                        return false; // 存在负环
                    }
                }
            }
        }
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> n >> m;
    
    // 超级源点 0 连接所有点
    for (int i = 1; i <= n; i++) {
        add_edge(0, i, 0);
    }
    
    for (int i = 1; i <= m; i++) {
        int op; cin >> op;
        if (op == 1) {
            int a, b, c; cin >> a >> b >> c;
            // x_a - x_b >= c  →  x_b - x_a <= -c  →  add_edge(a, b, -c)
            add_edge(a, b, -c);
        } else if (op == 2) {
            int a, b, c; cin >> a >> b >> c;
            // x_a - x_b <= c  →  add_edge(b, a, c)
            add_edge(b, a, c);
        } else if (op == 3) {
            int a, b; cin >> a >> b;
            // x_a == x_b  →  x_a - x_b <= 0, x_b - x_a <= 0
            add_edge(a, b, 0);
            add_edge(b, a, 0);
        }
    }
    
    if (spfa()) cout << "Yes\n";
    else cout << "No\n";
    
    return 0;
}