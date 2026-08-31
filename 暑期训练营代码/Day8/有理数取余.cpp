#include <iostream>
#include <string>
using namespace std;

typedef long long ll;
const ll MOD = 19260817;

// 将字符串表示的大整数对 MOD 取模
ll str_mod(const string& s) {
    ll res = 0;
    for (char c : s) {
        res = (res * 10 + (c - '0')) % MOD;
    }
    return res;
}

// 快速幂：计算 a^b % mod
ll mod_pow(ll a, ll b, ll mod) {
    ll ans = 1;
    a %= mod;
    while (b > 0) {
        if (b & 1) ans = ans * a % mod;
        a = a * a % mod;
        b >>= 1;
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string a_str, b_str;
    cin >> a_str >> b_str;

    ll a_mod = str_mod(a_str);
    ll b_mod = str_mod(b_str);

    if (b_mod == 0) {
        cout << "Angry!\n";
    } else {
        ll inv_b = mod_pow(b_mod, MOD - 2, MOD);   // 费马小定理求逆元
        ll ans = a_mod * inv_b % MOD;
        cout << ans << '\n';
    }

    return 0;
}