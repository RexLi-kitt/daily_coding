#include <bits/stdc++.h>
using namespace std;
using int64 = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int64 x;
    cin >> x;

    // 1. 求最短长度 n
    int64 n = 1;
    while (n * (n - 1) / 2 < x) ++n;

    int64 total = n * (n - 1) / 2;          // 完全逆序时的逆序对数
    int64 miss = total - x;                 // 需要减少的逆序对数
    int64 d = x - (n - 1) * (n - 2) / 2;    // 第一个元素贡献的最小逆序对数

    vector<int> B;
    B.reserve(n);

    if (miss <= n / 2) {
        // 构造 [1,1,2,2,...,miss,miss, miss+1, miss+2, ..., n-miss]
        for (int i = 1; i <= miss; ++i) {
            B.push_back(i);
            B.push_back(i);
        }
        for (int i = miss + 1; i <= n - miss; ++i) {
            B.push_back((int)i);
        }
    } else {
        // 构造 [1,1,2,2,...,d,d, d+1, d+2, ..., n-d-1, d+1]
        for (int i = 1; i <= d; ++i) {
            B.push_back(i);
            B.push_back(i);
        }
        for (int i = d + 1; i <= n - d - 1; ++i) {
            B.push_back((int)i);
        }
        B.push_back((int)(d + 1));
    }

    // 翻转得到最终数组 A
    reverse(B.begin(), B.end());

    cout << n << '\n';
    for (int i = 0; i < n; ++i) {
        if (i) cout << ' ';
        cout << B[i];
    }
    cout << '\n';

    return 0;
}