#include<iostream>
#include<algorithm>
#include<vector>
#include<iomanip>
#include<string>
#include<cstring>
using namespace std;
int main(){
    int n;
    cin >> n;
    vector<int> tails;
    for(int i = 0;i < n;i ++){
        int x;
        cin >> x;
        auto it = lower_bound(tails.begin(),tails.end(),x);
        if(it == tails.end()) tails.push_back(x);
        else{
            *it = x;
        }
    }
    cout << tails.size();
}