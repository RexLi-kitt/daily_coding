#include <cstdio>
using namespace std;
typedef long long ll;

const int MAXN = 500005;
int n, m;
ll tree[MAXN];  // 树状数组（维护差分数组）

// lowbit: 取二进制最低位 1 的值
inline int lowbit(int x) {
    return x & -x;
}

// 单点加：将位置 x 加上 k
void add(int x, ll k) {
    while (x <= n) {
        tree[x] += k;
        x += lowbit(x);
    }
}

// 前缀和：查询 [1, x] 的差分前缀和（即第 x 个数的值）
ll sum(int x) {
    ll ans = 0;
    while (x > 0) {
        ans += tree[x];
        x -= lowbit(x);
    }
    return ans;
}

int main() {
    scanf("%d%d", &n, &m);
    
    // 读入初始数组，建立差分树状数组
    // d[1] = a[1], d[i] = a[i] - a[i-1] (i > 1)
    ll prev = 0;  // a[i-1]
    for (int i = 1; i <= n; ++i) {
        ll val;
        scanf("%lld", &val);
        add(i, val - prev);  // d[i] = a[i] - a[i-1]
        prev = val;
    }
    
    // 处理操作
    while (m--) {
        int op;
        scanf("%d", &op);
        if (op == 1) {
            int x, y;
            ll k;
            scanf("%d%d%lld", &x, &y, &k);
            // 区间 [x, y] 加 k → 差分：d[x] += k, d[y+1] -= k
            add(x, k);
            add(y + 1, -k);
        } else {
            int x;
            scanf("%d", &x);
            // 单点查询：求差分前缀和
            printf("%lld\n", sum(x));
        }
    }
    
    return 0;
}