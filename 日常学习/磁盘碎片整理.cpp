#include<iostream>
#include<vector>
#include<cstring>
using namespace std;
int main(){
    int n,k;
    cin >> n >> k;
    vector<vector<int>> storage(k + 1);
    vector<bool> vis(n + 1,true);
    for(int i = 0;i < k;i ++){
        int t;
        cin >> t;
        for(int j = 0;j < t;j ++){
            int temp;
            cin >> temp;
            vis[temp] = false;
            storage[i].push_back(temp);
        }
    }
    int right_idx = 1,mov = 0;
    for(int i = 1;i <= k;i ++){
        for(int j = 0;j < storage[i].size();j ++,right_idx++){
            int u = storage[i][j];
            if(vis[right_idx]){
                vis[right_idx] = false;
                vis[u] = true;
                mov++;
            }
            else{
                vis[u] = true;
                mov += 2;
            }
        }
    }
    if(mov == 0){
        cout <<"No optimization needed.";
    }
    else{
        cout << "We need ";
        cout << mov;
        cout << " move operations.";
    }
}
