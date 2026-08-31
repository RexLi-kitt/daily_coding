#include<string>
#include<iostream>
#include<algorithm>
#include<vector>
#include<queue>
#include<vector>
using namespace std;
char left_child[128] = {0};
char right_child[128] = {0};
void front(string preorder,string midorder){
    if(preorder.empty()){
        return;
    }
    char root = preorder[0];
    int k = midorder.find(root);
    int n = midorder.size();
    front(preorder.substr(1, k),midorder.substr(0, k));
    front(preorder.substr(1 + k, n - k - 1),midorder.substr(1 + k, n - k - 1));
    cout << root;
}
int main(){
    string preorder,midoreder;
    cin >> preorder;
    cin >> midoreder;
    front(midoreder,preorder);
}