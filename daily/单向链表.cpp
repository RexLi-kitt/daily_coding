#include<iostream>
#include<vector>
using namespace std;
class node{
    public:
        int value;
        node* nxt;
        node(int v,node* n){value = v;nxt = n;}
        
        void push_back(node* n){
            if(this->nxt == nullptr){
                this->nxt = n;
            }
            else{
                n->nxt = this->nxt;
                this->nxt = n;
            }
        }
        int query(){
            if(this->nxt == nullptr){
                return 0;
            }
            else{
                return this->nxt->value;
            }
        }

        void delete_back(){
            node* a = this->nxt;
            if(a == nullptr) return;
            this->nxt = a->nxt;
            a->nxt = nullptr;
        }

};
node begin1(1,nullptr);
vector<node> q;
void init(){
    for(int i = 0;i <= 1000005;i ++){
        q.push_back(node(i,nullptr));
    }
}
node* find(int x){
    return &q[x];
}
int main(){
    int n;
    cin >> n;
    init();
    for(int i = 0;i < n;i ++){
        int op;
        cin >> op;
        if(op == 1){
            int x,y;
            cin >> x >> y;
            q[x].push_back(&q[y]);
        }
        if(op == 2){
            int x;
            cin >> x;
            node* a = find(x);
            cout << a->query() <<endl;
        }
        if(op == 3){
            int x;
            cin >> x;
            node* a = find(x);
            a->delete_back();
        }
    }
}