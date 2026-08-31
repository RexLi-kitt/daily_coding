#include<iostream>
#include<algorithm>
#include<vector>
#include<iomanip>
using namespace std;
int main(){
    int n;
    cin >> n;
    vector<pair<double,double>> data;
    for(double i = 0;i < n;i ++){
        double temp;
        cin >> temp;
        data.push_back({temp,i + 1});
    }
    sort(data.begin(),data.end());
    double sum = 0;
    int i = 0;
    for(auto u : data){
        auto[v,w] = u;
        sum = sum + v * (n - 1 - i);
        i ++;
        cout << w <<" ";
    }
    cout << endl;
    double avl = sum / n;
    cout << fixed << setprecision(2) << avl;
}