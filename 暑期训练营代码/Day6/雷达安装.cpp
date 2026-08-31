#include<iostream>
#include<vector>
#include<stack>
#include<cmath>
#include<algorithm>
using namespace std;
pair<double,double> calc(int a,int b,int d){
    if(d <= b){
        return{a + sqrt(d * d - b * b),a - sqrt(d * d - b * b)};
    }
}
int main(){
    int n,d;
    cin >> n >> d;
    vector<pair<double,double>> data;
    for(int i = 0;i < n;i ++){
        int x,y;
        cin >> x >> y;
        data.push_back(calc(x,y,d));
    }
    int a = data.size();
    if(a < n){
        cout << -1;
        return 0;
    }
    sort(data.begin(),data.end());
    if(n == 1){
        cout << -1;
        return 0;
    }
    int cnt = 0;
    double prv_x = -100000000;
    for(int i = 0;i < a;i ++){
        if(prv_x >= data[i].second && prv_x <= data[i].first){
            continue;
        }
        else{
            prv_x = data[i].first;
            cnt ++;
        }
    }
    cout << cnt;
}