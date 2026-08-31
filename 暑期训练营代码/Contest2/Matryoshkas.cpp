#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
using namespace std;

int main() {
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<int> data(n);
        for (int i = 0; i < n; i++) {
            cin >> data[i];
        }
        sort(data.begin(), data.end());
        map<int, int> tails;
        int ans = 0;
        for (int x : data) {
            if (tails[x - 1] > 0) {
                // 有一组的末尾是 x-1，可以把 x 接进去，该组末尾变成 x
                tails[x - 1]--;
                tails[x]++;
            } else {
                // 没有能接的组，新建一组
                tails[x]++;
                ans++;
            }
        }
        cout << ans << endl;
    }
    return 0;
}