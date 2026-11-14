#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;    
vector<int> weight;
vector<int> bucket;
int n,w;
bool dfs(int idx){
    if(idx == weight.size()){
        return true;
    }
    for(int i = 0;i < bucket.size();i ++){
        if(weight[idx] + bucket[i] <= w){
            bucket[i] += weight[idx];
            if(dfs(idx + 1)) return true;
            bucket[i] -= weight[idx];
        }
        if(bucket[i] == 0) break;
    }
    return false;
}

int test(){
    int lb = n;
    int sum = 0;
    for(int u : weight){
        sum += u;
    }
    int rb = sum / w;
    for(int i = rb;i <= lb;i ++){
        bucket.resize(i);
        if(dfs(0)) return i;
    }
}
int main(){
    cin >> n >> w;
    for(int i = 0;i < n;i ++){
        int temp;
        cin >> temp;
        weight.push_back(temp);
    }
    sort(weight.begin(),weight.end(),greater<int>());
    int ans = test();
    cout << ans;
}