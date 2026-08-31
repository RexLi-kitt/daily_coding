#include<iostream>
#include<algorithm>
#include<vector>
#include<queue>
using namespace std;
int main(){
    int n;
    cin >> n;
    priority_queue<int, vector<int>, greater<int>> q;
    for(int i = 0;i < n;i ++){
        int temp;
        cin >> temp;
        q.push(temp);
    }
    long long suma = 0;
    while(q.size() != 1){
    int a = q.top();
    q.pop();
    int b = q.top();
    q.pop();    
    suma += a + b;
    int sum = a + b;
    q.push(sum);    
    }
    cout << suma;
}