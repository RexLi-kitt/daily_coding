#include <iostream>
#include <vector>
using namespace std;

typedef long long ll;

const int MAXN = 100005;

ll a[MAXN];                // 原始数组
ll tree[MAXN << 2];        // 线段树，区间和
ll add_lazy[MAXN << 2];   // 加法懒标记
ll mul_lazy[MAXN << 2];   // 乘法懒标记

ll MOD;  // 模数

// 建树
void build(int node, int l, int r) {
    mul_lazy[node] = 1;    // 乘法标记初始化为 1
    add_lazy[node] = 0;    // 加法标记初始化为 0
    if (l == r) {
        tree[node] = a[l] % MOD;
        return;
    }
    int mid = (l + r) >> 1;
    build(node << 1, l, mid);
    build(node << 1 | 1, mid + 1, r);
    tree[node] = (tree[node << 1] + tree[node << 1 | 1]) % MOD;
}

// 应用标记到当前节点：先乘后加
// sum = sum * mul + add * len
void apply(int node, int l, int r, ll mul_val, ll add_val) {
    int len = r - l + 1;
    // 先乘后加，更新区间和
    tree[node] = (tree[node] * mul_val % MOD + add_val * len % MOD) % MOD;
    // 更新乘法标记：新的 mul = mul * mul_val
    mul_lazy[node] = mul_lazy[node] * mul_val % MOD;
    // 更新加法标记：新的 add = add * mul_val + add_val（因为先乘后加，原来的加法也要乘以 mul_val）
    add_lazy[node] = (add_lazy[node] * mul_val % MOD + add_val) % MOD;
}

// 下传懒标记
void push_down(int node, int l, int r) {
    if (mul_lazy[node] == 1 && add_lazy[node] == 0) return;
    int mid = (l + r) >> 1;
    int left = node << 1;
    int right = node << 1 | 1;

    apply(left, l, mid, mul_lazy[node], add_lazy[node]);
    apply(right, mid + 1, r, mul_lazy[node], add_lazy[node]);

    // 清除当前节点懒标记
    mul_lazy[node] = 1;
    add_lazy[node] = 0;
}

// 区间乘法 [ql, qr] *= k
void update_mul(int node, int l, int r, int ql, int qr, ll k) {
    if (ql <= l && r <= qr) {
        apply(node, l, r, k, 0);
        return;
    }
    push_down(node, l, r);
    int mid = (l + r) >> 1;
    if (ql <= mid) update_mul(node << 1, l, mid, ql, qr, k);
    if (qr > mid)  update_mul(node << 1 | 1, mid + 1, r, ql, qr, k);
    tree[node] = (tree[node << 1] + tree[node << 1 | 1]) % MOD;
}

// 区间加法 [ql, qr] += k
void update_add(int node, int l, int r, int ql, int qr, ll k) {
    if (ql <= l && r <= qr) {
        apply(node, l, r, 1, k);
        return;
    }
    push_down(node, l, r);
    int mid = (l + r) >> 1;
    if (ql <= mid) update_add(node << 1, l, mid, ql, qr, k);
    if (qr > mid)  update_add(node << 1 | 1, mid + 1, r, ql, qr, k);
    tree[node] = (tree[node << 1] + tree[node << 1 | 1]) % MOD;
}

// 区间查询 [ql, qr] 的和
ll query(int node, int l, int r, int ql, int qr) {
    if (ql <= l && r <= qr) {
        return tree[node];
    }
    push_down(node, l, r);
    int mid = (l + r) >> 1;
    ll ans = 0;
    if (ql <= mid) ans = (ans + query(node << 1, l, mid, ql, qr)) % MOD;
    if (qr > mid)  ans = (ans + query(node << 1 | 1, mid + 1, r, ql, qr)) % MOD;
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q >> MOD;
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
    }
    build(1, 1, n);

    while (q--) {
        int op, x, y;
        cin >> op >> x >> y;
        if (op == 1) {
            ll k;
            cin >> k;
            update_mul(1, 1, n, x, y, k % MOD);
        } else if (op == 2) {
            ll k;
            cin >> k;
            update_add(1, 1, n, x, y, k % MOD);
        } else {  // op == 3
            cout << query(1, 1, n, x, y) << '\n';
        }
    }
    return 0;
}