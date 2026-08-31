#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const int NEG = -1e9;

int N, M;
vector<vector<pair<int, int>>> children; // children[u]: (子结点, 边费用)
vector<int> money;                        // money[i]: 用户 i 愿意支付的钱
vector<vector<int>> dp;                   // dp[u][j]: 子树 u 中恰好服务 j 个用户的最大利润
vector<int> siz;                          // siz[u]: 子树 u 中的用户数

void dfs(int u) {
    dp[u][0] = 0;
    if (u > N - M) {              // 用户终端（叶子）
        siz[u] = 1;
        dp[u][1] = money[u];
        return;
    }
    siz[u] = 0;
    for (size_t i = 0; i < children[u].size(); i++) {
        int v = children[u][i].first;
        int c = children[u][i].second;
        dfs(v);
        int cap = min(siz[u] + siz[v], M);
        // 倒序枚举, 保证 dp[u][j-k] 是合并 v 之前的旧值
        for (int j = cap; j >= 1; j--) {
            for (int k = 1; k <= min(j, siz[v]); k++) {
                if (dp[u][j - k] != NEG && dp[v][k] != NEG)
                    dp[u][j] = max(dp[u][j], dp[u][j - k] + dp[v][k] - c);
            }
        }
        siz[u] += siz[v];
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> M;
    children.assign(N + 1, {});
    money.assign(N + 1, 0);
    dp.assign(N + 1, vector<int>(M + 1, NEG));
    siz.assign(N + 1, 0);

    for (int i = 1; i <= N - M; i++) {   // 转播站的下接结点与费用
        int K;
        cin >> K;
        while (K--) {
            int A, C;
            cin >> A >> C;
            children[i].push_back(make_pair(A, C));
        }
    }
    for (int i = N - M + 1; i <= N; i++) // 用户的支付钱数
        cin >> money[i];

    dfs(1);

    int ans = 0;
    for (int j = M; j >= 1; j--) {
        if (dp[1][j] >= 0) {
            ans = j;
            break;
        }
    }
    cout << ans << '\n';
    return 0;
}