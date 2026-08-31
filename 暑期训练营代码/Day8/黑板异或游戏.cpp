#include <vector>
using namespace std;

class Solution {
public:
    bool xorGame(vector<int>& nums) {
        int xorSum = 0;
        for (int num : nums) {
            xorSum ^= num;
        }
        // 如果初始异或值为0，Alice直接获胜；
        // 否则如果数组长度为偶数，Alice获胜，否则失败。
        return xorSum == 0 || nums.size() % 2 == 0;
    }
};