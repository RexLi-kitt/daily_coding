    #include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    string pattern = "Yes";

    while (t--) {
        string s;
        cin >> s;

        bool ok = false;
        // 枚举起始相位 0,1,2
        for (int start = 0; start < 3; start++) {
            bool match = true;
            for (int i = 0; i < (int)s.size(); i++) {
                if (s[i] != pattern[(start + i) % 3]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                ok = true;
                break;
            }
        }

        cout << (ok ? "YES" : "NO") << '\n';
    }

    return 0;
}