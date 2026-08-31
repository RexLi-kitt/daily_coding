#include <iostream>
#include <deque>
#include <vector>
using namespace std;

const int MAXN = 1000005;
int a[MAXN];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    deque<int> q_min, q_max;
    vector<int> ans_min, ans_max;

    for (int i = 0; i < n; i++) {
        // 维护单调递增队列（求最小值）
        while (!q_min.empty() && a[q_min.back()] >= a[i]) {
            q_min.pop_back();
        }
        q_min.push_back(i);
        // 移除滑出窗口的元素
        while (!q_min.empty() && q_min.front() <= i - k) {
            q_min.pop_front();
        }

        // 维护单调递减队列（求最大值）
        while (!q_max.empty() && a[q_max.back()] <= a[i]) {
            q_max.pop_back();
        }
        q_max.push_back(i);
        while (!q_max.empty() && q_max.front() <= i - k) {
            q_max.pop_front();
        }

        // 窗口形成后开始记录答案
        if (i >= k - 1) {
            ans_min.push_back(a[q_min.front()]);
            ans_max.push_back(a[q_max.front()]);
        }
    }

    // 输出最小值
    for (size_t i = 0; i < ans_min.size(); i++) {
        if (i > 0) cout << ' ';
        cout << ans_min[i];
    }
    cout << '\n';

    // 输出最大值
    for (size_t i = 0; i < ans_max.size(); i++) {
        if (i > 0) cout << ' ';
        cout << ans_max[i];
    }
    cout << '\n';

    return 0;
}