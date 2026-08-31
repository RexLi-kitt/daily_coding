#include<iostream>
#include<vector>
#include<iomanip>
#include<algorithm>
using namespace std;
int main(){
    int n,t;
    cin >> n >>t;
    vector<pair<double,double>> data;
    for(int i = 0;i < n;i ++){
        double a,b;
        cin >> a>> b;
        data.push_back({b,a});//价值 重量
    }
    for(int i = 0;i < n;i ++){
        double temp = data[i].first;
        data[i].first = temp / data[i].second;
    }

    sort(data.begin(),data.end(),greater<pair<double,double>>());
    int c = t;
    double sum = 0;
    for(int i = 0;i < n;i ++){
        if(c >= data[i].second){
            c -= data[i].second;
            sum += data[i].first * data[i].second;
        }
        else{
            sum += data[i].first * c;
            break;
        }
    }
    cout << fixed << setprecision(2)<<sum;
}