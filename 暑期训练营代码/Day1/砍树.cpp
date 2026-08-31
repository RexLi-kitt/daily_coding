#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
bool check(int h,long long m,vector<int> &data){
    long long sum = 0;
    for(int i = 0;i < data.size();i ++){
        if(data[i] >= h){
            sum += data[i] - h;
        }
    }
    return sum >= m;
}
int main(){
    long long n,m;
    cin >> n >>m;
    vector<int> data;
    for(int i = 0;i < n;i ++){
        int temp;
        cin >> temp;
        data.push_back(temp);
    }
    int maxh = *max_element(data.begin(), data.end());
    int l = 0, r = maxh;
    long long ans = 0;
    while(l <= r){
        int mid = l + (r - l) / 2;
        if(check(mid, m, data)){
            ans = mid;
            l = mid + 1;
        }
        else{
            r = mid - 1;
        }
    }
    cout << ans << endl;
    return 0;
}
