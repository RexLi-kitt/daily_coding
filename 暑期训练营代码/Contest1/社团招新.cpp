#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
class person{
    public:
       vector<int> satis;
       vector<int> loss;
        void calc_loss(){
            int maxde = 0;
            for(int i = 0;i < 3;i ++){
                maxde = max(maxde,satis[i]);
            }
            for(int i = 0;i < 3;i ++){
                loss.push_back(abs(satis[i] - maxde));
            }
            sort(loss.begin(),loss.end());
        }
        int min_loss(){
            return loss[1];
        }
        person(int q,int w,int e){
            satis.push_back(q);
            satis.push_back(w);
            satis.push_back(e);
            calc_loss();
        }
        int return_hi(){
            int cur = 0;
            for(int i = 0;i < 3;i ++){
                if(satis[i] > satis[cur]){
                    cur = i;
                }
            }
            return cur;
        }
};
int main(){
    int t;
    cin >> t;
    for(int b = 0;b < t;b ++){ 
        vector<vector<person>> dep(3);
        int n;
        int sum = 0;
        cin >> n;
        vector<person> p;
        for(int i = 0;i < n;i ++){
            int q,w,e;
            cin >> q >> w >> e;
            p.push_back(person(q,w,e));
            int wanted_de = p[i].return_hi();
            dep[wanted_de].push_back(p[i]);
            sum += p[i].satis[wanted_de];
        }
        for(int i = 0;i < 3;i ++){
            if(dep[i].size() > n / 2){
                int len = dep[i].size();
                vector<int> losss;
                for(person pe : dep[i]){
                    losss.push_back(pe.min_loss());
                }
                sort(losss.begin(),losss.end());
                int idx = 0;
                while(len > n / 2){
                    sum -= losss[idx];
                    idx ++;len --;
                }
                break;
            }
        }
        cout << sum << endl;
    }

}