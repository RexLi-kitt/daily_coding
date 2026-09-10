#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
vector<int> weight;

int main(){
    int v,n;
    cin >> v >> n;
    for(int i = 0;i < n;i ++){
        int temp;
        cin >> temp;
        weight.push_back(temp);
    }
    sort(weight.begin(), weight.end(),greater<int>());
    vector<int> dp(n,v);//前i个商品的范围下最小的背包剩余值 dp[i] = min(dp[i - j] - w[i],dp[i])
    for(int i = 0;i < n;i ++){
        for(int j = 0;i - j >= 0;j ++){
            if(dp[i - j] - weight[i]  >= 0){
                dp[i] = min(dp[i - j] - weight[i],dp[i]);
            }
        }
    }
    cout << dp[n - 1];
}