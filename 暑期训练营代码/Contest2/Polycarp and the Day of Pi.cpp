#include <iostream>
#include<string>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string pi = "314159265358979323846264338327";

    int t;
    cin >> t;
    while (t--) {
        string s;
        cin >> s;
        int ans = 0;
        while (ans < (int)s.size() && ans < (int)pi.size() && s[ans] == pi[ans]) {
            ++ans;
        }
        cout << ans << "\n";
    }
    return 0;
}