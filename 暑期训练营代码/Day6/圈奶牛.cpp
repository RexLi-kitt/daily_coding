#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
using namespace std;

double calc(double x0, double y0, double x1, double y1, double x2, double y2) {
    // 叉积 > 0: 左转, <= 0: 右转或共线
    return (x1 - x0) * (y2 - y1) - (x2 - x1) * (y1 - y0);
}

double calc_dist(double x0, double y0, double x1, double y1) {
    return sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
}

int main() {
    int n;
    cin >> n;
    if (n == 0) { printf("0.00\n"); return 0; }
    if (n == 1) { double x, y; cin >> x >> y; printf("0.00\n"); return 0; }
    if (n == 2) {
        double x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        printf("%.2f\n", 2.0 * sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
        return 0;
    }
    vector<pair<double, double>> data;
    for (int i = 0; i < n; i++) {
        double x, y;
        cin >> x >> y;
        data.push_back({x, y});
    }
    sort(data.begin(), data.end());

    vector<int> hull;
    
    // 下凸壳: i 从 0 到 n-1
    for (int i = 0; i < n; i++) {
        while (hull.size() >= 2) {
            int cur = hull.back();
            int prv = hull[hull.size() - 2];
            if (calc(data[prv].first, data[prv].second,
                     data[cur].first, data[cur].second,
                     data[i].first, data[i].second) <= 0) {
                hull.pop_back();
            } else {
                break;
            }
        }
        hull.push_back(i);
    }
    
    // 记录下凸壳大小，防止上凸壳弹掉下凸壳的点
    int lo = (int)hull.size();
    
    // 上凸壳: i 从 n-2 到 0
    for (int i = n - 2; i >= 0; i--) {
        while ((int)hull.size() > lo) {
            int cur = hull.back();
            int prv = hull[hull.size() - 2];
            if (calc(data[prv].first, data[prv].second,
                     data[cur].first, data[cur].second,
                     data[i].first, data[i].second) <= 0) {
                hull.pop_back();
            } else {
                break;
            }
        }
        hull.push_back(i);
    }
    // 最后一个点是重复的起点，移除
    hull.pop_back();

    // 计算周长
    double ans = 0;
    for (int i = 0; i < (int)hull.size(); i++) {
        int cur = hull[i];
        int nxt = hull[(i + 1) % hull.size()];
        ans += calc_dist(data[cur].first, data[cur].second,
                         data[nxt].first, data[nxt].second);
    }

    printf("%.2f\n", ans);
    return 0;
}