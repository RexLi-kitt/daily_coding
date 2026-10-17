#include <cctype>
#include <iostream>
#include <stack>
#include <string>
#include <vector>
using namespace std;
// ~ 是程序内部使用的一元负号标记。
int priority(char op) {
    if (op == '^') return 4;
    if (op == '~') return 3;
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

bool isBinaryOperator(char op) {
    return op == '+' || op == '-' || op == '*' || op == '/' || op == '^';
}
// 判断当前运算符入栈前，是否应先弹出栈顶运算符。
bool shouldPopOperator(char top, char current) {
    if (top == '(') return false;
    bool higher = priority(top) > priority(current);
    bool sameAndLeft = priority(top) == priority(current) && current != '^';
    return higher || sameAndLeft;
}
// 转换时同时检查语法；失败时通过 error 返回提示。
bool infixToPostfix(const string& expression,vector<string>& postfix,string& error) {
    stack<char> operators;
    bool needOperand = true;
    postfix.clear();
    error.clear();

    for (int i = 0; i < expression.size(); ++i) {
        char ch = expression[i];
        if (ch == ' ') continue;

        if (isdigit(static_cast<unsigned char>(ch))) {
            if (!needOperand) {
                error = "两个操作数之间缺少运算符";
                return false;
            }
            // 将连续数字作为一个完整操作数读入。
            size_t end = i;
            while (end < expression.size() &&
                   isdigit(static_cast<unsigned char>(expression[end]))) {
                ++end;
            }
            postfix.push_back(expression.substr(i, end - i));
            i = end - 1;
            needOperand = false;
        } else if (ch == '+' && needOperand) {
            // 一元正号不改变值，跳过后仍需要操作数。
            continue;
        } else if (ch == '-' && needOperand) {
            operators.push('~');
        } else if (ch == '(') {
            if (!needOperand) {
                error = "左括号前缺少运算符";
                return false;
            }
            operators.push(ch);
            needOperand = true;
        } else if (ch == ')') {
            if (needOperand) {
                error = "右括号前缺少操作数";
                return false;
            }
            while (!operators.empty() && operators.top() != '(') {
                postfix.push_back(string(1, operators.top()));
                operators.pop();
            }
            if (operators.empty()) {
                error = "括号不匹配";
                return false;
            }
            operators.pop();
            needOperand = false;
        } else if (isBinaryOperator(ch)) {
            if (needOperand) {
                error = "运算符位置错误";
                return false;
            }
            while (!operators.empty() && shouldPopOperator(operators.top(), ch)) {
                postfix.push_back(string(1, operators.top()));
                operators.pop();
            }
            operators.push(ch);
            needOperand = true;
        } else {
            error = "有非法字符";
            return false;
        }
    }

    if (postfix.empty()) {
        error = "表达式不能为空";
        return false;
    }
    if (needOperand) {
        error = "表达式末尾缺少操作数";
        return false;
    }
    while (!operators.empty()) {
        if (operators.top() == '(') {
            error = "括号不匹配";
            return false;
        }
        postfix.push_back(string(1, operators.top()));
        operators.pop();
    }
    return true;
}

// 执行一次二元运算，a 是左操作数，b 是右操作数。
bool calculateBinary(long long a, char op, long long b,long long& value, string& error) {
    switch (op) {
        case '+': value = a + b; break;
        case '-': value = a - b; break;
        case '*': value = a * b; break;
        case '/':
            if (b == 0) {
                error = "除数不能为0";
                return false;
            }
            value = a / b;
            break;
        case '^':
            if (b < 0) {
                error = "指数不能为负数";
                return false;
            }
            value = 1;
            for (long long i = 0; i < b; ++i) value *= a;
            break;
        default:
            error = "有非法运算符";
            return false;
    }
    return true;
}

// 根据内部后缀表达式求值；~ 只弹出一个操作数。
bool evaluatePostfix(const vector<string>& postfix,long long& result, string& error) {
    stack<long long> numbers;
    error.clear();
    for (const string& token : postfix) {
        if (!token.empty() && isdigit(static_cast<unsigned char>(token[0]))) {
            numbers.push(stoll(token));
        } else if (token == "~") {
            if (numbers.empty()) {
                error = "操作数数量不正确";
                return false;
            }
            long long value = numbers.top();
            numbers.pop();
            numbers.push(-value);
        } else {
            if (numbers.size() < 2) {
                error = "操作数数量不正确";
                return false;
            }
            long long b = numbers.top();
            numbers.pop();
            long long a = numbers.top();
            numbers.pop();
            long long value = 0;
            if (token.size() != 1) {
                error = "有非法运算符";
                return false;
            }
            if (!calculateBinary(a, token[0], b, value, error)) return false;
            numbers.push(value);
        }
    }

    if (numbers.size() != 1) {
        error = "表达式缺少运算符";
        return false;
    }
    result = numbers.top();
    return true;
}

void printPostfix(const vector<string>& postfix) {
    cout << "后缀表达式：";
    for (const string& token : postfix) {
        // 只调整显示，内部仍用 ~ 区分一元负号和减法。
        cout << (token == "~" ? "-" : token) << ' ';
    }
    cout << '\n';
}

int main() {
    string expression;
    cout << "请输入整数中缀表达式：";
    getline(cin, expression);

    vector<string> postfix;
    long long result = 0;
    string error;
    if (!infixToPostfix(expression, postfix, error) ||
        !evaluatePostfix(postfix, result, error)) {
        cout << "错误：" << error << '\n';
        return 0;
    }

    printPostfix(postfix);
    cout << "表达式的值：" << result << '\n';
    return 0;
}