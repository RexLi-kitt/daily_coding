#include <iostream>
using namespace std;

int main() {
    int b, c, l, r;
    cin >> b >> c >> l >> r;

    // 找到 >= l 的最小偶数
    int x = l;
    if (x % 2 != 0) x++;

    int sum = 0;
    for (int i = x; i <= r; i += 2) {
        sum += i * i + b * i + c;
    }

    int ans = 2 * sum;
    cout << ans << endl;

    return 0;
}