#include<iostream>
#include<vector>
#include<string>
#define ull unsigned long long
using namespace std;
const int N = 1e6 + 5;
int m,n,l1,r1,l2,r2;
ull base = 233,pw[N];
ull h[N];
ull get(int l,int r){
    return h[r] - h[l - 1] * pw[r - l + 1];
}

int main(){
    string temp;
    cin >> temp;
    pw[0] = 1;
    int n = temp.size();
    for(int i = 1; i <= n; i++){
        pw[i] = pw[i - 1] * base;
        h[i] = h[i - 1] * base + temp[i - 1];
    }
    int m;
    cin >> m;
    for(int i = 1;i <= m;i ++){
        cin >> l1 >> r1 >> l2 >> r2;
        if(r1 - l1 != r2 - l2){
            cout << "No" << endl;
        }
        else if(get(l1,r1) != get(l2,r2)){
            cout << "No" << endl;
        }
        else{
            cout << "Yes" <<endl;
        }
    }
}