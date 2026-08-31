#include <iostream>
using namespace std;

typedef long long ll;

const ll MOD = 998244353;

// 快速幂：a^b % MOD
ll mod_pow(ll a, ll b) {
    a %= MOD;
    ll res = 1;
    while (b > 0) {
        if (b & 1) res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}

// 计算单个质因子指数 k 的贡献
ll calc_term(ll k, ll n) {
    // term = (k+1)^n - 2*k^n + (k-1)^n
    ll term = (mod_pow(k + 1, n) - 2 * mod_pow(k, n) % MOD + mod_pow(k - 1, n)) % MOD;
    if (term < 0) term += MOD;
    return term;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int Q;
    cin >> Q;

    while (Q--) {
        ll x, y, n;
        cin >> x >> y >> n;

        // 题目保证 x | y，但为了防止意外仍保留判断
        if (y % x != 0) {
            cout << 0 << "\n";
            continue;
        }

        ll t = y / x;
        ll ans = 1;

        // 质因数分解 t
        for (ll p = 2; p <= t / p; ++p) {
            if (t % p == 0) {
                ll k = 0;
                while (t % p == 0) {
                    t /= p;
                    ++k;
                }
                ans = ans * calc_term(k, n) % MOD;
            }
        }

        // 处理遗留的大质数（指数 k=1）
        if (t > 1) {
            ans = ans * calc_term(1, n) % MOD;
        }

        cout << ans << "\n";
    }

    return 0;
}