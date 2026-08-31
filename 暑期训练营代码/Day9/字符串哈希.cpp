#include<iostream>
#include<cassert>
#include<vector>
#include <algorithm>
#include<string>
using namespace std;
class tree{
    private:
        int node_value;
        tree* left_node, *right_node;//左值小于node_value, 右值大于node_value
        vector<string> same_hash_value;
    public:
        ~tree() {                       
        delete left_node;
        delete right_node;
        }
        tree(int v,string name){
            node_value = v, left_node = nullptr,right_node = nullptr,same_hash_value.push_back(name);}
        void insert_left(tree *node){
            assert(node != nullptr);
            this->left_node = node;
        }
        void insert_right(tree *node){
            assert(node != nullptr);
            this->right_node = node;
        }

        void insert_node(tree* a,tree *node){      //插入节点 
            assert(node != nullptr && a != nullptr);
            if(a->left_node == nullptr && node->node_value < a->node_value){
                a->insert_left(node);
                return;
            }//插到左边
            if(a->right_node == nullptr && node->node_value > a->node_value){
                a->insert_right(node);
                return;
            }//插到右边
            if(node->node_value == a->node_value){
                for(auto u : a->same_hash_value){
                    if(u == node->same_hash_value[0]){
                        delete node;
                        return;//此哈希值和name已经存在，不需要插入
                    }
                }
                a->same_hash_value.push_back(node->same_hash_value[0]);
                delete node;
                return; 
            }//哈希值相等，先检查是否已经存在，然后是否插入name
            if(node->node_value < a->node_value){
                insert_node(a->left_node,node);
            }//递归调用
            else{
                insert_node(a->right_node,node);
            }
        }

        void insert_node(tree* node) {
        insert_node(this, node);  // 包装类，调用原来的递归版本
        }
        
        bool find_value(int hash_value,string name){//检查是否存入树中，注意可能存在的相同哈希值的情况
            if(hash_value == this->node_value){
                auto it = std::find(this->same_hash_value.begin(),this->same_hash_value.end(),name);
                if(it != this->same_hash_value.end()){
                    return true;
                }
                else{
                    return false;
                }
            }
            else{
                if(hash_value > this->node_value){
                    if(this->right_node == nullptr){
                        return false;
                    }
                    return this->right_node->find_value(hash_value,name);
                }
                else{
                    if(this->left_node == nullptr) return false;
                    return this->left_node->find_value(hash_value,name);
                }
            }
        }

        int count(){
            int total = same_hash_value.size();
            if(this->left_node != nullptr){
            total += this->left_node->count();                
            }
            if(this->right_node != nullptr){
            total += this->right_node->count();                
            }
            return total;
        }
};

int hash_function(string a,unsigned long long prime = 31){
    unsigned long long hash_value = 0;
    for(char u : a){
        hash_value = hash_value * prime + (int)u;
    }   
    return hash_value;
}
int main(){
    int n;
    cin >> n;
    //尝试使用二叉树

    string t;
    cin >> t;
    int h_v = hash_function(t) % n;
    tree data(h_v,t);
    for(int i = 1;i < n;i ++){
        string temp;
        cin >> temp;
        int hash_value = hash_function(temp) % n;
        tree* node = new tree(hash_value,temp);
        data.insert_node(node);
    }
    cout <<  data.count();
}