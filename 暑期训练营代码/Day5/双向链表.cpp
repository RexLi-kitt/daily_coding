#include<iostream>
#include<algorithm>
#include<vector>
#include<cstring>
using namespace std;
class node{
    public:
        int value;
        node* nxt;
        node* prv;
        node(int x):value(x),nxt(nullptr),prv(nullptr){}
};
    void insert_before(node* p,node* q){//...- q - p - ... 第一个参数是中心 
        node* temp = p->prv;
        if(temp == nullptr){
            q ->nxt = p;
            p->prv = q;
        }
        else{
        temp->nxt = q;
        q->prv = temp;
        q->nxt = p;
        p ->prv = q;            
        }
    }   
    void insert_after(node* p,node* q){// ... - p - q - ...
        node* temp = p->nxt;
        if(temp == nullptr){
            p->nxt = q;
            q->prv = p;
        }else{
        p->nxt = q;
        q->nxt = temp;
        temp->prv = q;
        q->prv = p;            
        }
    }    
    void delete_node(node* p){
        if(p->prv == nullptr && p->nxt != nullptr){
            node* temp = p->nxt;
            temp->prv = nullptr;
            p->nxt = nullptr;
        }
        else if(p->prv != nullptr && p->nxt == nullptr){
            node* temp = p->prv;
            temp->nxt = nullptr;
            p->prv = nullptr;
        }
        else if(p ->prv == nullptr && p -> nxt == nullptr){
            return;
        }
        else{
            node* prevoiuss = p->prv;
            node* nextt = p->nxt;
            prevoiuss->nxt = nextt;
            nextt->prv = prevoiuss;
            p->prv = nullptr;
            p->nxt = nullptr;
        }
    }
    void full_insert_after(node* a,node*b){
        delete_node(b);
        insert_after(a,b);
    }
    void full_insert_before(node* a,node* b){
        delete_node(b);
        insert_before(a,b);
    }


int main(){
    int n,m;
    cin >> n >> m;
    node* memo[n + 1];
    bool deleted[n + 1];
    memset(deleted, false, sizeof(deleted));
    for(int i = 1;i <= n;i ++){
        memo[i] = new node(i);
        if(i == 1) continue;
        else{
            memo[i]->prv = memo[i - 1];
            memo[i - 1]->nxt = memo[i];
        }   
    }
    node* que = nullptr;
    for(int i = 0; i < m;i ++){
        int a, b, c;
        cin >> a >> b;
        if(a == 1){
            cin >> c;
            if(b == c) continue;
            else{
                full_insert_before(memo[c],memo[b]);
            }
        }
        if(a == 2){
            cin >> c;
            if(b == c) continue;
            else{
                full_insert_after(memo[c],memo[b]);
            }
        }
        if(a == 3){
            delete_node(memo[b]);
            deleted[b] = true;
        }
    }
    
    for(int i = 1;i <= n;i ++){
        if(!deleted[i] && memo[i]->prv == nullptr){
            que = memo[i];
            break;
        }
    }    
    if(que == nullptr){
        cout << "Empty!";
        return 0;
    } 
    while (que->prv != nullptr)
    {
       que = que->prv; 
    }
    while(que != nullptr){
        cout << que->value <<" ";
        que = que->nxt;
    }
}