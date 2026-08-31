#include <bits/stdc++.h>
using namespace std;

using ll = long long;

const ll MOD = 1000000007LL;

ll modpow(ll a, ll e) {
    ll r = 1;
    while (e) {
        if (e & 1) r = r * a % MOD;
        a = a * a % MOD;
        e >>= 1;
    }
    return r;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int S;
    cin >> S;

    vector<int> a(S);
    int maxVal = 0;
    for (int i = 0; i < S; i++) {
        cin >> a[i];
        maxVal = max(maxVal, a[i]);
    }

    int K = S - 2;          // 矩阵元素个数
    if (K <= 0) {
        cout << 0 << '\n';
        return 0;
    }

    // 频次数组，同时要能访问因子值
    int LIM = max(maxVal, K) + 1;
    vector<int> cnt(LIM, 0);
    for (int x : a) cnt[x]++;

    // 阶乘和阶乘逆元，最大需要到 S
    vector<ll> fact(S + 1), invfact(S + 1);
    fact[0] = 1;
    for (int i = 1; i <= S; i++) {
        fact[i] = fact[i - 1] * i % MOD;
    }

    invfact[S] = modpow(fact[S], MOD - 2);
    for (int i = S; i >= 1; i--) {
        invfact[i - 1] = invfact[i] * i % MOD;
    }

    // base = ∏ invfact[cnt[v]]
    ll base = 1;
    for (int i = 1; i < LIM; i++) {
        if (cnt[i]) {
            base = base * invfact[cnt[i]] % MOD;
        }
    }

    // all = K! * ∏ invfact[cnt[v]]
    ll all = fact[K] * base % MOD;

    ll ans = 0;

    auto add_pair = [&](int n, int m) {
        if (n == m) {
            int fn = cnt[n];
            if (fn >= 2) {
                // 修正系数：invfact[fn-2] / invfact[fn] = invfact[fn-2] * fact[fn]
                ll ratio = invfact[fn - 2] * fact[fn] % MOD;
                ans = (ans + all * ratio) % MOD;
            }
        } else {
            int fn = cnt[n], fm = cnt[m];
            if (fn >= 1 && fm >= 1) {
                ll ratio = invfact[fn - 1] * fact[fn] % MOD;
                ratio = ratio * invfact[fm - 1] % MOD * fact[fm] % MOD;
                ans = (ans + all * ratio) % MOD;
            }
        }
    };

    // 枚举 K 的因子对，注意 (n,m) 和 (m,n) 都算
    for (int d = 1; d * d <= K; d++) {
        if (K % d == 0) {
            int e = K / d;
            if (d == e) {
                add_pair(d, d);
            } else {
                add_pair(d, e);
                add_pair(e, d);
            }
        }
    }

    cout << ans % MOD << '\n';
    return 0;
}