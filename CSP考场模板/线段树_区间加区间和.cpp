#include<iostream>
#include<vector>
using namespace std;



const int MAXN = 100005;

long long a[MAXN];           // 原始数组
long long tree[MAXN << 2];   // 线段树，区间和
long long lazy[MAXN << 2];   // 懒标记

// 建树
void build(int node, int l, int r){
    if(l == r){
        tree[node] = a[l];
        return;
    }
    int mid = (l + r) >> 1;
    build(node << 1, l, mid);
    build(node << 1 | 1, mid + 1, r);
    tree[node] = tree[node << 1] + tree[node << 1 | 1];
}

// 下传懒标记
void push_down(int node, int l, int r){
    if(lazy[node] == 0) return;
    int mid = (l + r) >> 1;
    int left = node << 1;
    int right = node << 1 | 1;
    // 更新左子节点
    tree[left] += lazy[node] * (mid - l + 1);
    lazy[left] += lazy[node];
    // 更新右子节点
    tree[right] += lazy[node] * (r - mid);
    lazy[right] += lazy[node];
    // 清除当前节点懒标记
    lazy[node] = 0;
}

// 区间更新 [ql, qr] += val
void update(int node, int l, int r, int ql, int qr, long long val){
    if(ql <= l && r <= qr){
        tree[node] += val * (r - l + 1);
        lazy[node] += val;
        return;
    }
    push_down(node, l, r);
    int mid = (l + r) >> 1;
    if(ql <= mid) update(node << 1, l, mid, ql, qr, val);
    if(qr > mid)  update(node << 1 | 1, mid + 1, r, ql, qr, val);
    tree[node] = tree[node << 1] + tree[node << 1 | 1];
}

// 区间查询 [ql, qr] 的和
long long query(int node, int l, int r, int ql, int qr){
    if(ql <= l && r <= qr){
        return tree[node];
    }
    push_down(node, l, r);
    int mid = (l + r) >> 1;
    long long ans = 0;
    if(ql <= mid) ans += query(node << 1, l, mid, ql, qr);
    if(qr > mid)  ans += query(node << 1 | 1, mid + 1, r, ql, qr);
    return ans;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    for(int i = 1;i <= n;i ++){
        cin >> a[i];
    }
    build(1, 1, n);

    while(m--){
        int op, x, y;
        cin >> op >> x >> y;
        if(op == 1){
            long long k;
            cin >> k;
            update(1, 1, n, x, y, k);
        } else {
            cout << query(1, 1, n, x, y) << '\n';
        }
    }
    return 0;
}