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
