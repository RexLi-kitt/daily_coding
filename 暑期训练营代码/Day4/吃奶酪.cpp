#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
using namespace std;

const double INF = 1e18;  // 足够大

double dist(double x0, double y0, double x1, double y1) {
    double dx = x1 - x0, dy = y1 - y0;
    return sqrt(dx * dx + dy * dy);
}

int main() {
    int n;
    cin >> n;

    // 存储坐标，原点在 0
    vector<vector<double>> graph(n + 1, vector<double>(2));
    graph[0][0] = 0.0;
    graph[0][1] = 0.0;
    for (int i = 1; i <= n; i++) {
        cin >> graph[i][0] >> graph[i][1];
    }

    int total = 1 << (n + 1);  // 总状态数

    // 【核心修正】用 vector 在堆上分配，安全且初始化方便
    vector<vector<double>> dp(total, vector<double>(n + 1, INF));

    // 起点：只访问了原点（0号点），停留在原点
    dp[1 << 0][0] = 0.0;

    for (int mask = 0; mask < total; mask++) {
        for (int i = 0; i <= n; i++) {
            if (!(mask & (1 << i))) continue;
            // 【修正】用 >= INF/2 判断无穷大
            if (dp[mask][i] >= INF / 2) continue;

            for (int j = 0; j <= n; j++) {
                if (mask & (1 << j)) continue;
                int new_mask = mask | (1 << j);
                double cost = dp[mask][i] + dist(graph[i][0], graph[i][1], graph[j][0], graph[j][1]);
                if (cost < dp[new_mask][j]) {
                    dp[new_mask][j] = cost;
                }
            }
        }
    }

    double ans = INF;
    for (int i = 0; i <= n; i++) {
        ans = min(ans, dp[total - 1][i]);
    }

    // 【修正】同样用 >= INF/2 判断
    if (ans >= INF / 2) {
        cout << -1 << endl;
    } else {
        cout << fixed << setprecision(2) << ans << endl;
    }

    return 0;
}