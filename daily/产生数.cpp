#include<iostream>
#include<vector>
#include<cstring>
using namespace std;
vector<vector<int>> re;
vector<int> memo;
bool find(int u){
    for(int v : memo){
        if(u == v){
            return true;
        }
    }
    return false;
}
int dfss(int u){//解决u可以替换为那些数字的问题
    if(!find(u)){
        memo.push_back(u);
    }
    for(int v : re[u]){
        if(!find(v)){
            memo.push_back(v);
            dfss(v);
        }
        else{
            continue;
        }
    }
    return memo.size();
}
void multiply(vector<int>& ans,int num){
    int carry = 0;
    for(size_t i = 0;i < ans.size();i ++){
        int cur = ans[i] * num + carry;
        ans[i] = cur % 10;
        carry = cur / 10;
    }
    while(carry > 0){
        ans.push_back(carry % 10);
        carry /= 10;
    }
}
int main(){
    re.resize(10);
    string d;
    int k;
    cin >> d >> k;
    for(int i = 0;i < k;i ++){
        int u,v;
        cin >> u >> v;
        re[u].push_back(v);
    }
    vector<int> ans(1,1);
    for(char ch : d){
        memo.clear();
        multiply(ans,dfss(ch - '0'));
    }
    for(int i = ans.size() - 1;i >= 0;i --){
        cout << ans[i];
    }
}