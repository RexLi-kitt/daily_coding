#include<iostream>
#include<stdio.h>
#include<algorithm>
#include<math.h>
#include<vector>
using namespace std;
void paint(vector<vector<char> > &graph,int n,int x0,int y0,int x1,int y1){
    if(n == 1){
        graph[x0][y0] = '/';
        graph[x1][y1] = '/';
        graph[x0][y0 + 1] = '\\';
        graph[x1][y1 + 1] = '_';
        graph[x1][y1 + 2] = '_';
        graph[x1][y1 + 3] = '\\';
        return;
    }
    else{
        paint(graph, n - 1,(x0 + x1) / 2 + 1,(y0 + y1) / 2,x1,y1);//左下
        paint(graph, n - 1,x0, y0, (x1 + x0) / 2,(y0 + y1) / 2 + 1);//中上
        paint(graph, n - 1,x0 + (1 << (n - 1)),y0 + (1 << (n - 1)),x1,y1 + (1 << n));//右下
    }
}
int main(){
    int n;
    cin >> n;
    vector<vector<char> > figure((1 << n) + 1,vector<char>((1 << (n + 1)) + 1, ' '));
    paint(figure,n,1,1 << n,1 << n,1);
    for(int i = 1;i <= (1 << n);i ++){
        for(int j = 1;j <= (1 << (n + 1));j ++){
            cout << figure[i][j];
        }
        cout << '\n';
    }
}