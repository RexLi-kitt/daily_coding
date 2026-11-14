#include<iostream>
#include<vector>
#include <queue>
#include<string>
#include<unordered_map>
using namespace std;
unordered_map<string,string> table;
class node{
    public:
    long long value;
    string label;
    node* Lchild;
    node* Rchild;
    virtual ~node() = default;
    node(int v) : value(v), label(""), Lchild(nullptr), Rchild(nullptr) {}
        node() {
        value = 0;
        label = "";
        Lchild = nullptr;
        Rchild = nullptr;
        }
    void add_Suffix(string a){
        label.insert(0, a); 
        if(this->Lchild != nullptr){
            this->Lchild->add_Suffix(a);
        }
        if(this->Rchild != nullptr){
            this->Rchild->add_Suffix(a);
        }
    }
    node(node* a,node* b){//权值较小的节点作为左子树（标记为 0）
        this->value = a->value + b->value;
        if(a->value <= b->value){
           this->Lchild = a; 
            a->add_Suffix("0");
           this->Rchild = b; 
           b->add_Suffix("1");
        } 
        else{
           this->Lchild = b; 
           b->add_Suffix("0");
           this->Rchild = a;
           a->add_Suffix("1");
        } 
    }
};
class root : public node{
    public:
    string word;
    root(string name,int v):word(name),node(v){}
};
class compare{
    public:
    bool operator()(const node *a,const node* b)const{
        return a->value > b->value;
    }
};
void traverse(node* a){
    root* leaf = dynamic_cast<root*>(a);
    if(leaf != nullptr){
        table[leaf->word] = leaf->label;
        return;
    }
    else{
        if(a->Lchild != nullptr)traverse(a->Lchild);
        if(a->Rchild != nullptr)traverse(a->Rchild);
    }
}
int main(){
    int n;
    cin >> n;
    priority_queue<node*,vector<node*>,compare> q;
    vector<string> s;
    for(int i = 0;i < n;i ++){
        string temp;
        long long f;
        cin >> temp;
        cin >> f;
        root* leaf = new root(temp, f);
        s.push_back(temp);
        q.push(leaf);
    }
    node* fa;
    while(!q.empty()){
        if(q.size() == 1){
            fa = q.top();
            break;
        }
        node* a = q.top();
        q.pop();
        node* b = q.top();
        q.pop();
        node* c = new node(a,b);
        q.push(c);
    }
    if (n == 1) {
        fa->label = "0";
    }
    traverse(fa);
    for(int i = 0;i < n;i ++){
        string temp = s[i];
        cout << temp <<" "<< table[temp] <<endl;
    }
}