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

long long qpow(long long a,long long b,long long mod){
    a = (a % mod + mod) % mod;
    long long ans = 1 % mod;
    while(b > 0){
        if(b % 2 == 1){
            ans = ans * a % mod;
        }
        a = a * a % mod;
        b /= 2;
    }
    return ans;
}
// 质数 p 下，a 的逆元：qpow(a,p-2,p)。
// 批量求逆元（1<=n<p，p是质数）：
// vector<long long> inv(n + 1);
// inv[1] = 1;
// for(int i = 2;i <= n;i ++){
//     inv[i] = (p - p / i) * inv[p % i] % p;
// }
