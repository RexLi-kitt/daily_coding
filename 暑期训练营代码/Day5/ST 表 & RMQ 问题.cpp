#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 100005;
const int LOG = 18; // log2(100000) ≈ 16.6, 18 足够

int st[MAXN][LOG];
int lg[MAXN];

// 快速读入
inline int read() {
    int x = 0, f = 1;
    char ch = getchar();
    while (ch < '0' || ch > '9') {
        if (ch == '-') f = -1;
        ch = getchar();
    }
    while (ch >= '0' && ch <= '9') {
        x = x * 10 + ch - 48;
        ch = getchar();
    }
    return x * f;
}

// 快速输出（非负整数）
inline void write(int x) {
    if (x == 0) {
        putchar('0');
        return;
    }
    if (x < 0) {
        putchar('-');
        x = -x;
    }
    char buf[12];
    int pos = 0;
    while (x) {
        buf[pos++] = x % 10 + '0';
        x /= 10;
    }
    while (pos--) {
        putchar(buf[pos]);
    }
}

int main() {
    int N = read();
    int M = read();

    // 预处理对数表
    lg[1] = 0;
    for (int i = 2; i <= N; i++) {
        lg[i] = lg[i >> 1] + 1;
    }

    // 读入数组，初始化 st[i][0]
    for (int i = 1; i <= N; i++) {
        st[i][0] = read();
    }

    // 预处理 ST 表
    for (int j = 1; j < LOG; j++) {
        for (int i = 1; i + (1 << j) - 1 <= N; i++) {
            st[i][j] = max(st[i][j - 1], st[i + (1 << (j - 1))][j - 1]);
        }
    }

    // 处理查询
    while (M--) {
        int l = read();
        int r = read();
        int k = lg[r - l + 1];
        int ans = max(st[l][k], st[r - (1 << k) + 1][k]);
        write(ans);
        putchar('\n');
    }

    return 0;
}