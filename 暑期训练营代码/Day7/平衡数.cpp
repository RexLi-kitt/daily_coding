#include<iostream>
#include<vector>
#include<string>
using namespace std;
int main(){
    int n;
    cin >> n;
    int cnt = 0;
    for(int i = 0;i < n;i ++){
        int temp;
        cin >> temp;
        int a = temp;
        int cnt_zero = 0,cnt_one = 0;
        while(temp != 0){
        if(temp & 1 == 1){
            cnt_one ++;
        } 
        else{
            cnt_zero ++;
        }
        temp >>= 1;            
        }
        if(cnt_zero == cnt_one){
            cnt ++ ;
        }
    }
    cout << cnt;
}