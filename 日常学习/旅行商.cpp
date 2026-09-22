#include<iostream>
#include<iomanip>
#include<vector>
using namespace std;
int main(){
    double s,c,l,p0,n;
    cin >> s >> c >> l >> p0 >> n;
    vector<double> dis(n + 2);
    dis[0] = 0;
    vector<double> data(n + 2);
    for(int i = 1;i <= n;i ++){
        double di, pi;
        cin >> di >> pi;
        dis[i] = di;
        data[i] = pi;
    }
    data[0] = p0;
    dis[n + 1] = s;
    data[n + 1] = 114514;
    double ans = 0;
    double pos = 0;
    int cur_g = 0;
    while(pos < s){
        double longest_dis = pos;
        longest_dis += l * c;
        int tar_g = cur_g + 1;
        for(int i = cur_g + 1;i <= n + 1;i ++){
            if(data[i] < data[tar_g] && dis[i] <= longest_dis){
                tar_g = i;
                break;
            }
        }
        ans += (dis[tar_g] - pos) / l * data[cur_g];
        cur_g = tar_g;
        pos = dis[tar_g];
    }
    cout << fixed << setprecision(2) << ans;
}