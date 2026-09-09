#include <vector>
#include <iostream>
using namespace std;

int main() {
    int r, c, n;
    cin >> r >> c;

    vector<string> g(r);
    vector<vector<bool>> vis(r, vector<bool>(c, false));

    for (int i = 0; i < r; i++) {
        cin >> g[i];
        for (int j = 0; j < c; j++)
            if (g[i][j] == '*') vis[i][j] = true;   // 起点也是“当前可能位置”
    }

    cin >> n;
    for (int t = 0; t < n; t++) {
        string d;
        cin >> d;
        int dr = 0, dc = 0;
        if (d[0] == 'N') dr = -1;
        else if (d[0] == 'S') dr = 1;
        else if (d[0] == 'E') dc = 1;
        else if (d[0] == 'W') dc = -1;

        // 本阶段所有可能的出发位置
        vector<pair<int,int>> src;
        for (int i = 0; i < r; i++)
            for (int j = 0; j < c; j++)
                if (vis[i][j]) src.push_back({i, j});

        // 沿方向一格一格走，每个能停的格子都作为本阶段的新位置（从下一格起，不含原地）
        vector<vector<bool>> nvis(r, vector<bool>(c, false));
        for (auto &p : src) {
            int i = p.first + dr, j = p.second + dc;
            while (i >= 0 && i < r && j >= 0 && j < c && g[i][j] != 'X') {
                nvis[i][j] = true;
                i += dr;
                j += dc;
            }
        }
        vis.swap(nvis);   // 本阶段结果作为下一阶段出发点
    }

    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++)
            cout << (g[i][j] == 'X' ? 'X' : (vis[i][j] ? '*' : '.'));
        cout << '\n';
    }
    return 0;
}