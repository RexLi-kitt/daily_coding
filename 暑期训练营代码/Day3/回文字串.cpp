#include<iostream>
#include<algorithm>
#include<vector>
#include<iomanip>
#include<string>
#include<cstring>
using namespace std;
int main(){
    string data;
    cin >> data;
    //Ab3bd
    /*
    Ab A b
    A b3bd 0 1 cost = min(len(A),len(B)) = 1  sum = 1 + 1 = 2
    Ab 3bd f[0][1] = 1 f[2][4] = 2 cost = min(len) = 2 sum = 2 + 2 = 4
    Ab3 bd f[0][2] = 2 f[3][4] = 1 cost = min(len) = 2 sum = 2 + 2 = 4
    Ab3b d f[0][3] = 1 f[4][4] = 0 cost = min(len) = 1 sum = 1 + 1 = 2
    */
   //Yuioi obviously 2
   /*
   Y uioi f[0][0] = 0 f[1][4] = 1 cost = min(len) = 1 sum = 2
   Yu ioi f[0][1] = 1 f[2][4] = 0 cost = min(len) = 2 sum = 2 + 0 = 2
   Yui oi f[0][2] = 2 f[3][4] = 1 cost = min(len) = 2 sum = 2 + 2 = 4
        in this situation Alternatives: sum = max(len) + f[3][4] = 3 + 1 = 4
   Yuio i f[0][3] = 3 f[4][4] = 0 cost = min(len) = 1 sum = 1 + 3 = 4 
   so we can define dp[i][j] = min{f(dp[i][k],dp[k][j]) + cost,dp[i][j]}; cost = min((k - i + 1),(j - k + 1))
    real?
    assume Ajkkjb 2
    Ajkkj b 1 0
    ok
   */
    
    int n = data.size();
    int dp[n][n];
    memset(dp,0x3f,sizeof(dp));
    for(int i = 0;i < n;i ++){
        dp[i][i] = 0;
    }
    for(int len = 2;len <= n;len ++){
        for(int i = 0;i + len - 1 < n;i ++){
            int j = i + len - 1;
            // 两端相等，不切直接往里缩
            if(data[i] == data[j]){
                if(len == 2) dp[i][j] = 0;
                else dp[i][j] = dp[i+1][j-1];
            }
            for(int k = i;k < j;k ++){
                int value = min(j - k + dp[i][k], k - i + 1 + dp[k+1][j]);
                dp[i][j] = min(value, dp[i][j]);
            }
        }
    }
    cout << dp[0][n-1] << endl;
}