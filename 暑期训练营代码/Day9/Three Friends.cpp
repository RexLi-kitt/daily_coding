#include <iostream>
#include <vector>
#include <string>
using namespace std;

using ull = unsigned long long;

// 双哈希参数
const int base1 = 131, base2 = 13331;
const int mod1 = 1000000007, mod2 = 1000000009;

vector<ull> hash1, hash2, pw1, pw2;

// 获取子串 [pos, pos+len-1] 的两个哈希值（打包成 ull 对）
pair<ull, ull> get(int pos, int len) {
    if (len == 0) return {0ULL, 0ULL};
    if (pos == 0) return {hash1[len - 1], hash2[len - 1]};
    ull h1 = (hash1[pos + len - 1] - hash1[pos - 1] * pw1[len] % mod1 + mod1) % mod1;
    ull h2 = (hash2[pos + len - 1] - hash2[pos - 1] * pw2[len] % mod2 + mod2) % mod2;
    return {h1, h2};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int n;
    cin >> n;
    if (n % 2 == 0) {
        cout << "NOT POSSIBLE\n";
        return 0;
    }

    string data;
    cin >> data;

    // 初始化双哈希
    hash1.resize(data.size());
    hash2.resize(data.size());
    pw1.resize(data.size());
    pw2.resize(data.size());

    hash1[0] = data[0] % mod1;
    hash2[0] = data[0] % mod2;
    pw1[0] = pw2[0] = 1;

    for (int i = 1; i < (int)data.size(); ++i) {
        hash1[i] = (hash1[i - 1] * base1 + data[i]) % mod1;
        hash2[i] = (hash2[i - 1] * base2 + data[i]) % mod2;
        pw1[i] = (pw1[i - 1] * base1) % mod1;
        pw2[i] = (pw2[i - 1] * base2) % mod2;
    }

    int half = n / 2;          // S 的长度
    int start_S = -1;          // 记录第一个解中 S 在原串的起始位置
    bool found = false;

    for (int i = 0; i < n; ++i) {
        pair<ull, ull> left, right;
        int cur_start;         // 当前解对应的 S 起始位置

        if (i == half) {
            left = get(0, half);
            right = get(i + 1, half);
            cur_start = 0;
        } else if (i < half) {
            // 左半部分拼接
            auto part1 = get(0, i);
            auto part2 = get(i + 1, half - i);
            left.first  = (part1.first * pw1[half - i] + part2.first) % mod1;
            left.second = (part1.second * pw2[half - i] + part2.second) % mod2;
            right = get(half + 1, half);
            cur_start = half + 1;
        } else { // i > half
            left = get(0, half);
            auto part1 = get(half, i - half);
            auto part2 = get(i + 1, n - 1 - i);
            right.first  = (part1.first * pw1[n - 1 - i] + part2.first) % mod1;
            right.second = (part1.second * pw2[n - 1 - i] + part2.second) % mod2;
            cur_start = 0;
        }

        if (left == right) {
            if (!found) {
                found = true;
                start_S = cur_start;
            } else {
                // 之前已经找到过解，比较两个 S 是否相同
                if (start_S != cur_start) {
                    // 不同起始位置，需要比较内容
                    if (data.compare(0, half, data, cur_start, half) != 0) {
                        cout << "NOT UNIQUE\n";
                        return 0;
                    }
                }
                // 内容相同则忽略，仍为唯一解
            }
        }
    }

    if (!found) {
        cout << "NOT POSSIBLE\n";
    } else {
        cout << data.substr(start_S, half) << '\n';
    }

    return 0;
}