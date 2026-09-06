#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    // next_pos[p] 表示位置 p 上的数据块的目标位置
    // 空位和已经归位的位置保持为 0
    vector<int> next_pos(n + 1, 0);

    int target = 1;
    int mov = 0;

    for (int i = 0; i < k; i++) {
        int t;
        cin >> t;

        for (int j = 0; j < t; j++) {
            int pos;
            cin >> pos;

            if (pos != target) {
                next_pos[pos] = target;
                mov++;  // 每个未归位块至少移动一次
            }
            target++;
        }
    }

    // 0：未访问，1：在当前路径中，2：已处理
    vector<int> state(n + 1, 0);

    for (int start = 1; start <= n; start++) {
        if (next_pos[start] == 0 || state[start] != 0)
            continue;

        int p = start;

        while (p != 0 && state[p] == 0) {
            state[p] = 1;
            p = next_pos[p];
        }

        // 遇到当前路径中的位置，说明形成了环
        if (p != 0 && state[p] == 1) {
            mov++;
        }

        // 将当前路径标记为已处理，避免重复统计
        p = start;
        while (p != 0 && state[p] == 1) {
            state[p] = 2;
            p = next_pos[p];
        }
    }

    if (mov == 0) {
        cout << "No optimization needed.\n";
    } else {
        cout << "We need " << mov << " move operations.\n";
    }

    return 0;
}