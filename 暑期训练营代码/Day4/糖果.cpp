#include <iostream>
#include <vector>
using namespace std;

const long long INF = 1e18;

long long caozuo(long long mask, vector<long long>& pack, long long k) {
    long long new_mask = mask;
    for (long long i = 0; i < k; i++) {
        new_mask = (new_mask | (1LL << (pack[i] - 1)));
    }
    return new_mask;
}

int main() {
    long long n, m, k;
    cin >> n >> m >> k;
    vector<vector<long long>> pack(n, vector<long long>(k));
    for (long long i = 0; i < n; i++) {
        for (long long j = 0; j < k; j++) {
            cin >> pack[i][j];
        }
    }
    long long total = 1LL << m;
    vector<long long> dp(total, INF);
    dp[0] = 0;
    for (long long mask = 0; mask < total - 1; mask++) {
        if (dp[mask] >= INF) continue;
        for (long long i = 0; i < n; i++) {
            long long new_mask = caozuo(mask, pack[i], k);
            dp[new_mask] = min(dp[new_mask], dp[mask] + 1);
        }
    }
    if(dp[total - 1] >= INF / 2){
        cout << -1;
    }
    else{
    cout << dp[total - 1];        
    }
 
    return 0;
}
