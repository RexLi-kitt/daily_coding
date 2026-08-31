#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
long long lower_bound_search(long long value,vector<long long> &data){
    long long l = 0, r = data.size();
    while(l < r){
        long long mid = l + (r - l) / 2;
        if(data[mid] >= value){
            r = mid;
        }
        else{
            l = mid + 1;
        }
    }
    return l;
}
long long upper_bound_search(long long value,vector<long long> &data){
    long long l = 0, r = data.size();
    while(l < r){
        long long mid = l + (r - l) / 2;
        if(data[mid] > value){
            r = mid;
        }
        else{
            l = mid + 1;
        }
    }
    return l;
}
int main(){
    long long n,c;
    cin >> n >> c;
    vector<long long> data;
    for(long long i = 0;i < n;i ++){
        long long temp;
        cin >> temp;
        data.push_back(temp);
    }
    long long ans = 0;
    sort(data.begin(),data.end());
    for(long long i = 0;i < n;i ++){
        long long value = c + data[i];
        long long lower = lower_bound_search(value,data);
        long long upper = upper_bound_search(value,data);
        ans += (upper - lower);
    }
    cout << ans;
}
