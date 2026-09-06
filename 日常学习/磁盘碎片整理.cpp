#include<iostream>
#include<vector>
#include<cstring>
using namespace std;
int main(){
    int n,k;
    cin >> n >> k;
    vector<vector<int>> storage(k + 1);
    vector<vector<int>> supposed(k + 1);
    int cnt = 1;
    vector<pair<int,int>> occupied(n + 1);
    for(int i = 0;i < k;i ++){
        int t;
        cin >> t;
        for(int j = 0;j < t;j ++){
            int temp;
            cin >> temp;
            storage[i + 1].push_back(temp);
            supposed[i + 1].push_back(cnt++);
            occupied[temp] = {i + 1,j};
        }
    }
    int mov = 0;
    for(int i = 1;i <= k;i ++){
        for(int j = 0;j < storage[i].size();j ++){
            int u = storage[i][j];
            int right_idx = supposed[i][j];
            if(right_idx == u) continue;
            pair<int,int> vid = {0,0};
            if(occupied[right_idx] == vid){
                occupied[right_idx] = occupied[u];
                occupied[u] = {0,0};
                mov++;
            }
            else{
                //先考虑称环的情况
                auto[z,v] = occupied[right_idx];
                int his_target = supposed[z][v];
                if(his_target == u){
                    mov += 2;
                    occupied[u] = {0,0};
                    occupied[right_idx] = {i,j};
                }
                else{
                    mov ++;
                    occupied[right_idx] =occupied[u];
                    occupied[u] = {0,0};
                }
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
