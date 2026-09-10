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
