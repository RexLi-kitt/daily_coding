#include <iostream>
#include <vector>

int main() {
    const int n = 2023;
    const int mod = 2023;
    std::vector<long long> M(n + 1, 0);
    M[0] = 1;
    M[1] = 1;

    for (int i = 2; i <= n; ++i) {
        long long sum = 0;
        for (int j = 0; j <= i - 2; ++j) {
            sum = (sum + M[j] * M[i - 2 - j]) % mod;
        }
        M[i] = (M[i - 1] + sum) % mod;
    }

    std::cout << M[n] << std::endl;
    return 0;
}