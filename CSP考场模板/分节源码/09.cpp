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
