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

struct edge{
    int u;
    int v;
    long long w;
    bool operator<(const edge &other) const{
        return w < other.w;
    }
};
vector<edge> edges;
bool kruskal(int n,long long &total){
    init(n);
    sort(edges.begin(),edges.end());
    total = 0;
    int cnt = 0;
    for(auto &e : edges){
        if(cnt == n - 1){
            break;
        }
        if(find(e.u) != find(e.v)){
            unite(e.u,e.v);
            total += e.w;
            cnt ++;
        }
    }
    return cnt == n - 1;
}
// 每组先 edges.clear()，读边：edges.push_back({u,v,w});
// long long total;
// if(kruskal(n,total)) cout << total;
// else cout << "orz"; // 不连通的输出按题目要求修改
