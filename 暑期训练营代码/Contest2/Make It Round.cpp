#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll count_factor(ll x, int p) {
    ll cnt = 0;
    while (x % p == 0) {
        cnt++;
        x /= p;
    }
    return cnt;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        ll n, m;
        cin >> n >> m;
        ll c2 = count_factor(n, 2);
        ll c5 = count_factor(n, 5);
        ll cur_zero = min(c2, c5);
        ll ans = n * m; // 默认最大价格
        // 枚举目标零数 x，从高到低
        for (int x = 18; x > cur_zero; --x) {
            ll need2 = max(0LL, (ll)x - c2);
            ll need5 = max(0LL, (ll)x - c5);

            ll base = 1;
            for (int i = 0; i < need2; ++i) base *= 2;
            for (int i = 0; i < need5; ++i) base *= 5;

            if (base <= m) {
                ll k = (m / base) * base; // 最大倍数
                ans = n * k;
                break; // 第一个可行就是最大零数
            }
        }

        cout << ans << '\n';
    }
    return 0;
}