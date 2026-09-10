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

vector<int> memo;
vector<bool> vis;
void dfs(int idx,int n){
    if(idx == n + 1){
        for(int i = 1;i <= n;i ++){
            cout << memo[i] << ' ';
        }
        cout << '\n';
        return;
    }
    for(int i = 1;i <= n;i ++){
        if(vis[i]){
            continue;
        }
        memo[idx] = i;
        vis[i] = true;
        dfs(idx + 1,n);
        vis[i] = false; // 撤销选择，让其他分支也能用 i
    }
}
// main 里先初始化，再开始搜索：
// memo.assign(n + 1,0);
// vis.assign(n + 1,false);
// dfs(1,n);
// 找一个解就结束：改为 bool dfs，找到后 return true，参考你的原题。
