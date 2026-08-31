#include<iostream>
#include<vector>
#include<string>
#define ull unsigned long long
using namespace std;
const int base = 233;
vector<ull> hash_num,pw;
ull get(int pos,int len){
    return hash_num[pos + len - 1] - hash_num[pos - 1] * pw[len];
}
int main(){
    string ta,s;
    cin >> ta >> s;
    pw.push_back(1);
    hash_num.push_back(s[0]);
    for(int i = 1;i < s.size();i ++){
        ull temp = hash_num[i - 1] * base + s[i];
        hash_num.push_back(temp);
        pw.push_back(pw[i - 1] * base);
    }
    ull hasho = 0;
    for(int i = 0;i < ta.size();i ++){
        hasho = hasho * base + ta[i];
    }
    int len = ta.size();
    int cnt = 0;
    for(int i = 0;i + len - 1 < s.size();i ++){
        if(hasho == get(i,len)){
            cnt ++;
        }
    }
    cout << cnt;
}