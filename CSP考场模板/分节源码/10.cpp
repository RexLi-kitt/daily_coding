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
