#include <iostream>
#include <vector>
using namespace std;

int main(){
    int n, k;
    cin >> n >> k;
    vector<int> target(n + 1, 0);        // target[u]: 块 u 应该去的位置
    vector<bool> occupied(n + 1, false); // 该位置当前是否有块
    int pos = 1;
    for (int i = 0; i < k; i++){
        int s; cin >> s;
        for (int j = 0; j < s; j++){
            int u; cin >> u;
            occupied[u] = true;
            target[u] = pos++;           // 按文件顺序依次分配目标位
        }
    }

    int misplaced = 0, cycles = 0;
    vector<bool> vis(n + 1, false);
    for (int u = 1; u <= n; u++){
        if (!occupied[u] || target[u] == u) continue; // 空块或已就位：跳过
        misplaced++;
        if (vis[u]) continue;
        int cur = u;
        while (!vis[cur]){                // 块编号 = 其当前位置，故下一块就是 target[cur]
            vis[cur] = true;
            cur = target[cur];
            if (!occupied[cur]) break;    // 链的尽头是空块：不是环
        }
        if (cur == u) cycles++;           // 走回起点：找到一个环
    }

    if (misplaced == 0) cout << "No optimization needed." << endl;
    else cout << "We need " << misplaced + cycles << " move operations." << endl;
    return 0;
}
