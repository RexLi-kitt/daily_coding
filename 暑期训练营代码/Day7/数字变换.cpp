#include <bits/stdc++.h>
using namespace std;

int f(int x, int k) {
    return ((x * x + k * k) % 8) ^ k;
}

int rev_g(int y, int k) {
    int A = (y >> 6) & 7;
    int B = (y >> 3) & 7;
    int C = y & 7;
    int b = A;
    int c = B ^ f(b, k);
    int a = C ^ f(c, k);
    return (a << 6) | (b << 3) | c;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> k(m);
    for (int i = 0; i < m; ++i) cin >> k[i];

    vector<int> out(n);
    for (int i = 0; i < n; ++i) cin >> out[i];

    // 预处理：对每个可能的输出值 (0~511) 计算对应的输入
    int inv[512];
    for (int y = 0; y < 512; ++y) {
        int cur = y;
        for (int j = m - 1; j >= 0; --j) {
            cur = rev_g(cur, k[j]);
        }
        inv[y] = cur;
    }

    for (int i = 0; i < n; ++i) {
        cout << inv[out[i]] << (i + 1 == n ? '\n' : ' ');
    }

    return 0;
}