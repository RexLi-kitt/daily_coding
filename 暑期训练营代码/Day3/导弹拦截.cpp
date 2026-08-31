#include<algorithm>
#include<vector>
#include<string>
#include<map>
#include<unordered_map>
#include<iostream>
using namespace std;
int lis(vector<int> &arr){
    vector<int> tails;
    for(int i = 0;i < arr.size();i ++){
        auto it = lower_bound(tails.begin(),tails.end(),arr[i]);
        if(it == tails.end()) tails.push_back(arr[i]);
        else{
            *it = arr[i];
        }
    }
    return tails.size();
}
int uis(vector<int> &arr){
    vector<int> tails;
    for(int i = 0;i < arr.size();i ++){
        auto it = upper_bound(tails.begin(),tails.end(),arr[i]);
        if(it == tails.end()) tails.push_back(arr[i]);
        else{
            *it = arr[i];
        }
    }
    return tails.size();
}
int main(){
    vector<int> data;
    int x;
    while(cin >> x){
        data.push_back(x);
        if(cin.get() == '\n') break;
    }
    vector<int> rev_data = data;
    reverse(rev_data.begin(),rev_data.end());
    int a = uis(rev_data);
    int b = lis(data);
    cout << a << endl;
    cout << b << endl;
}