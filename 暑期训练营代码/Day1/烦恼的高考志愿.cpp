#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
int main(){
    int m,n;
    cin >> m >> n;
    vector<int>school;
    for(int i = 0;i < m;i ++){
        int temp;
        cin >> temp;
        school.push_back(temp);
    }
    sort(school.begin(),school.end());
    vector<int> student;
    for(int i = 0;i < n;i ++){
        int temp;
        cin >> temp;
        student.push_back(temp);
    }
    long long ans = 0;
    for(int i = 0;i < n;i ++){
        int q = student[i];
        int pos = lower_bound(school.begin(), school.end(), q) - school.begin();
        if(pos == 0){
            ans += abs(q - school[0]);
        }
        else if(pos == m){
            ans += abs(q - school[m - 1]);
        }
        else{
            ans += min(abs(q - school[pos]), abs(q - school[pos - 1]));
        }
    }
    cout << ans << endl;
    return 0;
}
