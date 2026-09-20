#include<iostream>
#include<stack>
#include<vector>
#include<string>
#include<cctype>
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
    vector<string> ans;
    bool needOperand = true;

    cout << "请输入整数中缀表达式：";
    getline(cin, s);

    for(int i = 0;i < (int)s.size();i ++){
        char ch = s[i];

        if(ch == ' ') continue;

        if(isdigit(ch)){
            if(!needOperand){
                cout << "错误：两个操作数之间缺少运算符" << endl;
                return 0;
            }

            string number;
            while(i < (int)s.size() && isdigit(s[i])){
                number += s[i];
                i ++;
            }
            i --;

            ans.push_back(number);
            needOperand = false;
        }
        else if(ch == '('){
            if(!needOperand){
                cout << "错误：左括号前缺少运算符" << endl;
                return 0;
            }
            st.push(ch);
            needOperand = true;
        }
        else if(ch == ')'){
            if(needOperand){
                cout << "错误：右括号前缺少操作数" << endl;
                return 0;
            }

            while(!st.empty() && st.top() != '('){
                ans.push_back(string(1, st.top()));
                st.pop();
            }
            if(st.empty()){
                cout << "错误：括号不匹配" << endl;
                return 0;
            }
            st.pop();
            needOperand = false;
        }
        else if(ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^'){
            if(needOperand){
                cout << "错误：运算符位置错误" << endl;
                return 0;
            }

            while(!st.empty() && st.top() != '(' &&
                  (priority(st.top()) > priority(ch) ||
                  (priority(st.top()) == priority(ch) && ch != '^'))){
                ans.push_back(string(1, st.top()));
                st.pop();
            }
            st.push(ch);
            needOperand = true;
        }
        else{
            cout << "错误：有非法字符" << endl;
            return 0;
        }
    }

    if(ans.empty()){
        cout << "错误：表达式不能为空" << endl;
        return 0;
    }
    if(needOperand){
        cout << "错误：表达式末尾缺少操作数" << endl;
        return 0;
    }

    while(!st.empty()){
        if(st.top() == '('){
            cout << "错误：括号不匹配" << endl;
            return 0;
        }
        ans.push_back(string(1, st.top()));
        st.pop();
    }

    stack<long long> numbers;
    for(int i = 0;i < (int)ans.size();i ++){
        if(isdigit(ans[i][0])){
            numbers.push(stoll(ans[i]));
        }
        else{
            if(numbers.size() < 2){
                cout << "错误：操作数数量不正确" << endl;
                return 0;
            }
            long long b = numbers.top();
            numbers.pop();
            long long a = numbers.top();
            numbers.pop();
            long long value = 0;
            if(ans[i] == "+") value = a + b;
            else if(ans[i] == "-") value = a - b;
            else if(ans[i] == "*") value = a * b;
            else if(ans[i] == "/"){
                if(b == 0){
                    cout << "错误：除数不能为0" << endl;
                    return 0;
                }
                value = a / b;
            }
            else if(ans[i] == "^"){
                value = 1;
                if(b < 0){
                    cout << "错误：指数不能为负数" << endl;
                    return 0;
                }
                for(int j = 0;j < b;j ++) value *= a;
            }
            numbers.push(value);
        }
    }

    if(numbers.size() != 1){
        cout << "错误：表达式缺少运算符" << endl;
        return 0;
    }

    cout << "后缀表达式：";
    for(int i = 0;i < (int)ans.size();i ++) cout << ans[i] << ' ';
    cout << endl;
    cout << "表达式的值：" << numbers.top() << endl;
}
