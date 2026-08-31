#include<iostream>
#define ll long long
using namespace std;
int main(){
    int t;
    cin >> t;
    while(t --){
        int l,r,x;
        cin >> l >> r >> x;
        int a,b;
        cin >> a >> b;
        if(a == b){
            cout << 0;
        }
        else if(abs(a - b) >= x){
            cout << 1;
        }
        else if((abs(l - a) >= x && abs(l - b) >= x) ||( abs(r - b) >= x && abs(r - a) >= x)){
            cout << 2;
        }
        else if(abs(l - a) >= x && abs(l - r) >= x && abs(r - b) >= x){
            cout << 3;
        }
        else if(abs(r - a) >= x && abs(r - l) >= x && abs(l - b) >= x){
            cout << 3;
        }
        else{
            cout << -1;
        }
        cout << endl;
    }
}