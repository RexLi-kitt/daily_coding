#include<iostream>
#include<algorithm>
using namespace std;
bool ok(int dist,int *location, int n,int m){
    int move_cnt = 0, last = 0;
    for(int i = 0;i < n + 1;i ++){
        if(location[i] - last < dist){
            move_cnt ++;
        }
        else{
            last = location[i];
        }
    }
    return move_cnt <= m;
}
int main(){
    int l,n,m;
    cin >> l >> n >> m;
    int location[n + 1];
    for(int i = 0;i < n;i ++){
        int temp;
        cin >> temp;
        location[i] = temp;
    }
    location[n] = l;
    int max_dist = 1;
    while(ok(max_dist,location,n,m)){
        max_dist ++;
    }
    cout << max_dist - 1;
}