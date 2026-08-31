#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<limits>
#define ull unsigned long long
using namespace std;
const int base = 233;
ull hash_func(string &a){
    ull ans = 0;
    for(auto c : a){
        ans = ans * base + c;
    }
    return ans;
}
int main(){
    map<ull,int> sys;
    int n;
    if(!(cin >> n)){
        return 0;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    for(int i = 0; i < n;i ++){
        string temp;
        if(!getline(cin,temp)){
            continue;
        }
        // 处理 add 命令：必须以 "add " 开头且后面有内容
        if(temp.size() > 4 && temp.substr(0,4) == "add "){
            string t;
            for(int j = 4;j < temp.size();j ++){
                t += temp[j];
            }
            if(t.empty()){
                continue;
            }
            auto num = hash_func(t);
            sys[num] ++;
        }
        // 处理 find 命令：必须以 "find " 开头且后面有内容
        else if(temp.size() > 5 && temp.substr(0,5) == "find "){
            string t;
            for(int j = 5;j < temp.size();j ++){
                t += temp[j];
            }
            if(t.empty()){
                continue;
            }
            auto num = hash_func(t);
            if(sys[num]){
                cout << "yes"<<endl;
            }
            else{
                cout << "no" << endl;
            }
        }
        else{
            continue;
        }
    }
    return 0;
}
