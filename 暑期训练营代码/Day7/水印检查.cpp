#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, L;
    cin >> n >> L;

    vector<vector<int>> A(n, vector<int>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            cin >> A[i][j];

    // 水印模板 (1: 白, 0: 黑)
    vector<string> pattern = {
        "111111111",
        "100100101",
        "100111110",
        "100001100",
        "111111100"
    };

    // 预处理白色和黑色的相对坐标
    vector<pair<int,int>> white, black;
    for (int r = 0; r < 5; ++r) {
        for (int c = 0; c < 9; ++c) {
            if (pattern[r][c] == '1') white.push_back({r, c});
            else black.push_back({r, c});
        }
    }

    vector<pair<ll, ll>> intervals; // 存储所有可行区间 [l, r]

    // 遍历所有 5x9 子区域
    for (int i = 0; i + 5 <= n; ++i) {
        for (int j = 0; j + 9 <= n; ++j) {
            int min_white = INT_MAX;
            int max_black = INT_MIN;

            for (auto &p : white) {
                int r = i + p.first, c = j + p.second;
                min_white = min(min_white, A[r][c]);
            }
            for (auto &p : black) {
                int r = i + p.first, c = j + p.second;
                max_black = max(max_black, A[r][c]);
            }

            if (max_black < min_white) {
                ll l = max_black + 1LL;
                ll r = min_white;
                // 限制在 [0, L-1] 范围内
                l = max(l, 0LL);
                r = min(r, (ll)L - 1);
                if (l <= r) intervals.push_back({l, r});
            }
        }
    }

    // 如果没有可行区间，直接结束
    if (intervals.empty()) return 0;

    // 区间排序 + 合并
    sort(intervals.begin(), intervals.end());
    vector<pair<ll, ll>> merged;
    merged.push_back(intervals[0]);
    for (int i = 1; i < intervals.size(); ++i) {
        auto &last = merged.back();
        ll l = intervals[i].first, r = intervals[i].second;
        if (l <= last.second + 1) {
            // 可以合并（注意是整数，相邻也合并）
            last.second = max(last.second, r);
        } else {
            merged.push_back({l, r});
        }
    }

    // 输出所有合并区间内的每个整数
    for (auto &seg : merged) {
        for (ll k = seg.first; k <= seg.second; ++k) {
            cout << k << '\n';
        }
    }

    return 0;
}