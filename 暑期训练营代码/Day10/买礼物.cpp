#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
const int N = 11451;
int f[N] = {0};
int n = 0 , m = 0;
void dsu_init(){
    for(int i = 0;i <= n;i ++) f[i] = i;
}
int F(int x){   
    if(f[x] == x) return x;
    else return f[x] = F(f[x]);
}
void unify(int x , int y){
    x = F(x) , y = F(y);
    if(x == y) return;
    f[x] = y;
}
vector<pair<int , pair<int , int>>> vec;

int main(){
    int value;
    cin >> value >> n;
    vector<vector<int>> data(n + 1,vector<int>(n + 1,0));
    for(int i = 1;i <= n;i ++){
        for(int j = 1;j <= n;j ++){
            cin >> data[i][j];
        }
    }
    dsu_init();
    for(int i = 1;i <= n;i ++){
        vec.push_back({value , {0 , i}});
    }
    for(int i = 1;i <= n;i ++){
        for(int j = i + 1;j <= n;j ++){
            if(data[i][j] > 0){
                vec.push_back({data[i][j] , {i , j}});
            }
        }
    }
    sort(vec.begin() , vec.end());
    int ans = 0;
    for(auto &p : vec){
        int x = p.second.first , y = p.second.second , z = p.first;
        if(F(x) != F(y)){
            unify(x , y);
            ans += z;
        }
    }
    cout << ans << '\n';
    return 0;
}