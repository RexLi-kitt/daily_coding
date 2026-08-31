#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using ll = long long;

const int MOD = 998244353;

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
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    sort(a.begin(), a.end());
    // 所有大小 >= 3的子集总数
    ll total = (modpow(2, n) - 1 - n - 1LL * n * (n - 1) / 2) % MOD;
    if (total < 0) total += MOD;
    int maxSum = a.back(); 
    vector<ll> dp(maxSum + 1, 0);
    dp[0] = 1;  // 空集
    ll bad = 0; // 不满足多边形条件的方案数（大小 >=3）
  
    for (int i = 0; i < n; ++i) {
        int x = a[i];
        //统计当前前缀中，和 <= x 的子集总数
        ll sum_leq = 0;
        for (int s = 0; s <= x && s <= maxSum; ++s) {
            sum_leq += dp[s];
            if (sum_leq > MOD) sum_leq -= MOD; // 保持取模
        }
        // 减去空集（1种）和单元素子集（i 种）
        ll invalid = (sum_leq - 1 - i) % MOD;
        if (invalid < 0) invalid += MOD;
        bad = (bad + invalid) % MOD;
        //将当前元素 x 加入背包
        for (int s = maxSum; s >= x; --s) {
            dp[s] = (dp[s] + dp[s - x]) % MOD;
        }
    }
    ll ans = (total - bad) % MOD;
    if (ans < 0) ans += MOD;
    cout << ans << endl;
}