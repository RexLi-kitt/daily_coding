#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

const double PI = acos(-1.0);

// 叉积: (x1-x0, y1-y0) × (x2-x1, y2-y1)
// >0 左转, <0 右转, =0 共线
double cross(double x0, double y0, double x1, double y1, double x2, double y2) {
    return (x1 - x0) * (y2 - y1) - (x2 - x1) * (y1 - y0);
}

double dist(double x0, double y0, double x1, double y1) {
    return sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
}

int main() {
    int n, L;
    cin >> n >> L;
    
    vector<pair<double, double>> pts(n);
    for (int i = 0; i < n; i++) {
        cin >> pts[i].first >> pts[i].second;
    }
    
    // 按 x 排序，x 相同按 y 排序
    sort(pts.begin(), pts.end());
    
    vector<int> hull;
    
    // 下凸壳
    for (int i = 0; i < n; i++) {
        while (hull.size() >= 2) {
            int cur = hull.back();
            int prv = hull[hull.size() - 2];
            if (cross(pts[prv].first, pts[prv].second,
                      pts[cur].first, pts[cur].second,
                      pts[i].first, pts[i].second) <= 0) {
                hull.pop_back();
            } else {
                break;
            }
        }
        hull.push_back(i);
    }
    
    int lo = (int)hull.size();
    
    // 上凸壳
    for (int i = n - 2; i >= 0; i--) {
        while ((int)hull.size() > lo) {
            int cur = hull.back();
            int prv = hull[hull.size() - 2];
            if (cross(pts[prv].first, pts[prv].second,
                      pts[cur].first, pts[cur].second,
                      pts[i].first, pts[i].second) <= 0) {
                hull.pop_back();
            } else {
                break;
            }
        }
        hull.push_back(i);
    }
    // 移除重复的起点
    hull.pop_back();
    
    // 计算凸包周长
    double perimeter = 0.0;
    int m = (int)hull.size();
    for (int i = 0; i < m; i++) {
        int cur = hull[i];
        int nxt = hull[(i + 1) % m];
        perimeter += dist(pts[cur].first, pts[cur].second,
                          pts[nxt].first, pts[nxt].second);
    }
    
    // 加上圆弧部分: 2 * PI * L
    double ans = perimeter + 2.0 * PI * L;
    
    // 四舍五入到整数
    cout << (long long)round(ans) << endl;
    
    return 0;
}