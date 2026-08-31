#include<iostream>
 #include<vector>
 #include<stack>
#include<set>
 #include<map>
 using namespace std;
 int main(){
    int t;
    cin >> t;
    for(int z = 0;z < t;z ++){
        int n;
        cin >> n;
        if (n == 3) {
            int cntFirst[105] = {0}, cntLast[105] = {0};
            int a, b;
            for (int i = 0; i < 3; i++) {
                cin >> a >> b;
                cntFirst[a]++;
                cntLast[b]++;
            }
            int first = -1, last = -1, mid = -1;
            for (int i = 1; i <= 3; i++) {
                if (cntFirst[i] == 2) first = i;
                if (cntLast[i] == 2) last = i;
                if (cntFirst[i] == 1 && cntLast[i] == 1) mid = i;
            }
            cout << first << " " << mid << " " << last << "\n";
            continue;
        }
        map<pair<int,int>,int> cnt;
        set<pair<int,int>> exi;
        for(int i = 0;i < n;i ++){
            vector<int> cur_d(n - 1);
            for(int j = 0;j < n - 1;j ++){
                cin >> cur_d[j];
            }
            for(int j = 0;j < n - 2;j ++){
                pair<int,int>p = make_pair(cur_d[j],cur_d[j + 1]);
                cnt[p]++;
                auto it = exi.find(p);
                if(it == exi.end()) exi.insert(p);
            }
        }
        vector<pair<int,int>> tuo_tuple;        
        int rudu[105] = {0};            
        int nxt[105] = {0};
        for(auto elem : exi){
            if(cnt[elem] == n - 2){
                tuo_tuple.push_back(elem);
                rudu[elem.second] ++;
                nxt[elem.first] = elem.second;
            }
        }
        int start = -1;
        for(int i = 1;i <= n;i ++){
            if(rudu[i] == 0){
                start = i;
                break;
            }
        }
        int cur = start;
        for(int i = 0;i < n;i ++){
            cout << cur <<" ";
            cur = nxt[cur];
        }
        cout << endl;
    }
 }