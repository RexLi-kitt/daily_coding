#include<algorithm>
#include<vector>
#include<string>
#include<map>
#include<unordered_map>
#include<iostream>
using namespace std;
int main(){
    int n,m;
    cin >> n >> m;
    vector<vector<int>> graph(n + 1,vector<int>(m + 1));
    graph[1][1] = 1;
    for(int i = 1;i <= n;i ++){
        for(int j = 1;j <= m;j ++){
            if(i == 1 && j == 1) continue;
            if(i % 2 == 0 && j % 2 == 0){
                continue;
            }
            else{
                graph[i][j] = graph[i - 1][j] + graph[i][j - 1];
            }
        }
    }
    cout << graph[n][m];
}