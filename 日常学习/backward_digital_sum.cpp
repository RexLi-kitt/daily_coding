#include<iostream>
#include<vector>
using namespace std;
vector<int> memo,vis;
vector<vector<int>> tri;
void init(int n){
    memo.resize(n + 1);
    vis.resize(n + 1);
    tri.resize(n + 1);
    tri[1].push_back(1);
    tri[2].push_back(1);
    tri[2].push_back(1);
    for(int i = 3;i <= n;i ++){
        tri[i].push_back(1);
        for(int j = 0;j < tri[i - 1].size();j ++){
            if(j == tri[i - 1].size() - 1){
                tri[i].push_back(1);
            }
            else{
                tri[i].push_back(tri[i][j] + tri[i][j + 1]);
            }
        }
    }
}
bool check(int n){

}
bool dfs(int idx,int n){
    if(idx == n){
        if(check(n)) return true;
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

}