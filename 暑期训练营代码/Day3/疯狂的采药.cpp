#include<iostream>
#include<algorithm>
#include<vector>
#include<cstring>
using namespace std;
int main(){
    int n,m;
    cin >> n >> m;
    int v[m],w[m];
    for(int i = 0;i < m;i ++){
        cin >> v[i] >> w[i];
    }
    vector<long long> dp(n + 1, 0);
    for(int i = 0;i < m;i ++){
        for(int j = v[i];j <= n;j ++){
            dp[j] = max(dp[j], dp[j - v[i]] + w[i]);
        }
    }
    cout << dp[n];
    return 0;
}
