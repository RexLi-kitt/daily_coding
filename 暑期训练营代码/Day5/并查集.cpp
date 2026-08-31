#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

class tree {
private:
    int value;
    int rank;
    tree* father;

public:
    tree(int v = 0,tree* f = nullptr, int r = 0) : value(v), father(f), rank(0){}

    void set_father(tree *f){
        tree* base = this->get_base_root();
        tree* base_f = f->get_base_root();
        if(base == base_f) return;
        if(base->rank >= base_f->rank){
            base_f->father = this->get_base_root();
            if(base->rank == base_f->rank) base->rank ++;
        }
        else{
            base->father = f->get_base_root();
        }
    }

    tree* get_base_root() {
        if(father == nullptr) return this;
        father = father -> get_base_root();
        return father;
    }

    bool equals(tree &a){
        return (a.value == this->value);
    }

    bool check_same_base_root(tree &a) {
    return this->get_base_root() == a.get_base_root();
    }

};
int main() {
    int n,m;
    cin >> n >> m;
    vector<tree*> data;
    for(int i = 0;i <= n;i ++){
        data.push_back(new tree(i,nullptr,0));
    }
    for(int i = 0;i < m;i ++){
        int op,y,z;
        cin >>op >>y >>z;
        tree* node_y = data[y];
        tree* node_z = data[z];
        if(op == 1){
            node_y->set_father(node_z);
        }
        else{
        if(node_y->check_same_base_root(*node_z)){  
            cout << "Y" << endl;
        } else {
            cout << "N" << endl;
        }
        }
    }
}
