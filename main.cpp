#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<cmath>
#include<set>
#include<map>
#include<fstream>
#include<sstream>
#include<cstdlib>
#include<ctime>
#include<cstring>
#include<queue>
#include<stack>
#include<list>
using namespace std;
//读取文件内容
typedef pair<int,int> point;
typedef pair<point,point>line; 
typedef vector<point>ploygon;
typedef vector<ploygon> layer;
struct Marker{
    int x1,y1;
    int x2,y2;
};
struct Tamplate{
    vector<layer>layers;
    Marker marker;
} tmplate_file;//模板图
struct tmplate_map{
    Marker mark;
    vector<layer>layers;
};
void readTmplatefile(string filename){//读取模板文件内容
    string str;string s;
    ifstream in(filename);
    while(getline(in,s)){
        str += s;
        str += "\n";
    }
    in.close();
    for(int i=0;i<(int)str.size();i++){
        if(str[i] == ':'){
            string tmps="";//一层中所有的图形
            int j=i+2;
            while(j<(int)str.size() && str[j]!= 'l'){
                if(str[j] == ')'||str[j] == '(');
                else tmps += str[j];
                j++;
            }
            i=j;
            //处理每一层图形
            int flag=1;//判断正负号
            int num=0;
            vector<int>nums;nums.clear();
            ploygon poly;poly.clear();//图形
            layer layer;layer.clear();
            //一层中所有的图形
            for(int k=0;k<(int)tmps.size();k++){
                if(tmps[k] == '\n'){
                    nums.push_back(flag*num);
                    flag=1;
                    num=0;
                    int x,y;
                    for(int l=0;l<(int)nums.size();l+=2){
                        x = nums[l];
                        y = nums[l+1];
                        poly.push_back(point(x,y));
                    }
                    layer.push_back(poly);
                    nums.clear();poly.clear();
                }
                else{
                    if(tmps[k]==','){
                        nums.push_back(flag*num);
                        flag=1;
                        num=0;
                    }
                    else{
                        if(tmps[k] == '-') flag=-1;
                        else num = num*10 + (tmps[k]-'0');
                    }
                }
            }
            tmplate_file.layers.push_back(layer);
        }
    }
}
void printTmplate(){//打印模板图
    for(int i=0;i<(int)tmplate_file.layers.size();i++){
        cout<<"layer "<<i<<":"<<endl;
        for(int j=0;j<(int)tmplate_file.layers[i].size();j++){
            cout<<"ploygon "<<j<<":"<<endl;
            for(int k=0;k<(int)tmplate_file.layers[i][j].size();k++){
                cout<<tmplate_file.layers[i][j][k].first<<","<<tmplate_file.layers[i][j][k].second<<endl;
            }
        }
    }
}
int main()
{
    //读取tmplate文件内容
    readTmplatefile("C:/Users/29415/Desktop/eda2024/public/small/small_case.txt");
    //打印模板图
    printTmplate();
    //读取待匹配图内容
    return 0;
}