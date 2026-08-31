#include<iostream>
#include<vector>
#include<stack>
using namespace std;
double calcs(int x0,int y0,int x1,int y1,int x2,int y2){
    pair<double,double> vec_a = {x1 - x0,y1 - y0};
    pair<double,double> vec_b = {x2 - x0,y2 - y0};
    return (vec_a.first * vec_b.second - vec_a.second * vec_b.first);
}
int main(){
    int n;
    cin >> n;
    vector<int> x(n),y(n);
    for(int i = 0;i < n;i ++){
        cin >> x[i] >> y[i];
    }
    int x0 = x[0];
    int y0 = y[0];
    double s = 0;
    for(int i = 1;i < n - 1;i ++){
        s += calcs(x0,y0,x[i],y[i],x[i + 1],y[i + 1]);
    }
    cout << abs(s) / 2.0;
}