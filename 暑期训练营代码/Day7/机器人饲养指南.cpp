#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> A(m + 1);
    A[0] = 0;
    for (int i = 1; i <= m; ++i) {
        cin >> A[i];
    }

    // dp[j] = 投喂恰好 j 个苹果能获得的最大快乐值
    const ll NEG_INF = -1e18;
    vector<ll> dp(n + 1, NEG_INF);
    dp[0] = 0;

    for (int i = 1; i <= m; ++i) {
        for (int j = i; j <= n; ++j) {
            if (dp[j - i] != NEG_INF) {
                dp[j] = max(dp[j], dp[j - i] + A[i]);
            }
        }
    }

    // 题目保证投喂全部 n 个苹果，必然有解
    cout << dp[n] << "\n";

    return 0;
}