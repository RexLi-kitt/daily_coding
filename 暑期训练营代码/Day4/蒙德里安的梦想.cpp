#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int n, m;

// 根据上一行状态 up，构造当前行所有合法状态 cur，累加到 nxt[cur]
void dfs(int up, int j, int cur, vector<ll> &dp, vector<ll> &nxt) {
    if (j == m) {
        nxt[cur] += dp[up];
        return;
    }
    // 上一行竖着放下来的骨牌占了这一格，当前行只能空着
    if (up >> j & 1) {
        dfs(up, j + 1, cur, dp, nxt);
    } else {
        // 选择竖着放，当前行这一位被占用
        dfs(up, j + 1, cur | (1 << j), dp, nxt);
        // 选择横着放，需要右边一格也空着
        if (j + 1 < m && !(up >> (j + 1) & 1)) {
            dfs(up, j + 2, cur, dp, nxt);
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    while (cin >> n >> m && (n || m)) {
        // 状压宽度取较小的那个
        if (n < m) swap(n, m);
        vector<ll> dp(1 << m), nxt(1 << m);
        dp[0] = 1;
        for (int i = 0; i < n; ++i) {
            fill(nxt.begin(), nxt.end(), 0);
            for (int up = 0; up < (1 << m); ++up) {
                if (dp[up]) dfs(up, 0, 0, dp, nxt);
            }
            dp.swap(nxt);
        }
        cout << dp[0] << '\n';
    }
    return 0;
}