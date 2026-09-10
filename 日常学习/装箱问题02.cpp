#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;
//此题能否用dfs做呢
vector<int> weight;    
int v,n;
int dfs(int idx, int cur) {
    if (idx == weight.size()) {
        return cur;
    }
    // 不选择当前物品
    int ans = dfs(idx + 1, cur);
    // 选择当前物品
    if (cur + weight[idx] <= v) {
        ans = max(ans, dfs(idx + 1, cur + weight[idx]));
    }

    return ans;
}
int main(){
    cin >> v >> n;
    for(int i = 0;i < n;i ++){
        int temp;
        cin >> temp;
        weight.push_back(temp);
    }
    sort(weight.begin(), weight.end(),greater<int>());
    cout << v - dfs(0,0);
}