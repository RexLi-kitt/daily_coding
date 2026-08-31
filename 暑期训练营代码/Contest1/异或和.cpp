#include<iostream>
#include<vector>
#include<algorithm>
#include<unordered_set>
using namespace std;
int main(){
    int n,k;
    cin >> n >> k;
    unordered_set<int> pre;
    pre.insert(0);
    int cur = 0,cnt = 0;
    for(int i = 0;i < n;i ++){
        int temp;
        cin >> temp;
        cur = cur ^ temp;
        int nxt = cur ^ k;
        auto it = pre.find(nxt);
        if(it == pre.end()){//没找到，插入当前值
            pre.insert(cur);
        }
        else{//找到了，更新cur,cnt
            cur = 0;
            cnt ++;
            pre.clear();
            pre.insert(0);
        }
    }
    cout << cnt;
}