#include<iostream>
#include<vector>
#include<queue>
#include<iomanip>
using namespace std;
vector<vector<pair<double,int>>> graph(2005);
vector<double> dis(2005,0.0);
void add(int u,int v,double w){
    graph[u].push_back({w,v});
    graph[v].push_back({w,u});
}
void dijkstra(int u){
    priority_queue<pair<double,int>> pq;
    pq.push({1,u});
    dis[u] = 1.0;
    while(!pq.empty()){
        auto [w,v] = pq.top();
        pq.pop();
        if(w < dis[v]) continue;
        for(auto [t,r] : graph[v]){
            if(dis[r] < t * dis[v]){
                dis[r] = t * dis[v];
                pq.push({dis[r],r});
            }
        }
    }
}
int main(){
    int n,m;
    cin >> n >>m;
    for(int i = 0;i < m;i ++){
        int x,y,z;
        cin >> x >> y >> z;
        double rate = 1.0 - z / 100.0;
        add(x,y,rate);
    }
    int a,b;
    cin >> a >> b;
    dijkstra(a);
    cout << fixed << setprecision(8) << 100.0 / dis[b]; 
}