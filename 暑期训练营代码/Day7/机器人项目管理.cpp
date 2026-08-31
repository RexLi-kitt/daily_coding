#include<iostream>
#include<vector>
#include<algorithm>
#include<iomanip>
using namespace std;
int main(){
    int n,m;
    cin >> n >> m;
    vector<double>dp(m + 1);
    vector<vector<int>> normal_data,flex_data;
    double time = 0;
    for(int i = 0;i < n;i ++){
        int o,t,a,b; // t 每项任务耗时 a 咖啡数 b 节省时间
        cin >> o >> t >> a >> b;
        time += t;
        if(o == 0){
            flex_data.push_back({o,t,a,b});
        }
        else{
            normal_data.push_back({o,t,a,b});
        }
    }
    // 按效率 b/a 降序排列 flex_data
    sort(flex_data.begin(), flex_data.end(), [](const vector<int>& x, const vector<int>& y) {
        return (double)x[3] / x[2] > (double)y[3] / y[2];
    });

    int k = normal_data.size();
    for(int i = 0;i < k;i ++){
    for(int v = m;v >= normal_data[i][2];v --){
            dp[v] = max(dp[v],dp[v - normal_data[i][2]] + normal_data[i][3]);
        }
    }
    double max_saved_time = 0;
    for(int i = 0;i <= m;i ++){
        double saved_time = dp[i];
        double rem = m - i;
        for(int j = 0;j < flex_data.size() && rem > 0;j ++){
            if(rem >= flex_data[j][2]){
                rem -= flex_data[j][2];
                saved_time += flex_data[j][3];
            }
            else{
                double proportion = rem / flex_data[j][2];
                saved_time += proportion * flex_data[j][3];
                rem = 0;
            }
        }
        max_saved_time = max(max_saved_time,saved_time);
    }
    cout << fixed << setprecision(6) << time - max_saved_time;
    return 0;
}
