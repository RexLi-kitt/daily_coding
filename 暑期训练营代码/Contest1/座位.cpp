#include <iostream>
#include<vector>
#include<algorithm>
using namespace std;
int main() {
    int n, m;
    cin >> n >> m;
    vector<int> a(n * m);
    for (int i = 0; i < n * m; i++) {
        cin >> a[i];
    }
    int xiaor = a[0];                 // 小R的成绩
    sort(a.begin(), a.end(), greater<int>());
    int rank = 0;
    for (int i = 0; i < n * m; i++) {
        if (a[i] == xiaor) {
            rank = i + 1;             //小R的名次
            break;
        }
    }
    int col = (rank - 1) / n + 1;     
    int offset = (rank - 1) % n;      
    int row;
    if (col % 2 == 1) {
        row = offset + 1;             // 奇数列从上往下
    } else {
        row = n - offset;             // 偶数列从下往上
    }
    cout << col << " " << row << endl;
    return 0;
}