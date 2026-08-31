#include <iostream>
#include <string>
using namespace std;

int main() {
    string pre, post;
    cin >> pre >> post;          // 读取前序和后序遍历字符串
    
    unsigned long long ans = 1;  // 结果可能很大，用无符号64位整数
    int n = pre.size();
    
    for (int i = 0; i < n - 1; ++i) {
        // 取出相邻的两个字符
        string pair = pre.substr(i, 2);          // 例如 "ab"
        string rev = string(1, pair[1]) + pair[0]; // 逆序 "ba"
        
        // 在后序序列中查找逆序对
        if (post.find(rev) != string::npos) {
            ans <<= 1;   // 等价于 ans *= 2
        }
    }
    
    cout << ans << endl;
    return 0;
}