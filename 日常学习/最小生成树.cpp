#include<vector>
#include<iostream>
#include<algorithm>
using namespace std;
vector<int> fa,rk;
struct edge
{
    int u;
    int v;
    int w;
    bool operator<(const edge& other) const{return w < other.w;}
};
vector<edge> edges;
void init_e(int m){
    edges.resize(m);
}
void init(int n){
    fa.resize(n + 1);
    rk.resize(n + 1);
    for(int i = 1;i <= n;i ++){
        fa[i] = i;
        rk[i] = 0;
    }
}
int find(int x){
    return x == fa[x] ? x : fa[x] = find(fa[x]);
}
void unite(int x,int y){
    x = find(x);
    y = find(y);
    if(rk[x] < rk[y]){
        swap(x,y);
    }
    fa[y] = x;
    if(rk[x] == rk[y]) rk[x] ++;
}
int krustal(int n){
    sort(edges.begin(),edges.end());

    int total = 0;int cnt = 0;
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
    return cnt == n - 1 ? total : -1;
}
int main(){
    int n,m;
    cin >> n >> m;
    init(n);
    init_e(m);
    for(int i = 0;i < m;i ++){
        int u,v,w;
        cin >> u >> v >> w;
        edge temp = {u,v,w};
        edges[i] = temp;
    }
    int ans = krustal(n);
    if(ans == -1){
        cout << "orz";
    }
    else{
        cout << ans;
    }
    return 0;
}