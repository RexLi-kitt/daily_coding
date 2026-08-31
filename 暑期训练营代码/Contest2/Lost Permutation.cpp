#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
int main(){
    int t;
    cin >> t;
    while(t--){
        int m,s;
        cin >> m >> s;
        int par_sum = 0;
        int maxde = 0;
        for(int i = 0;i < m;i ++){
            int temp;
            cin >> temp;
            par_sum += temp;
            maxde = max(maxde,temp);
        }
        int n = maxde;
        bool ok = 0;
        while(1){
            int sum = n * (n + 1) / 2;
            if(sum - par_sum == s){
                ok = true;
                break;
            }
            else if(sum - par_sum > s){
                break;
            }
            n ++;
        }
        if(ok){
            cout << "YES";
        }
        else{
            cout << "NO";
        }
        cout << endl;

    }
}