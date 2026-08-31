#include <iostream>
#include <algorithm>
using namespace std;
using ll = long long;

ll n, k, m;

// 检查 R 个机器人是否能撑过 m 天
bool check(ll R) {
    ll t = n;
    for (ll day = 1; day <= m; day++) {
        if (t == 0) {
            // 没有苹果了，但还需要喂 R 个机器人
            return R == 0;
        }
        // 坏苹果数 = ceil(t * k / 100)
        ll bad = (t * k + 99) / 100;
        ll good = t - bad;
        if (good < R) return false;
        t = good - R;
    }
    return true;
}

int main() {
    cin >> n >> k >> m;

    // 特判 k == 100：所有苹果都会变质，只能入住 0 个机器人
    if (k == 100) {
        cout << 0 << endl;
        return 0;
    }

    // 二分答案：R 的范围 [0, n]
    ll lo = 0, hi = n;
    ll ans = 0;
    while (lo <= hi) {
        ll mid = lo + (hi - lo) / 2;
        if (check(mid)) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    cout << ans << endl;
    return 0;
}