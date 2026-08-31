#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, a;
    cin >> n >> a; // 读取点的数量n和正方形半边长a[reference:17]

    int m = 0; // 记录落在圆内的点数
    for (int i = 0; i < n; i++) {
        double x, y;
        cin >> x >> y; // 读取每个点的坐标[reference:18]
        // 判断点是否在圆内[reference:19]
        if (x * x + y * y <= a * a) {
            m++;
        }
    }

    // 计算π的近似值并保留6位小数输出[reference:20]
    printf("%.6f\n", 4.0 * m / n);
    return 0;
}