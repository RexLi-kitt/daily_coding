#include<iostream>
#include<vector>
#include<stack>
#include<cmath>
using namespace std;
int calc(int x1,int y1, int x2,int y2){
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}
int main(){
    int n;
    cin >> n;
    vector<int> x(n);
    vector<int> y(n);
    for(int i = 0;i < n;i ++){
        cin >> x[i];
    }
    for(int i = 0;i < n;i ++){
        cin >> y[i];
    }
    vector<int> radius(n);
    for(int i = 0;i < n;i ++){
        cin >> radius[i];
    }
    int x1, y1, x2, y2;
    cin >> x1 >> y1 >> x2 >> y2;
    int cnt = 0;
    for(int i = 0;i < n;i ++){
        int suma = calc(x1,y1,x[i],y[i]);
        int sumb = calc(x2,y2,x[i],y[i]);
        if(suma < radius[i]){
            cnt ++;
        }
        if(sumb < radius[i]){
            cnt ++;
        }
        if(suma < radius[i] && sumb < radius[i]){
            cnt -= 2;
        }  
    }
    cout << cnt;
}