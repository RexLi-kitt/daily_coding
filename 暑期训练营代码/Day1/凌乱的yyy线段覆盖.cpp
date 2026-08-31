#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;
int main(){
    int n;
    cin >> n;
    vector<pair<int,int>> data;
    for(int i = 0;i < n;i ++){
        int a,b ;
        cin >> a >> b;
        data.push_back({b,a});//结束 开始
    }
    sort(data.begin(),data.end());
    int sum = 1;
    int current_idx = 0;
    for(int i = 1;i < n;i ++){
        if(data[current_idx].first <= data[i].second){
            sum ++;
            current_idx = i;
        }
    }
    cout << sum;
}