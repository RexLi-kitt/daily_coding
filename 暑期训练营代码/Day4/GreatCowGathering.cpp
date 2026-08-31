#include<iostream>
#include<algorithm>
using namespace std;
typedef long long ll;

const int N = 1e5 + 5;
const int M = 2e5 + 5;  // 双向边，需要 2*(N-1)

int n;
ll cow[N];           // 每个节点的奶牛数
int head[N], to[M], nxt[M];
ll weight[M];
int tot;

ll total_cows;       // 奶牛总数
ll sz[N];            // 子树奶牛总数
ll cost[N];          // 子树内所有奶牛走到该节点的总代价
ll ans[N];           // 以每个节点为集合点的答案

void add(int u, int v, ll w) {
    to[++tot] = v;
    weight[tot] = w;
    nxt[tot] = head[u];
    head[u] = tot;
}

// 第一次 DFS：自底向上计算 sz 和 cost
void dfs1(int u, int fa) {
    sz[u] = cow[u];
    cost[u] = 0;
    for (int i = head[u]; i; i = nxt[i]) {
        int v = to[i];
        if (v == fa) continue;
        dfs1(v, u);
        sz[u] += sz[v];
        cost[u] += cost[v] + sz[v] * weight[i];
    }
}

// 第二次 DFS：换根 DP
void dfs2(int u, int fa) {
    for (int i = head[u]; i; i = nxt[i]) {
        int v = to[i];
        if (v == fa) continue;
        // ans[v] = ans[u] - sz[v]*w + (total_cows - sz[v])*w
        ans[v] = ans[u] + (total_cows - 2 * sz[v]) * weight[i];
        dfs2(v, u);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> n;
    total_cows = 0;
    for (int i = 1; i <= n; i++) {
        cin >> cow[i];
        total_cows += cow[i];
    }
    
    for (int i = 1; i < n; i++) {
        int u, v; ll w;
        cin >> u >> v >> w;
        add(u, v, w);
        add(v, u, w);   // 双向边
    }
    
    // 第一次 DFS，以 1 为根
    dfs1(1, 0);
    ans[1] = cost[1];
    
    // 第二次 DFS 换根
    dfs2(1, 0);
    
    // 找最小答案
    ll res = ans[1];
    for (int i = 2; i <= n; i++) {
        res = min(res, ans[i]);
    }
    cout << res << '\n';
    
    return 0;
}