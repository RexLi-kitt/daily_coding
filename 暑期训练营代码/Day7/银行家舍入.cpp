#include<iostream>
#include<vector>
#include<string>
using namespace std;

// 检查小数部分5后面是否还有非零数字
bool hasNonZeroAfter(const string& xiaoshu, int start) {
    for (int i = start; i < (int)xiaoshu.size(); i++) {
        if (xiaoshu[i] != '0') return true;
    }
    return false;
}

int main() {
    int n;
    cin >> n;
    vector<string> data(n);
    vector<string> normal(n), banker(n);
    
    for (int i = 0; i < n; i++) {
        cin >> data[i];
        int len = data[i].size();
        bool before_dot = true;
        string zhengshu = "";
        string xiaoshu = "";
        
        for (int j = 0; j < len; j++) {
            if (data[i][j] == '.') {
                before_dot = false;
                continue;
            }
            if (before_dot) {
                zhengshu += data[i][j];
            } else {
                xiaoshu += data[i][j];
            }
        }
        
        // 如果没有小数部分或小数部分为空，原样输出
        if (xiaoshu.empty()) {
            normal[i] = zhengshu;
            banker[i] = zhengshu;
            continue;
        }
        
        char first_dec = xiaoshu[0];
        long long num = stoll(zhengshu);
        
        // 普通四舍五入
        if (first_dec >= '5') {
            normal[i] = to_string(num + 1);
        } else {
            normal[i] = to_string(num);
        }
        
        // 银行家舍入
        if (first_dec > '5') {
            // > 5 直接进位
            banker[i] = to_string(num + 1);
        } else if (first_dec < '5') {
            // < 5 直接舍去
            banker[i] = to_string(num);
        } else {
            // first_dec == '5'
            // 检查5后面是否还有非零数字
            if (hasNonZeroAfter(xiaoshu, 1)) {
                // 5后面还有非零数字，说明 > 0.5，进位
                banker[i] = to_string(num + 1);
            } else {
                // 恰好是 .5，银行家舍入：向偶数舍入
                if (num % 2 == 0) {
                    // 偶数，不进位
                    banker[i] = to_string(num);
                } else {
                    // 奇数，进位变成偶数
                    banker[i] = to_string(num + 1);
                }
            }
        }
    }
    
    // 输出结果：第一行普通四舍五入，第二行银行家舍入
    for (int i = 0; i < n; i++) {
        if (i > 0) cout << " ";
        cout << normal[i];
    }
    cout << endl;
    for (int i = 0; i < n; i++) {
        if (i > 0) cout << " ";
        cout << banker[i];
    }
    cout << endl;
    
    return 0;
}