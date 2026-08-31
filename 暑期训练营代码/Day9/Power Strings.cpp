#include<iostream>
#include<vector>
#include<string>
#include<limits>
using namespace std;

int main(){
    int cnt = 0;
    while(1){
        string a;
        cin >> a;
        if(a == ".") break;
        else{
            for(int i = 1;i <= a.size();i ++){
                string ora;
                bool sp_len = 1;
                for(int j = 0;j < i;j ++){
                    ora += a[j];
                }
                for(int k = 0;k < a.size(); k+= ora.size()){
                    string comp = a.substr(k,ora.size());
                    if(!(comp == ora)){
                        sp_len = 0;
                        break;
                    }
                }
                if(sp_len){
                    cout << a.size() / i << endl;
                    break;
                }
            }
        }
    }
}