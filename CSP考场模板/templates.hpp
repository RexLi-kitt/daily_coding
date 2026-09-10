#pragma once
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

// 每个编号是独立模板。纸质版、分节源码无需 namespace。
namespace section01{
void dfs(int u,vector<vector<int>> &g,vector<bool> &vis){
    vis[u] = true;
    // 在这里处理当前点 u
    for(int v : g[u]){
        if(!vis[v]){
            dfs(v,g,vis);
        }
    }
}
// 调用：vector<bool> vis(n + 1,false); dfs(s,g,vis);
// 求无向图连通块：枚举 i=1..n，没访问过就 cnt++，dfs(i,g,vis)。

// 点很多、递归可能太深时，用栈代替递归
void dfs_stack(int s,vector<vector<int>> &g,vector<bool> &vis){
    stack<int> st;
    st.push(s);
    vis[s] = true;
    while(!st.empty()){
        int u = st.top();
        st.pop();
        for(int v : g[u]){
            if(!vis[v]){
                vis[v] = true;
                st.push(v);
            }
        }
    }
}
}

namespace section02{
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
}

namespace section03{
void bfs(int s,vector<vector<int>> &g,vector<int> &dist){
    dist.assign(g.size(),-1);
    queue<int> q;
    dist[s] = 0;
    q.push(s);
    while(!q.empty()){
        int u = q.front();
        q.pop();
        for(int v : g[u]){
            if(dist[v] == -1){
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
}
// vector<int> dist;
// bfs(s,g,dist);
// cout << dist[t]; // 这里约定不可达输出 -1，实际看题目要求
}

namespace section04{
void bfs(vector<string> &grid,vector<pair<int,int>> &src,
         vector<vector<int>> &dist){
    int n = grid.size();
    int m = 0;
    if(n > 0){
        m = grid[0].size();
    }
    dist.assign(n,vector<int>(m,-1));
    queue<pair<int,int>> q;
    for(auto [x,y] : src){
        if(grid[x][y] != 'X' && dist[x][y] == -1){
            dist[x][y] = 0;
            q.push({x,y});
        }
    }
    int dx[4] = {1,-1,0,0};
    int dy[4] = {0,0,1,-1};
    while(!q.empty()){
        auto [x,y] = q.front();
        q.pop();
        for(int i = 0;i < 4;i ++){
            int nx = x + dx[i];
            int ny = y + dy[i];
            if(nx < 0 || nx >= n || ny < 0 || ny >= m){
                continue;
            }
            if(grid[nx][ny] == 'X' || dist[nx][ny] != -1){
                continue;
            }
            dist[nx][ny] = dist[x][y] + 1;
            q.push({nx,ny});
        }
    }
}
}

namespace section05{
void dijkstra(int s,vector<vector<pair<int,long long>>> &g,
              vector<long long> &dist){
    dist.assign(g.size(),INF);
    vector<bool> done(g.size(),false);
    priority_queue<pair<long long,int>,
                   vector<pair<long long,int>>,greater<>> pq;
    dist[s] = 0;
    pq.push({0,s}); // 距离在前，点的编号在后
    while(!pq.empty()){
        auto [d,u] = pq.top();
        pq.pop();
        if(done[u]){
            continue;
        }
        done[u] = true;
        for(auto [v,w] : g[u]){
            if(dist[u] + w < dist[v]){
                dist[v] = dist[u] + w;
                pq.push({dist[v],v});
            }
        }
    }
}
// 调用：vector<long long> dist; dijkstra(s,g,dist);
// dist[t] == INF 表示不可达，按题意输出。
// 边权也须小于 INF，保证 dist[u]+w 不溢出。
// 记录路径：更新成功时 pre[v]=u；从终点回溯，再 reverse。
}

namespace section06{
vector<int> topo(vector<vector<pair<int,long long>>> &g,int n){
    vector<int> indeg(n + 1,0);
    for(int i = 1;i <= n;i ++){
        for(auto [v,w] : g[i]){
            indeg[v] ++;
        }
    }
    queue<int> q;
    for(int i = 1;i <= n;i ++){
        if(indeg[i] == 0){
            q.push(i);
        }
    }
    vector<int> ans;
    while(!q.empty()){
        int u = q.front();
        q.pop();
        ans.push_back(u);
        for(auto [v,w] : g[u]){
            indeg[v] --;
            if(indeg[v] == 0){
                q.push(v);
            }
        }
    }
    return ans;
}
// 返回 false 表示有环，dp 通过引用参数带回
bool longest(int s,vector<vector<pair<int,long long>>> &g,
             vector<long long> &dp){
    int n = (int)g.size() - 1;
    vector<int> topo_q = topo(g,n);
    dp.assign(n + 1,-INF);
    if((int)topo_q.size() != n){
        return false;
    }
    dp[s] = 0;
    for(int u : topo_q){
        if(dp[u] == -INF){
            continue;
        }
        for(auto [v,w] : g[u]){
            dp[v] = max(dp[v],dp[u] + w);
        }
    }
    return true;
}
}

namespace section07{
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
}

namespace section08{
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
}

namespace section09{
bool ok(long long dist,vector<long long> &location,int m){
    int move_cnt = 0;
    long long last = 0;
    for(long long x : location){
        if(x - last < dist){
            move_cnt ++;
        }
        else{
            last = x;
        }
    }
    return move_cnt <= m;
}
long long binary_search(long long L,vector<long long> &location,int m){
    long long l = 0;
    long long r = L;
    while(l < r){
        long long mid = l + (r - l + 1) / 2;
        if(ok(mid,location,m)){
            l = mid;
        }
        else{
            r = mid - 1;
        }
    }
    return l;
}
// 调用前 sort(location.begin(),location.end()); location.push_back(L);
// 最后一段过短时，计数相当于移除前一个保留石头，终点不移动。
// 找最小可行值（先 false 后 true），改成：
// mid = l + (r-l)/2;
// if(ok(mid,...)) r=mid; else l=mid+1;
// 两种写法都要求 [l,r] 内至少有一个可行答案。
}

namespace section10{
int n;
vector<long long> tree;
int lowbit(int x){
    return x & -x;
}
void add(int x,long long k){
    while(x <= n){
        tree[x] += k;
        x += lowbit(x);
    }
}
long long sum(int x){
    long long ans = 0;
    while(x > 0){
        ans += tree[x];
        x -= lowbit(x);
    }
    return ans;
}
long long query(int l,int r){
    return sum(r) - sum(l - 1);
}
// 区间加、单点查：改为用差分数组建树。
// [l,r]加k：add(l,k); 若r<n，再add(r+1,-k)。
// 查询第x个值：sum(x)。不要和“单点加、区间和”混用。
}

namespace section11{
vector<int> next_greater(vector<long long> &data){
    int n = (int)data.size() - 1;
    stack<int> s;
    vector<int> ans(n + 1,0);
    for(int i = 1;i <= n;i ++){
        while(!s.empty() && data[i] > data[s.top()]){
            ans[s.top()] = i;
            s.pop();
        }
        s.push(i);
    }
    return ans;
}
}

namespace section12{
vector<long long> window_min(vector<long long> &a,int k){
    deque<int> q;
    vector<long long> ans;
    for(int i = 0;i < (int)a.size();i ++){
        while(!q.empty() && q.front() <= i - k){
            q.pop_front(); // 移除已经离开窗口的下标
        }
        while(!q.empty() && a[q.back()] >= a[i]){
            q.pop_back(); // 后来的数更小，前面的大数没有用了
        }
        q.push_back(i);
        if(i >= k - 1){
            ans.push_back(a[q.front()]);
        }
    }
    return ans;
}
}

namespace section13{
long long bag01(int V,vector<int> &weight,vector<long long> &value){
    vector<long long> dp(V + 1,0);
    for(int i = 0;i < (int)weight.size();i ++){
        // 每件只能选一次，所以容量倒序
        for(int j = V;j >= weight[i];j --){
            dp[j] = max(dp[j],dp[j - weight[i]] + value[i]);
        }
    }
    return dp[V];
}
long long bag_complete(int V,vector<int> &weight,
                       vector<long long> &value){
    vector<long long> dp(V + 1,0);
    for(int i = 0;i < (int)weight.size();i ++){
        // 每件可以选无限次，所以容量正序
        for(int j = weight[i];j <= V;j ++){
            dp[j] = max(dp[j],dp[j - weight[i]] + value[i]);
        }
    }
    return dp[V];
}
// 装箱最小剩余：让 value[i]=weight[i]，答案 V-bag01(V,weight,value)。
}

namespace section14{
int lis(vector<long long> &a){
    vector<long long> tails;
    for(long long x : a){
        auto it = lower_bound(tails.begin(),tails.end(),x);
        if(it == tails.end()){
            tails.push_back(x);
        }
        else{
            *it = x;
        }
    }
    return tails.size();
}
}

namespace section15{
vector<unsigned long long> hash_num,pw;
void init_hash(string &s){
    int n = s.size();
    hash_num.assign(n + 1,0);
    pw.assign(n + 1,1);
    for(int i = 0;i < n;i ++){
        hash_num[i + 1] = hash_num[i] * 131 + (unsigned char)s[i] + 1;
        pw[i + 1] = pw[i] * 131;
    }
}
unsigned long long get(int l,int r){
    return hash_num[r] - hash_num[l] * pw[r - l];
}
// string s="ababa"; init_hash(s);
// get(0,3) 和 get(2,5) 对应两个 "aba"。
// 当前全局数组只保存最近一次 init_hash 的字符串。
}

namespace section16{
vector<int> kmp(string &s,string &p){
    int m = p.size();
    vector<int> ans;
    if(m == 0){
        return ans;
    }
    vector<int> nxt(m,0);
    int j = 0;
    for(int i = 1;i < m;i ++){
        while(j > 0 && p[i] != p[j]){
            j = nxt[j - 1];
        }
        if(p[i] == p[j]){
            j ++;
        }
        nxt[i] = j;
    }
    j = 0;
    for(int i = 0;i < (int)s.size();i ++){
        while(j > 0 && s[i] != p[j]){
            j = nxt[j - 1];
        }
        if(s[i] == p[j]){
            j ++;
        }
        if(j == m){
            ans.push_back(i - m + 1);
            j = nxt[j - 1]; // 继续找，允许重叠
        }
    }
    return ans;
}
}

namespace section17{
long long qpow(long long a,long long b,long long mod){
    a = (a % mod + mod) % mod;
    long long ans = 1 % mod;
    while(b > 0){
        if(b % 2 == 1){
            ans = ans * a % mod;
        }
        a = a * a % mod;
        b /= 2;
    }
    return ans;
}
// 质数 p 下，a 的逆元：qpow(a,p-2,p)。
// 批量求逆元（1<=n<p，p是质数）：
// vector<long long> inv(n + 1);
// inv[1] = 1;
// for(int i = 2;i <= n;i ++){
//     inv[i] = (p - p / i) * inv[p % i] % p;
// }
}

