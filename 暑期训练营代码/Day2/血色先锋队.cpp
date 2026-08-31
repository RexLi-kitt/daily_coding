#include<string>
#include<iostream>
#include<algorithm>
#include<vector>
#include<queue>
using namespace std;
void bfs(vector<vector<int>> &graph,vector<pair<int,int>> &src){
    queue<pair<int,int>> q;
    for(auto &s :src){
        q.push(s);
        graph[s.first][s.second] = 0;
    }
    while(!q.empty()){
        auto [u,v] = q.front();
        q.pop();
        int next = graph[u][v] + 1;
        if(u - 1 >= 1 && graph[u - 1][v] == -1){
            graph[u - 1][v] = next;
            q.push({u - 1,v});
        }        
        if(u + 1 < graph.size() && graph[u + 1][v] == -1){
            graph[u + 1][v] = next;
            q.push({u + 1,v});
        }
        if(v - 1 >= 1 && graph[u][v - 1] == -1){
            graph[u][v - 1] = next;
            q.push({u,v - 1});
        }
        if(v + 1 < graph.size() && graph[u][v + 1] == -1){
            graph[u][v + 1] = next;
            q.push({u,v + 1});
        }
    }
}
int main(){
    int n,m,a,b;
    cin >> n >> m >> a >> b;
    vector<vector<int>> graph(n + 1,vector<int>(m + 1,-1));
    vector<pair<int,int>> src;
    for(int i = 0;i < a;i ++){
        int a , b;
        cin >> a >> b;
        src.push_back({a,b});
        graph[a][b] = 0;
    }
    bfs(graph,src);
    for(int i = 0;i < b;i ++){
        int x,y;
        cin >> x >> y;
        cout << graph[x][y] <<'\n';
    }
}