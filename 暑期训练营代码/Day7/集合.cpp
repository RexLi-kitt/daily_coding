#include <bits/stdc++.h>
using namespace std;
using ull = unsigned long long; // 用无符号类型避免溢出问题

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<ull> a(n + 1); // 下标从1开始，方便对应元素值
    for (int i = 1; i <= n; ++i) cin >> a[i];

    vector<vector<int>> S(m), T(m);

    // 读入所有 S
    for (int i = 0; i < m; ++i) {
        int sz;
        cin >> sz;
        S[i].resize(sz);
        for (int j = 0; j < sz; ++j) cin >> S[i][j];
    }

    // 读入所有 T
    for (int i = 0; i < m; ++i) {
        int sz;
        cin >> sz;
        T[i].resize(sz);
        for (int j = 0; j < sz; ++j) cin >> T[i][j];
    }

    // 处理每组询问
    for (int i = 0; i < m; ++i) {
        bool real_equal = (S[i] == T[i]);          // 真实相等
        ull xorS = 0, xorT = 0;
        for (int x : S[i]) xorS ^= a[x];
        for (int x : T[i]) xorT ^= a[x];
        bool xor_equal = (xorS == xorT);

        cout << (real_equal == xor_equal ? "correct" : "wrong") << '\n';
    }

    return 0;
}