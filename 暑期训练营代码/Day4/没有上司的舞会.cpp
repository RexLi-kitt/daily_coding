#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;    
int n;
vector<vector<int>> adje(n + 1);
vector<vector<int>> value(n + 1,vector<int>(2,-1145141));//0->没有i的，以i为根的最大开心值；1->有i的，以i为根的最大开行值
vector<int> happiness(n + 1);
int dfs(int o,bool willing){//找以O为根的，意愿为willing的最大开心值 
    
    int max_h = 0;    
    if(willing && adje[o].empty()){
        value[o][1] = happiness[o];
        return happiness[o];
    }
    if(!willing && adje[o].empty()){
        value[o][0] = 0;
        return 0;
    }//处理底层情况
//更新没有o的
    if(!willing){
        if(value[o][0] != -1145141){
            max_h = value[o][0];
        }
        else{
            for(auto v : adje[o]){
                max_h += max(dfs(v,0),dfs(v,1));
            }
            value[o][0] = max_h;
        }
    }
    else if(willing && !adje[o].empty()){
        if(value[o][1] != -1145141){
            max_h = value[o][1];
        }
        else{
            for(auto v: adje[o]){
                max_h += dfs(v,0);
            }         
            max_h += happiness[o];
            value[o][1] = max_h;           
        }
    }
    return max_h;
}
int main(){
    cin >> n;
    adje.resize(n + 1);
    happiness.resize(n + 1);
    value.resize(n + 1,vector<int>(2, -1145141));
    for(int i = 1;i <= n;i ++){
        cin >> happiness[i];
    }
    vector<int> indegree(n + 1, 0);
    for(int i = 0;i < n - 1;i ++){
        int l,k;
        cin >> l >> k;
        adje[k].push_back(l);
        indegree[l]++;
    }
    int root = 1;
    for(int i = 1;i <= n;i ++){
        if(indegree[i] == 0){
            root = i;
            break;
        }
    }
    dfs(root,1),dfs(root,0);
    cout << max(value[root][0],value[root][1]);
}