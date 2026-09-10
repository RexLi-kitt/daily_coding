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
