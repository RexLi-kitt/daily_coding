#include<iostream>
#include<vector>
#include<cmath>
using namespace std;
#define ll long long
vector<int> prime;
bool not_prime[50000];
void pre(int n){
    for(int i = 2;i <= n;i ++){
        if(!not_prime[i]){
            prime.push_back(i);
        }
        for(int pri_j : prime){
            if(pri_j * i > n){
                break;
            }
            not_prime[pri_j * i] = true;
            if(i % pri_j == 0){
                break;
            }
        }
    }
}
int main(){
    ll l,r;
    cin >> l >> r;
    if(l == 1) l = 2;
    vector<bool> is_composite(r - l + 1); 
    
    int b = sqrt(r);
    pre(b);
    for(int i = 0;i < prime.size();i ++){
        ll p = prime[i];
        ll start = max(2 * p,((l + p - 1) / p) * p);
        for(ll j = start;j <= r;j += p){
            is_composite[j - l] = true;
        }
    }

    int ans = 0;
    for(bool u : is_composite){
        if(!u){
            ans ++;
        }
    }
    cout << ans;
}