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

vector<unsigned long long> hash_num,pw;
void init_hash(string &s){
    int n = s.size();
    hash_num.assign(n + 1,0);
    pw.assign(n + 1,1);
    for(int i = 0;i < n;i ++){
        hash_num[i + 1] = hash_num[i] * 131 + (unsigned char)s[i] + 1;
        pw[i + 1] = pw[i] * 131;
    }
}
unsigned long long get(int l,int r){
    return hash_num[r] - hash_num[l] * pw[r - l];
}
// string s="ababa"; init_hash(s);
// get(0,3) 和 get(2,5) 对应两个 "aba"。
// 当前全局数组只保存最近一次 init_hash 的字符串。
