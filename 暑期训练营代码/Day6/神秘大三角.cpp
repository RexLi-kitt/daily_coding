#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
using namespace std;
class point{
    public:
        int x,y;
        point(int a,int b){
            this->x = a;
            this->y = b;
        }
};
int abs_cross(point* a,point* b,point* c){
    //a, b是顶点，c是点
    int vec_a_x = c->x - a->x;
    int vec_a_y = c->y - a->y;
    int vec_b_x = c->x - b->x;
    int vec_b_y = c->y - b->y;
    return abs(vec_a_x * vec_b_y - vec_b_x * vec_a_y);
}
bool is_on_segment(point* a, point* b, point* c){
    //判断点c是否在线段ab上（前提：三点共线）
    int min_x = min(a->x, b->x);
    int max_x = max(a->x, b->x);
    int min_y = min(a->y, b->y);
    int max_y = max(a->y, b->y);
    return (c->x >= min_x && c->x <= max_x && c->y >= min_y && c->y <= max_y);
}

int main(){
    vector<point*> data;//储存a,b,c三个点
    for(int i = 0;i < 3;i ++){
        string temp;
        cin >> temp;
        int x, y;
        sscanf(temp.c_str(), "(%d,%d)", &x, &y);
        data.push_back(new point(x,y));
    }
    string temp;
    cin >> temp;
    int x, y;
    sscanf(temp.c_str(), "(%d,%d)", &x, &y);
    point* tag = new point(x,y);
    int zero_cnt = 0;
    int a_b_t = abs_cross(data[0],data[1],tag);
    if(a_b_t == 0) zero_cnt++;
    int a_c_t = abs_cross(data[0],data[2],tag);
    if(a_c_t == 0) zero_cnt++;
    int b_c_t = abs_cross(data[1],data[2],tag);
    if(b_c_t == 0) zero_cnt ++;
    
    if(zero_cnt == 0){
    int sum = abs_cross(data[0],data[1],data[2]);    
    if(a_c_t + a_b_t + b_c_t == sum){
        cout << 1;
        return 0;
    }
    else if(a_c_t + a_b_t + b_c_t != sum){
        cout << 2;
        return 0;
    }
    }
    if(zero_cnt == 1){
        // 点在直线延长线上还是在线段上？
        // 找出与点共线的那条边
        if(a_b_t == 0 && is_on_segment(data[0], data[1], tag))
            cout << 3;
        else if(a_c_t == 0 && is_on_segment(data[0], data[2], tag))
            cout << 3;
        else if(b_c_t == 0 && is_on_segment(data[1], data[2], tag))
            cout << 3;
        else
            cout << 2;  // 在延长线上，算外部
    }
    else if(zero_cnt == 2){
        cout << 4;  // 在顶点上
    }
    else if(zero_cnt == 3){
        cout << 4;
    }
    return 0;
}
