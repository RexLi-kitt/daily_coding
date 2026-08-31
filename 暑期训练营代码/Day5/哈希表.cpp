#include <cstdio>
#include <cctype>
using namespace std;

typedef unsigned long long ull;

// 快速读入
char buf[1<<23], *p1=buf, *p2=buf;
#define gc() (p1==p2&&(p2=(p1=buf)+fread(buf,1,1<<21,stdin),p1==p2)?EOF:*p1++)
inline ull rd() {
    ull x = 0;
    char ch = gc();
    while (!isdigit(ch)) ch = gc();
    while (isdigit(ch)) x = x*10 + (ch^48), ch = gc();
    return x;
}

// splitmix64 哈希函数，抗 hack
inline ull splitmix64(ull x) {
    x += 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    x = x ^ (x >> 31);
    return x;
}

const int SZ = 1 << 23;      // 8,388,608 个槽位
const int MASK = SZ - 1;

ull keys[SZ];
ull vals[SZ];
bool occ[SZ];

int main() {
    int n = (int)rd();
    ull sum = 0;

    for (int i = 1; i <= n; ++i) {
        ull x = rd();
        ull y = rd();

        ull h = splitmix64(x);
        int pos = h & MASK;

        // 线性探查找 key
        while (occ[pos] && keys[pos] != x) {
            pos = (pos + 1) & MASK;
        }

        if (occ[pos]) {
            // key 已存在，答案是旧值
            sum += (ull)i * vals[pos];
        }
        // else: key 不存在，ans = 0，sum 不变

        // 更新映射
        keys[pos] = x;
        vals[pos] = y;
        occ[pos] = true;
    }

    printf("%llu\n", sum);
    return 0;
}