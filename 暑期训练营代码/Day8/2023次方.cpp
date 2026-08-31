#include<iostream>
#include<vector>
using namespace std;
#define ll long long 
const int N = 2024;
vector<int> prime,arr;
bool not_prime[N];
int phi[N];
ll qpow(ll a, ll b, ll mod) {
    if (mod == 1) return 0;      // 任何数模1都是0
    ll res = 1 % mod;
    a %= mod;
    while (b > 0) {
        if (b & 1) res = (res * a) % mod;
        a = (a * a) % mod;
        b >>= 1;
    }
    return res;
}
void pre(int n){
    phi[1]=1;
    for(int i = 2;i <= n;i ++){
        if(!not_prime[i]){
          prime.push_back(i); phi[i] = i - 1;  
        } 
        for(int pri_j : prime){
            if(pri_j * i > n) break;
            not_prime[i * pri_j] = true;
            if(i % pri_j == 0){
                phi[i * pri_j] = phi[i] * pri_j;
                break;
            }
            else{
                phi[i * pri_j] = phi[i] * (pri_j - 1); 
            }
        }        
    }
}
ll dfs(int pos, int m) {
    // 终止条件 1：如果模数是 1，任何数对 1 取模都得 0
    if (m == 1) return 0;
    
    // 终止条件 2：如果已经超出数组末尾（即没有指数了），返回 1（空幂约定）
    if (pos == (int)arr.size()) return 1;

    // 递归计算：指数部分要对 φ(m) 取模
    ll exp_mod = dfs(pos + 1, phi[m]);

    // 因为指数（即 arr[pos+1]^...）极大，必然 >= φ(m)，直接套扩展欧拉定理
    ll exponent = exp_mod + phi[m];

    // 计算 arr[pos]^exponent % m
    return qpow(arr[pos], exponent, m);
}
int main(){
    pre(2023);
    int n = phi[2023];
    for(int i = 2;i <= 2023;i ++){
        arr.push_back(i);
    }
    cout << dfs(0,2023) % 2023;
}