#include<vector>
#include<iostream>
#include<algorithm>
#include<string>
using namespace std;
int main(){
    string data;
    cin >> data;
    vector<int> t;
    for(auto c : data){
        if(c >= '0' && c <= '9'){
            t.push_back(c - '0');
        }
    }
    sort(t.begin(),t.end());
    for(int i = t.size() - 1;i >= 0;i --){
        cout << t[i];
    }
}