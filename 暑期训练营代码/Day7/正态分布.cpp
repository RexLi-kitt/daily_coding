#include <iostream>
using namespace std;

int main() {
    int k;
    cin >> k;                      // 查询个数
    while (k--) {
        int mu, sigma, n;
        cin >> mu >> sigma >> n;

        // 因为 sigma 是 100 的因子，所以 d = 100 / sigma 是整数
        int d = 100 / sigma;
        int val = (n - mu) * d;    // 即 z × 100，保证为整数

        int row = val / 10 + 1;    // 行号
        int col = val % 10 + 1;    // 列号

        cout << row << " " << col << endl;
    }
    return 0;
}