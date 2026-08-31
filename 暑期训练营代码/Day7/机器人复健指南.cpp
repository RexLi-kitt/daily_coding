#include <bits/stdc++.h>
using namespace std;

// 方向数组，代表8个“日”字跳法
int dx[8] = {-2, -1, 1, 2, 2, 1, -1, -2};
int dy[8] = {1, 2, 2, 1, -1, -2, -2, -1};

// 用于BFS的节点结构体
struct Node {
    int x, y, step;
};

int main() {
    int n, k, x, y;
    cin >> n >> k;
    cin >> x >> y;

    // 访问标记数组，初始为false
    vector<vector<bool>> visited(n + 1, vector<bool>(n + 1, false));
    queue<Node> q;

    // 初始化：起点入队并标记
    q.push({x, y, 0});
    visited[x][y] = true;
    int ans = 1; // 起点算一个可达格子

    while (!q.empty()) {
        Node cur = q.front();
        q.pop();

        // 如果当前步数已达上限，则不再扩展
        if (cur.step == k) {
            continue;
        }

        // 尝试8个方向移动
        for (int i = 0; i < 8; i++) {
            int nx = cur.x + dx[i];
            int ny = cur.y + dy[i];

            // 检查新位置是否在网格内且未被访问过
            if (nx >= 1 && nx <= n && ny >= 1 && ny <= n && !visited[nx][ny]) {
                visited[nx][ny] = true;
                q.push({nx, ny, cur.step + 1});
                ans++; // 发现一个新格子
            }
        }
    }

    cout << ans << endl;
    return 0;
}