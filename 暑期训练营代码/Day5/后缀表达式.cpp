#include<iostream>
#include<vector>
#include<string>
#include<stack>
using namespace std;
int main(){
    string temp;
    getline(cin ,temp);
    stack<int> q;
    string num = "";
    int ans = 0,prv = 0;
    for(char u : temp){
        if(u == '@'){
            break;
        }
        if(u >= '0' && u <= '9'){
            num.push_back(u);
        }
        else if(u == '.'){
            int x = stoi(num);
            q.push(x);
            num.clear();
        }
        if(u == '+' || u == '-' || u == '*' || u == '/'){
            int a = q.top();
            q.pop();
            int b = q.top();
            q.pop();
            if(u == '+'){
                q.push(a + b);
            }
            else if(u == '-'){
                q.push(b - a);
            }
            else if(u == '*'){
                q.push(a * b);
            }
            else if(u == '/'){
                q.push(b / a);
            }
        }
    }
    cout << q.top();
}