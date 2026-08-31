#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

int n;
string words[22];
int used[22];           // 每个单词已使用的次数
int overlap[22][22];    // overlap[i][j]: word[i] 后缀与 word[j] 前缀的最小重叠长度
int ans = 0;

// 计算 word[a] 后接 word[b] 的最小重叠长度，不能重叠则返回 0
int calcOverlap(const string &a, const string &b) {
    int la = a.size(), lb = b.size();
    // 重叠长度 k：1 ≤ k < min(la, lb)
    for (int k = 1; k < min(la, lb); k++) {
        if (a.substr(la - k) == b.substr(0, k)) {
            return k;
        }
    }
    return 0;
}

// 预处理所有单词对的重叠长度
void precompute() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            overlap[i][j] = calcOverlap(words[i], words[j]);
        }
    }
}

// DFS 回溯
// last: 当前龙末尾单词的索引
// curLen: 当前龙的总长度
void dfs(int last, int curLen) {
    ans = max(ans, curLen);
    for (int j = 0; j < n; j++) {
        if (used[j] < 2 && overlap[last][j] > 0) {
            used[j]++;
            dfs(j, curLen + (int)words[j].size() - overlap[last][j]);
            used[j]--;
        }
    }
}

int main() {
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> words[i];
    }
    char startChar;
    cin >> startChar;

    precompute();

    for (int i = 0; i < n; i++) {
        if (words[i][0] == startChar) {
            used[i] = 1;
            dfs(i, words[i].size());
            used[i] = 0;
        }
    }

    cout << ans << endl;
    return 0;
}