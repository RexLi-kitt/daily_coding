#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

struct DSU {
    vector<int> f, sz;
    DSU(int n = 0) { init(n); }
    void init(int n) {
        f.resize(n + 1); sz.assign(n + 1, 1);
        iota(f.begin(), f.end(), 0);
    }
    int find(int x) { return f[x] == x ? x : f[x] = find(f[x]); }
    bool merge(int x, int y) {
        x = find(x); y = find(y);
        if (x == y) return false;
        if (sz[x] < sz[y]) swap(x, y);
        f[y] = x; sz[x] += sz[y];
        return true;
    }
};

struct Edge { int u, v, town; ll w; };  // town=0 为原图树边，否则为乡镇编号

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    vector<Edge> roads(m);
    for (int i = 0; i < m; ++i) cin >> roads[i].u >> roads[i].v >> roads[i].w;

    vector<ll> c(k + 1);
    vector<vector<ll>> a(k + 1, vector<ll>(n + 1));
    for (int j = 1; j <= k; ++j) {
        cin >> c[j];
        for (int i = 1; i <= n; ++i) cin >> a[j][i];
    }

    // 1) 原图 MST 缩边：只保留 n-1 条树边
    sort(roads.begin(), roads.end(), [](const Edge& x, const Edge& y) { return x.w < y.w; });
    DSU dsu(n + k);
    vector<Edge> cand;
    ll baseCost = 0;
    int cnt = 0;
    for (const auto& e : roads) {
        if (dsu.merge(e.u, e.v)) {
            cand.push_back({e.u, e.v, 0, e.w});
            baseCost += e.w;
            if (++cnt == n - 1) break;
        }
    }

    // 2) 每个乡镇是独立节点 n+j，预先把星形边全部加入候选集
    for (int j = 1; j <= k; ++j)
        for (int i = 1; i <= n; ++i)
            cand.push_back({n + j, i, j, a[j][i]});

    // 3) 所有候选边全局排序一次，之后每个子集线性扫描即可
    sort(cand.begin(), cand.end(), [](const Edge& x, const Edge& y) { return x.w < y.w; });

    vector<ll> C(1 << k, 0);  // C[mask]：子集改造费（lowbit 递推）
    for (int mask = 1; mask < (1 << k); ++mask) {
        int lb = mask & -mask;
        int j = __builtin_ctz(mask) + 1;
        C[mask] = C[mask ^ lb] + c[j];
    }

    ll ans = baseCost;  // 不选任何乡镇

    for (int mask = 1; mask < (1 << k); ++mask) {
        DSU d2(n + k);
        ll cost = C[mask];
        int need = n + __builtin_popcount(mask) - 1;
        int got = 0;
        for (const auto& e : cand) {
            if (e.town && !(mask & (1 << (e.town - 1)))) continue;  // 未选乡镇的边跳过
            if (d2.merge(e.u, e.v)) {
                cost += e.w;
                if (++got == need) break;  // 全部连通即结束
            }
        }
        ans = min(ans, cost);
    }

    cout << ans << '\n';
    return 0;
}