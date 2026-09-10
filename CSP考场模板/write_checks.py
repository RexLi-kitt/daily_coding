from pathlib import Path
p=Path('CSP考场模板')
(p/'自测.cpp').write_text(r'''#include "templates.hpp"
#include<cassert>
#include<random>
#include<sstream>
int main(){
    vector<vector<int>> g = {{},{2},{1,3},{2},{}};
    vector<bool> vis(5,false);
    section01::dfs(1,g,vis);
    assert(vis[1] && vis[3] && !vis[4]);
    vector<bool> vis2(5,false);
    section01::dfs_stack(1,g,vis2);
    assert(vis == vis2);
    section02::memo.assign(4,0);
    section02::vis.assign(4,false);
    ostringstream output;
    streambuf *old = cout.rdbuf(output.rdbuf());
    section02::dfs(1,3);
    cout.rdbuf(old);
    string result = output.str();
    assert(count(result.begin(),result.end(),'\n') == 6);
    for(bool used : section02::vis) assert(!used);
    vector<int> dist;
    section03::bfs(1,g,dist);
    assert((dist == vector<int>{-1,0,1,2,-1}));
    vector<string> grid = {"....",".X.."};
    vector<pair<int,int>> src = {{0,0},{0,0}};
    vector<vector<int>> gd;
    section04::bfs(grid,src,gd);
    assert(gd[1][3] == 4 && gd[1][1] == -1);
    vector<vector<pair<int,long long>>> w(5);
    w[1] = {{2,3000000000LL},{2,2},{3,10}};
    w[2] = {{3,3}};
    vector<long long> d;
    section05::dijkstra(1,w,d);
    assert(d[3] == 5 && d[4] == INF);
    w[1] = {{2,3000000000LL}};
    section05::dijkstra(1,w,d);
    assert(d[3] == 3000000003LL);
    assert(section06::longest(1,w,d));
    assert(d[3] == 3000000003LL && d[4] == -INF);
    w[3].push_back({1,1});
    assert(!section06::longest(1,w,d));
    section07::init(4);
    section07::unite(1,2);section07::unite(2,3);section07::unite(1,1);
    assert(section07::find(1)==section07::find(3));
    assert(section07::find(1)!=section07::find(4));
    section08::edges = {{1,2,-1}};
    long long total;
    assert(section08::kruskal(2,total) && total == -1);
    assert(!section08::kruskal(3,total));
    section08::edges.clear();
    assert(section08::kruskal(1,total) && total == 0);
    section10::n = 5;section10::tree.assign(6,0);
    section10::add(1,3);section10::add(5,-2);
    assert(section10::query(1,5)==1 && section10::query(5,5)==-2);
    vector<long long> a={0,2,2,3,1};
    assert((section11::next_greater(a)==vector<int>{0,3,3,0,0}));
    a={3,1,2,0};
    assert((section12::window_min(a,2)==vector<long long>{1,1,0}));
    vector<int> weight={3};vector<long long> value={5};
    assert(section13::bag01(6,weight,value)==5);
    assert(section13::bag_complete(6,weight,value)==10);
    a={2,2,1,3};assert(section14::lis(a)==2);
    a.clear();assert(section14::lis(a)==0);
    string s="ababa";section15::init_hash(s);
    assert(section15::get(0,3)==section15::get(2,5));
    assert(section15::get(0,0)==0);
    s="aaaa";string p="aa";
    assert((section16::kmp(s,p)==vector<int>{0,1,2}));
    assert(section17::qpow(2,10,1000000007)==1024);
    mt19937 rng(42);
    for(int t=0;t<100;t++){
        int n=8;
        vector<vector<pair<int,long long>>> adj(n+1);
        vector<vector<long long>> f(n+1,vector<long long>(n+1,INF));
        for(int i=1;i<=n;i++) f[i][i]=0;
        for(int k=0;k<30;k++){
            int u=1+rng()%n,v=1+rng()%n;
            long long z=rng()%100;
            adj[u].push_back({v,z});f[u][v]=min(f[u][v],z);
        }
        for(int k=1;k<=n;k++) for(int i=1;i<=n;i++) for(int j=1;j<=n;j++){
            if(f[i][k]!=INF && f[k][j]!=INF) f[i][j]=min(f[i][j],f[i][k]+f[k][j]);
        }
        for(int start=1;start<=n;start++){
            section05::dijkstra(start,adj,d);
            for(int v=1;v<=n;v++) assert(d[v]==f[start][v]);
        }
        // 跳石头二分与穷举所有删石方案对拍
        vector<long long> stones;
        for(int x=1;x<12;x++) if(rng()%2) stones.push_back(x);
        int cnt=stones.size(),m=rng()%(cnt+1);
        long long best=0;
        for(int mask=0;mask<(1<<cnt);mask++){
            if(__builtin_popcount((unsigned)mask)>m) continue;
            long long last=0,gap=12;
            for(int i=0;i<cnt;i++) if(!(mask>>i&1)){
                gap=min(gap,stones[i]-last);last=stones[i];
            }
            gap=min(gap,12-last);best=max(best,gap);
        }
        stones.push_back(12);
        assert(section09::binary_search(12,stones,m)==best);
    }
    cout << "All checks passed\n";
}
''',encoding='utf-8')
