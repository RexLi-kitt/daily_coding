#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
int main(){
    int n,s;
    cin >> n >> s;
    int a,b;
    cin >> a >> b;
    vector<pair<int,int>> data;
    for(int i = 0;i < n;i ++){
        int x,y;
        cin >> x >> y;
        data.push_back({y,x});//力气 距离
    }
    sort(data.begin(),data.end());
    int c = s;
    int cnt = 0;
    for(int i = 0;i < data.size();i ++){
        if(c >= data[i].first && data[i].second <= (a + b)){
            c -= data[i].first;
            cnt ++;
        }
    }
    cout << cnt;
}
