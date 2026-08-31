#include<iostream>
#include<vector>
#include<stack>
using namespace std;
void parition(int n,int bound,int sum,stack<int>& q){
    int cur_sum = 0;
    for(int i = 0;i < n;i ++){
        q.push(1);
        cur_sum ++;
    }
    while (cur_sum != sum){
        int t = q.top();
        q.pop();
        if(t < bound){
            cur_sum ++;
            t ++;
            q.push(t);
        }
        if(cur_sum == sum){
            break;
        }
    }
}
int main(){
    int t;
    cin >> t;
    for(int i = 0;i < t;i ++){
        int n,s,r;
        cin >> n >> s >> r;
        int maxi = s - r;
        stack<int> q;
        parition(n - 1,maxi,r,q);
        int full = n - q.size();
        for(int i = 0;i < full;i ++){
            cout << maxi << " ";
        }
        while(!q.empty()){
            int v = q.top();
            q.pop();
            cout << v << " ";
        }
        cout << endl;
    }
}