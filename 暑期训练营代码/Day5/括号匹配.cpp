#include<iostream>
#include<string>
#include<stack>
using namespace std;
int main(){
    stack<char> q;
    int n;
    bool ans = 1;
    cin >> n;
    cin.ignore();
    for(int i = 0;i < n;i ++){
        string temp;
        getline(cin, temp);
        for(int i = 0;i < temp.size();i ++){
            if(temp[i] == '(' || temp[i] == '[' || temp[i] == '{'){
                q.push(temp[i]);
            }
            else{
                if(q.empty()){
                    ans = 0;
                    break;
                }
                char front = q.top();
                q.pop();
                if(front == '('){
                    ans = (temp[i] == ')');
                }
                else if(front == '['){
                    ans = (temp[i] == ']');
                }
                else if(front == '{'){
                    ans = (temp[i] == '}');
                }
                if(!ans) break;
            }

        }    
        if(!q.empty()){
            ans = 0;
            while(!q.empty()){
                q.pop();
            }
        }
        if(ans) cout << "YES" <<endl;
        else cout << "NO" << endl;
        ans = 1;
    }
}
