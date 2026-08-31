#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <climits>
#include <iomanip>
using namespace std;
using ll = long long;

struct Point {
    ll x, y;
};

// Squared distance
ll dist2(const Point& a, const Point& b) {
    ll dx = a.x - b.x;
    ll dy = a.y - b.y;
    return dx * dx + dy * dy;
}

// Classic divide and conquer
// pts sorted by x
ll closest_pair(vector<Point>& pts, int l, int r) {
    if (l >= r) return LLONG_MAX;
    
    int mid = (l + r) >> 1;
    ll mid_x = pts[mid].x;
    
    ll d = min(closest_pair(pts, l, mid), closest_pair(pts, mid + 1, r));
    
    // Merge by y: in-place merge the two halves sorted by y
    // We use temporary arrays for the merge
    static vector<Point> tmp;
    if ((int)tmp.size() < r - l + 1) tmp.resize(r - l + 1);
    
    int i = l, j = mid + 1, k = 0;
    while (i <= mid && j <= r) {
        if (pts[i].y <= pts[j].y) tmp[k++] = pts[i++];
        else tmp[k++] = pts[j++];
    }
    while (i <= mid) tmp[k++] = pts[i++];
    while (j <= r) tmp[k++] = pts[j++];
    for (int t = 0; t < k; t++) pts[l + t] = tmp[t];
    
    vector<int> strip;
    for (int t = l; t <= r; t++) {
        ll dx = pts[t].x - mid_x;
        if (dx * dx < d) {
            strip.push_back(t);
        }
    }
    
    for (int t = 0; t < (int)strip.size(); t++) {
        int idx_a = strip[t];
        for (int s = t + 1; s < (int)strip.size() && s <= t + 7; s++) {
            int idx_b = strip[s];
            ll dy = pts[idx_b].y - pts[idx_a].y;
            if (dy * dy >= d) break; // points are sorted by y
            d = min(d, dist2(pts[idx_a], pts[idx_b]));
        }
    }
    
    return d;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    vector<Point> pts(n);
    for (int i = 0; i < n; i++) {
        cin >> pts[i].x >> pts[i].y;
    }
    
    sort(pts.begin(), pts.end(), [](const Point& a, const Point& b) {
        return a.x < b.x;
    });
    
    ll d2 = closest_pair(pts, 0, n - 1);
    double ans = sqrt((long double)d2);
    cout << fixed << setprecision(4) << ans << "\n";
    
    return 0;
}