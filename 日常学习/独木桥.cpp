#include<iostream>
#include<vector>
using namespace std;
int main(){
    int l,n;
    cin >> l >> n;
    vector<int> pos(n);
    for(int i = 0;i < n;i ++){
        cin >> pos[i];
    }
    vector<int> a(n),b(n);    int ans = 0;
    int bas = 0;
    for(int i = 0;i < n;i ++){
        a[i] = min(abs(pos[i]),l + 1 - pos[i]);
        ans = max(ans,a[i]);
        b[i] = max(abs(pos[i]),l + 1 - pos[i]);
        bas = max(bas,b[i]);
    }
    cout << ans << " " << bas;
}