#include<iostream>
#include<vector>
#include<iomanip>
#include<string>
using namespace std;
int f(string a){
    int ans = 0;
    int carry = 1;
    for(int i = a.size() - 1;i >= 0;i --){
        ans += (a[i] - '0') * carry;
        carry *= 10;
    }
    return ans;
} 
int main(){
    string data;
    cin >> data;
    int apprx = 0, b = 0;
    bool check_l = true;
    bool fuhao = true;
    char pp;
    string temp;
    for(int i = 0;i < data.size();i ++){
        char cur = data[i];
        if(cur == '='){
        if(!temp.empty()){                 // 结算 '=' 左边最后一个整数
        int formal = f(temp);
        if(!fuhao) formal *= -1;
        b += formal;                   // 此时仍是左端
         }
        temp.clear();
        fuhao = true;
        check_l = false;
        continue;
        }
        if(cur >= '0' && cur <= '9'){
            temp.push_back(cur);
        }
        if(cur >= 'a' && cur <= 'z'){
            pp = cur;
            int formal = temp.empty() ? 1 : f(temp);
            temp.clear();
            if(!fuhao) formal *= -1;
            fuhao = true;
            if(check_l){
                apprx += formal;
            }
            else{
                apprx -= formal;
            }
        }
        if(cur == '+'){
            int formal = f(temp);
            temp.clear();
            if(!fuhao) formal *= -1;
            fuhao = true;
            if(check_l) b += formal;
            else b -= formal;
        }
        if(cur == '-'){
            int formal = f(temp);
            temp.clear();
            if(!fuhao) formal *= -1;
            fuhao = false;
            if(check_l) b += formal;
            else b -= formal;
        }
    }
    if(!temp.empty()){
        int formal = f(temp);
        if(!fuhao) formal *= -1;
        if(check_l) b += formal;
        else b -= formal;
    }
    double ans = (double)b / (double)apprx * (-1);
    if(ans == 0) ans = 0;
    cout << pp <<"=" << fixed << setprecision(3) << ans;
}