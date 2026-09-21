#include<vector>
#include<iostream>
#include<string>
using namespace std;
string constructString(string a,int pos){
    string ans = "";
    for(int i = 0;i < a.size();i ++){
        if(i == pos){
            continue;
        }
        ans += a[i];
    }  
    return ans;
}
bool compare(string a,string b){
    for(int i = 0;i < a.size();i ++){
        if(a[i] - '0' == b[i] - '0') continue;
        else{
            return a[i] > b[i];
        }
    }
    return false;
}
string f(string a,int k){
    string cur_s = "";
    string mov = a;
    for(int i = 1;i <= k;i ++){
        for(int j = 0;j < a.size() - i;j ++){
            cur_s += '9';
        }
        for(int j = 0;j < mov.size();j ++){
            string s = constructString(mov,j);
            if(compare(cur_s,s)){cur_s = s;}
        }
        mov = cur_s;
        cur_s.clear();
    }
    return mov;
}
int main(){
    string data;
    cin >> data;
    int k;
    cin >> k;
    //这样思考：每轮拿掉一个数字，得到当前最小
    string ans = f(data,k);
    if (ans == "0") {
        cout << ans;
        return 0;
    }
    string r_ans = "";
    bool before_zero = true;
    for(int i = 0;i < ans.size();i ++){
        if(before_zero && ans[i] == '0'){
            continue;
        }
        else if(ans[i] == '0' && !before_zero){
            r_ans += ans[i];
        }
        else if(ans[i] != '0'){
            before_zero = false;
            r_ans += ans[i];
        }
    }
    cout << r_ans;
}