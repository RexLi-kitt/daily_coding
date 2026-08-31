#include<iostream>
#include<vector>
#include<stack>
#include<cmath>
using namespace std;
int main(){
    int n;
    cin >> n;
    vector<int> x(n);
    vector<int> y(n);
    for(int i = 0;i < n;i ++){
        cin >> x[i] >> y[i];
    }
    if( n <= 2){
        cout << n;
        return 0;
    }
    int ans = 0;
    int cnt = 0;
    for(int i = 0;i < n;i ++){
        for(int j = i + 1;j < n;j ++){
            int x1 = x[i];
            int y1 = y[i];
            int x2 = x[j];
            int y2 = y[j];
            pair<int,int> vec_a = {x2 - x1,y2 - y1};
            for(int k = 0;k < n;k ++){
                pair<int,int> vec_b = {x[k] - x2,y[k] - y2};
                if(vec_b.second * vec_a.first == vec_b.first * vec_a.second){
                    cnt ++;
                }
            }
        ans = max(ans,cnt);
        cnt = 0;    
        }
        
    }
    cout << ans;
}