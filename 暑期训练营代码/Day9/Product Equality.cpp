#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long ull;
typedef __uint128_t u128;

// 第二个哈希的模数（大素数）
const ull MOD2 = 1000000007ULL;

// 把十进制大数字符串转成 (mod 2^64, mod MOD2) 双哈希
pair<ull, ull> hashNum(const string &s) {
    ull h1 = 0; // 自然溢出，即 mod 2^64
    ull h2 = 0;
    for (char c : s) {
        int d = c - '0';
        h1 = h1 * 10 + d;
        h2 = (h2 * 10 + d) % MOD2;
    }
    return {h1, h2};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<string> nums(n);
    vector<pair<ull, ull>> h(n);
    map<pair<ull, ull>, long long> cnt; // 哈希值 -> 作为 A_k 出现的次数

    for (int i = 0; i < n; i++) {
        cin >> nums[i];
        h[i] = hashNum(nums[i]);
        cnt[h[i]]++;
    }

    long long ans = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            ull p1 = h[i].first * h[j].first;                      // mod 2^64
            ull p2 = (ull)((u128)h[i].second * h[j].second % MOD2); // mod MOD2
            auto it = cnt.find({p1, p2});
            if (it != cnt.end()) ans += it->second;
        }
    }

    cout << ans << '\n';
    return 0;
}

