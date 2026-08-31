#include <cstdio>
using namespace std;
typedef long long ll;

const int MAXN = 500005;
int n, m;
ll tree[MAXN];  // 树状数组

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

// 前缀和：查询 [1, x] 的和
ll sum(int x) {
    ll ans = 0;
    while (x > 0) {
        ans += tree[x];
        x -= lowbit(x);
    }
    return ans;
}

// 区间和：查询 [l, r] 的和
ll query(int l, int r) {
    return sum(r) - sum(l - 1);
}

int main() {
    scanf("%d%d", &n, &m);
    
    // 读入初始数组，建树
    for (int i = 1; i <= n; ++i) {
        ll val;
        scanf("%lld", &val);
        add(i, val);
    }
    
    // 处理操作
    while (m--) {
        int op, x;
        ll k;
        scanf("%d%d", &op, &x);
        if (op == 1) {
            scanf("%lld", &k);
            add(x, k);
        } else {
            int y;
            scanf("%d", &y);
            printf("%lld\n", query(x, y));
        }
    }
    
    return 0;
}