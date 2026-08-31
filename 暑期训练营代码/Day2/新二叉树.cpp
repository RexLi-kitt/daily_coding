#include<iostream>
#include<string>
using namespace std;

char left_child[128] = {0};
char right_child[128] = {0};

// 前序遍历
void preorder(char cur) {
    if (cur == 0 || cur == '*') return;
    cout << cur;
    preorder(left_child[cur]);
    preorder(right_child[cur]);
}

int main() {
    int n;
    cin >> n;
    string temp;
    char root = 0;
    for (int i = 0; i < n; i++) {
        cin >> temp;
        char parent = temp[0];
        char left_c = temp[1];
        char right_c = temp[2];
        if (i == 0) root = parent; 
        if (left_c != '*') left_child[parent] = left_c;
        if (right_c != '*') right_child[parent] = right_c;
    }
    preorder(root);
    return 0;
}