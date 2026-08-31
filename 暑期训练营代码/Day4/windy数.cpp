#include <iostream>
#include <cstring>
#include <cmath>
using namespace std;

// dp[pos][pre]: 当前处理到第pos位，上一位数字是pre时，后续位能构成的Windy数个数
// limit: 当前位是否受原数上界限制
// isNum: 前面是否已经填了非零数字（即是否已经脱离前导零）
int dp[15][10];

// 将数字按位拆分
int digit[15];
int len;

// 记忆化搜索(pos: 当前位下标，pre: 上一位数字，limit: 是否受限，isNum: 是否已开始填数)
int dfs(int pos, int pre, bool limit, bool isNum) {
    if (pos == 0) return isNum; // 到达末位，如果已经填过数字则贡献1
    
    // 记忆化：只有不受限且已经开始填数时才可以使用缓存
    if (!limit && isNum && dp[pos][pre] != -1)
        return dp[pos][pre];
    
    int up = limit ? digit[pos] : 9; // 当前位能填的上界
    int res = 0;
    
    for (int d = 0; d <= up; d++) {
        if (!isNum) {
            // 还没开始填数（前面都是前导零）
            // 如果d==0，继续维持isNum=false；否则pre变为d，isNum变为true
            res += dfs(pos - 1, d, limit && d == up, d != 0);
        } else {
            // 已开始填数，需要满足相邻差≥2
            if (abs(d - pre) >= 2) {
                res += dfs(pos - 1, d, limit && d == up, true);
            }
        }
    }
    
    // 存入记忆化缓存（只有不受限且已开始填数）
    if (!limit && isNum) dp[pos][pre] = res;
    return res;
}

// 计算[0, x]范围内Windy数的个数
int solve(int x) {
    if (x <= 0) return 0;
    
    len = 0;
    while (x > 0) {
        digit[++len] = x % 10;
        x /= 10;
    }
    // 此时digit[1]是个位，digit[len]是最高位
    
    memset(dp, -1, sizeof(dp));
    return dfs(len, 0, true, false);
}

int main() {
    int A, B;
    cin >> A >> B;
    
    // 区间[A, B]内Windy数个数 = solve(B) - solve(A-1)
    cout << solve(B) - solve(A - 1) << endl;
    
    return 0;
}