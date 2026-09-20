#include<iostream>
#include<stack>
using namespace std;

int priority(char ch){
    if(ch == '^') return 3;
    if(ch == '*' || ch == '/') return 2;
    if(ch == '+' || ch == '-') return 1;
    return 0;
}

int main(){
    string s;
    stack<char> st;
    string ans;

    cout << "请输入中缀表达式：";
    cin >> s;

    for(int i = 0;i < (int)s.size();i ++){
        char ch = s[i];

        if((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9')){
            ans += ch;
        }
        else if(ch == '('){
            st.push(ch);
        }
        else if(ch == ')'){
            while(!st.empty() && st.top() != '('){
                ans += st.top();
                st.pop();
            }
            if(st.empty()){
                cout << "错误：括号不匹配" << endl;
                return 0;
            }
            st.pop();
        }
        else if(ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^'){
            while(!st.empty() && st.top() != '(' &&
                  (priority(st.top()) > priority(ch) ||
                  (priority(st.top()) == priority(ch) && ch != '^'))){
                ans += st.top();
                st.pop();
            }
            st.push(ch);
        }
        else{
            cout << "错误：有非法字符" << endl;
            return 0;
        }
    }

    while(!st.empty()){
        if(st.top() == '('){
            cout << "错误：括号不匹配" << endl;
            return 0;
        }
        ans += st.top();
        st.pop();
    }

    cout << "后缀表达式：" << ans << endl;
}