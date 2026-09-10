#include<iostream>
#include<vector>
#include<queue>
#include<stack>
#include<deque>
#include<algorithm>
#include<string>
#include<functional>
#include<utility>
using namespace std;
const long long INF = (1LL << 62);

long long bag01(int V,vector<int> &weight,vector<long long> &value){
    vector<long long> dp(V + 1,0);
    for(int i = 0;i < (int)weight.size();i ++){
        // 每件只能选一次，所以容量倒序
        for(int j = V;j >= weight[i];j --){
            dp[j] = max(dp[j],dp[j - weight[i]] + value[i]);
        }
    }
    return dp[V];
}
long long bag_complete(int V,vector<int> &weight,
                       vector<long long> &value){
    vector<long long> dp(V + 1,0);
    for(int i = 0;i < (int)weight.size();i ++){
        // 每件可以选无限次，所以容量正序
        for(int j = weight[i];j <= V;j ++){
            dp[j] = max(dp[j],dp[j - weight[i]] + value[i]);
        }
    }
    return dp[V];
}
// 装箱最小剩余：让 value[i]=weight[i]，答案 V-bag01(V,weight,value)。
