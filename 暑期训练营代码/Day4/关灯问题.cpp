#include<iostream>
#include<vector>
#include<algorithm>
#include<cstring>
using namespace std;
const int inf = 1e8;
int graph[105][15]; 
int n,m;
int caozuo(int bttn[],int mask){
    int new_mask = mask;
    for(int i = 0;i < n;i ++){
        if(bttn[i] == 1){
            new_mask = (new_mask & ~(1 << i));
        }
        if(bttn[i] == -1){
            new_mask = (new_mask | (1 << i));
        }
    }
    return new_mask;
}
int main(){

    cin >> n >> m;
    for(int i = 0;i < m;i ++){
        for(int j = 0;j < n;j ++){
            cin >> graph[i][j];
        }
    }
    /*
    3
    2
    1 0 1
    -1 1 0
    int total = 1 << n
    dp[total];
    dp[mask] = min(dp[pre_mask] + 1)
    */
   int total = 1 << n;
   int dp[total];
   fill(dp,dp + total,inf);
   dp[(1 << n) - 1] = 0;
   for(int mask = (1 << n) - 1;mask >= 0;mask --){
        for(int i = 0;i < m;i ++){
            int new_mask = caozuo(graph[i],mask);
            int cost = dp[mask] + 1;
            if(cost < dp[new_mask]){
                dp[new_mask] = cost;
            }
        }
   }
   if(dp[0] == inf){
    cout << -1;
   }
   else{
   cout << dp[0];    
   }
}