#include<iostream>
#include<vector>
#include<stack>
using namespace std;
int main(){  
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    cin >> n;
    stack<int> s;
    vector<int> data(n + 1);
    vector<int> ans(n + 1, 0);
    for(int i = 1; i <= n; i++){
        cin >> data[i];
        while(!s.empty() && data[i] > data[s.top()]){
            ans[s.top()] = i;
            s.pop();
        }
        s.push(i);
    }
    for(int i = 1; i <= n; i++){
        cout << ans[i] << " ";
    }
    return 0;
}
