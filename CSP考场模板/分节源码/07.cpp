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

vector<int> fa,rk;
void init(int n){
    fa.resize(n + 1);
    rk.assign(n + 1,0);
    for(int i = 0;i <= n;i ++){
        fa[i] = i;
    }
}
int find(int x){
    if(x == fa[x]){
        return x;
    }
    fa[x] = find(fa[x]); // 路径压缩
    return fa[x];
}
void unite(int x,int y){
    x = find(x);
    y = find(y);
    if(x == y){
        return;
    }
    if(rk[x] < rk[y]){
        swap(x,y);
    }
    fa[y] = x;
    if(rk[x] == rk[y]){
        rk[x] ++;
    }
}
// init(n); unite(x,y);
// if(find(x) == find(y)) cout << "同一个集合";
