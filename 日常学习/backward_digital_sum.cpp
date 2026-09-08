#include<iostream>
#include<vector>
using namespace std;
vector<int> memo,vis;
vector<int> coef;
int sum;
int compound(int n,int m){
    if(m == 0 || m == n){
        return 1;
    }
    else{
        int ans = 1;
        for(int i = n;i >= n - m + 1;i --){
            ans *= i;
        }
        for(int i = m;i >= 2;i --){
            ans /= i;
        }
        return ans;
    }
}
void init(int n){
    memo.resize(n + 1);
    coef.resize(n + 1);
    vis.resize(n + 1);
    for(int i = 1;i <= n;i ++){
        vis[i] = true;
    }
    coef.resize(n + 1);
    int base = n - 1;
    for(int i = 1;i <= n;i ++){
        coef[i] = compound(base,i - 1);
    }
}
bool check(int n){
    int aa = 0;
    for(int i = 1;i <= n;i ++){
        aa += coef[i] * memo[i];
    }
    return aa == sum;
}
bool already_exceed(int idx){
    int aa = 0;
    for(int i = 1;i < idx;i ++){
        aa += coef[i] * memo[i];
    }
    return aa > sum;    
}
bool dfs(int idx,int n){
    if(idx == n + 1){
        if(check(n)) return true;
        return false;
    }
    if(already_exceed(idx)){
        return false;
    }
    for(int i = 1;i <= n;i ++){
        if(vis[i]){
            memo[idx] = i;
            vis[i] = false;

        }
        else{
            continue;
        }
        if(dfs(idx + 1,n)){
            return true;
        }
        vis[i] = true;
    }
    return false;
}
int main(){
    int n;
    cin >> n >> sum;
    init(n);
    if(dfs(1,n)){
        for(int i = 1;i <= n;i ++) cout << memo[i] <<" ";
        cout <<"\n";
    }      
    //
}