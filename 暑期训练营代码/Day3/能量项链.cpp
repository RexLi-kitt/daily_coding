#include<algorithm>
#include<cstring>
#include<vector>
#include<string>
#include<map>
#include<unordered_map>
#include<iostream>
using namespace std;
int main(){
    int n;
    cin >> n;
    vector<int> data(2 * n);
    for(int i = 0;i < n;i ++){
        int temp;
        cin >> temp;
        data[i] = temp;
        data[i + n] = temp;
    }
    int dp[2 * n][2 * n];//i -> j 的最大释放能量
    memset(dp,0,sizeof(dp));
    /*
    状态转换方程：
    i == k : dp[i][k] = 0 i == k + 1: dp[i][i + 1] = data[i] * data[i + 1] * data[i + 2]
    else:dp[i][j] = max(dp[i][k] + dp[k][j] + data[i] * data[k + 1] * data[j + 1]);
    4
    2 3 5 10

    */
    for(int i = 0;i < 2 * n - 1;i ++){
     dp[i][i + 1] = data[i] * data[(i + 1)] * data[(i + 2) % (2 * n)];
    }
    for(int len = 3;len <= n;len ++){
        for(int i = 0;i + len - 1 < 2 * n;i ++){
            int j = i + len - 1;
                 for(int k = i;k < j;k ++){
                    dp[i][j] = max(dp[i][j],dp[i][k] + dp[k + 1][j] + data[i] * data[(k + 1) % (2 * n)] * data[(j + 1) % (2 * n)]);
                }
        }
    }
    int ans = 0;
    for(int i = 0;i < n;i ++){
        ans = max(ans, dp[i][i + n - 1]);
    }
    cout << ans;
}