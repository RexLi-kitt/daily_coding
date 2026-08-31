#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;
int main(){
    int n;
    cin >> n;
    vector<int> data(n);
    vector<int> f(n,0);
    cin >> data[0];
    f[0] = data[0];
    int max_value = data[0];
    for(int i = 1;i < n;i ++){
        cin >> data[i];
        f[i] = max(f[i - 1] + data[i],data[i]);
        max_value = max(f[i],max_value);
        }
    cout << max_value;
    }
    