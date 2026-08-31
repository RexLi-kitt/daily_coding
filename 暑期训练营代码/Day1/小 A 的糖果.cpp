#include<iostream>
#include<vector>
using namespace std;
int main(){
    int n,m;
    cin >> n >>m;
    vector<int> data;
    for(int i = 0;i < n;i ++){
        int temp;
        cin >> temp;
        data.push_back(temp);
    }
    long long eaten = 0;
    for(int i = 0;i < n - 1;i ++){
        if(data[i] + data[i + 1] <= m){
            continue;
        }
        else{
            eaten += data[i] + data[i + 1] - m;
            if(data[i + 1] < data[i] + data[i + 1] - m){
                data[i + 1] = 0;
                data[i] = m;
            }
            else{
                data[i + 1] -= data[i] + data[i + 1] - m;
            }
        }
    }
    cout << eaten;
}