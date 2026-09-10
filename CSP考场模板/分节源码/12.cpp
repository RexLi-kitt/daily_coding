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
