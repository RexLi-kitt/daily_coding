#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;
bool ok(int len,int k,vector<int> &woods){
    int sum = 0;
    for(auto u : woods){
        sum += u / len;
    }
    return sum >= k;
}
int main(){
    int n,k;
    cin >> n >> k;
    vector<int> woods;
    for(int i = 0;i < n;i ++){
        int temp;
        cin >> temp;
        woods.push_back(temp);
    }
    int l = 1,r = *max_element(woods.begin(),woods.end());
    int ans = 0;
    while (l <= r){
        int mid = l + (r - l) / 2;
        if(ok(mid,k,woods)){
            ans = mid;
            l = mid + 1;
        }
        else{
            r = mid - 1;
        }
    }
    cout << ans;
}