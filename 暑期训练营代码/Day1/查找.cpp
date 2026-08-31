#include<iostream>
#include<vector>
using namespace std;
int main(){
    int n,m;
    cin >> n >> m;
    vector<int> data;
    for(int i = 0;i < n;i ++){
        int temp;
        cin >> temp;
        data.push_back(temp);
    }
    for(int i = 0;i < m;i ++){
        int value;
        cin >> value;
        int left = 0,right = data.size();
        while(left < right){
            int mid = left + (right - left) / 2;
            if(data[mid] >= value){
                right = mid;
            }
            else{ 
                left = mid + 1;
            }
        }
        if(left < data.size() && data[left] == value){
            cout << left + 1 << " ";
        }
        else{
            cout << -1 << " ";
        }
    }

}