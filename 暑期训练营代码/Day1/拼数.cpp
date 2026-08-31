#include<iostream>
#include<vector>
#include<algorithm>
#include<string>
using namespace std;
int main(){
    int n;
    cin >> n;
    vector<string> data;
    for(int i = 0;i < n;i ++){
        string temp;
        cin >> temp;
        data.push_back(temp);
    }
    sort(data.begin(),data.end(),[](const string& a,const string& b){
        return a + b > b + a;
    });
    for(int i = 0;i < data.size();i ++){
        cout <<data[i];
    }
    cout << endl;
    return 0;
}