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
typedef pair<int,int> point;
typedef pair<point,point>line;//first->second;
struct polygon{
    vector<line>lines;
    int x_min=1e9,x_max=-1e9,y_min=1e9,y_max=-1e9;//相当于marker
};
//     layer
//       |
//     polygon（template_file中的图形为绝对的坐标，pattern中的图形为相对（marker）坐标）
//       |
//     line+最小边框（template_file中的marker为最小能包住图形的的，pattern中的图形为相对（marker）坐标)
//       |
//     pa(x1,y1)->pb(x2,y2)
//       |
//     逆时针方向
//     
//     
typedef vector<polygon> layer;
struct Template_file{
    vector<layer>layers;
    int x_max=-1e9,y_max=-1e9,x_min=1e9,y_min=1e9;
}template_file;
struct pattern{
    vector<layer>layers;
    int x=0,y=0;//marker的坐标->x,y为右上角
};
vector<pattern>patterns;//pattern的集合
//
bool point_in_marker(line marker[],point p){
    int x=p.first,y=p.second;
    point p1=marker[0].first,p2=marker[1].second;
    int x1=p1.first,y1=p1.second,x2=p2.first,y2=p2.second;
    if(x1<=x&&x2>=x&&y1<=y&&y2>=y){
        return true;
    }
    return false;
}
void read_template_file(string filename){//读取模板文件
    string str="";string s;
    ifstream in(filename);
    while(getline(in,s)){
        str+=s;
        str+='\n';
    }
    in.close();
    for(int i=0;i<(int)str.size();i++){
        if(str[i]==':'){//出现一个图层
        string tmps="";
        layer tmp_layer;tmp_layer.clear();//暂时存储该层所有图形
        int x_max=-1e9,y_max=-1e9,x_min=1e9,y_min=1e9;//该层所有图形的最大最小坐标
            int j=i+2;
            while(j<(int)str.size()&&str[j]!='l'){
                if(str[j]==')'||str[j]=='(');
                else tmps+=str[j];
                j++;
            }
            //读取该层所有图形
            i=j;
            int num=0,flag=1;
            vector<int>nums;nums.clear();
            polygon tmp_poly;tmp_poly.lines.clear();
            for(int k=0;k<(int)tmps.size();k++){
                if(tmps[k]=='\n'){//出现一个图形
                    nums.push_back(num*flag);//图形的所有坐标
                    for(int l=0;l<=(int)nums.size()-4;l+=2){
                        int x1=nums[l],y1=nums[l+1],x2=nums[l+2],y2=nums[l+3];
                        line tmp_line;//连线
                        tmp_line.first=point(x1,y1);
                        tmp_line.second=point(x2,y2);
                        tmp_poly.lines.push_back(tmp_line);
                        tmp_poly.x_max=max(tmp_poly.x_max,max(x1,x2));
                        tmp_poly.y_max=max(tmp_poly.y_max,max(y1,y2));
                        tmp_poly.x_min=min(tmp_poly.x_min,min(x1,x2));
                        tmp_poly.y_min=min(tmp_poly.y_min,min(y1,y2));
                        x_max=max(x_max,tmp_poly.x_max);
                        y_max=max(y_max,tmp_poly.y_max);
                        x_min=min(x_min,tmp_poly.x_min);
                        y_min=min(y_min,tmp_poly.y_min);
                    }
                    line l1=line(point(nums[nums.size()-2],nums[nums.size()-1]),point(nums[0],nums[1]));//最后一个点连上初始的点
                    tmp_poly.lines.push_back(l1);
                    nums.clear();//清空这个图形
                    flag=1;
                    num=0;
                    tmp_layer.push_back(tmp_poly);//加入图形到临时层
                    tmp_poly.lines.clear();//清空临时图形
                }//读完一个图形
                else if(tmps[k]==','){
                    nums.push_back(num*flag);
                    flag=1;
                    num=0;
                }
                else{
                    if(tmps[k]=='-'){
                        flag=-1;
                    }
                    else{
                        num=num*10+(tmps[k]-'0');
                    }
                }
            }
            //读完一层的图形了，加入到模板文件中
            template_file.layers.push_back(tmp_layer);
            template_file.x_max=max(template_file.x_max,x_max);
            template_file.y_max=max(template_file.y_max,y_max);
            template_file.x_min=min(template_file.x_min,x_min);
            template_file.y_min=min(template_file.y_min,y_min);
        }
    }
}
void read_pattern_file(string filename){//读取pattern文件
    string str="";string s;
    ifstream in(filename);
    while(getline(in,s)){
        str+=s;
        str+='\n';
    }
    in.close();
    for(int i=0;i<(int)str.size();i++){
        if(str[i]=='p'){//出现一个pattern
            while(str[i]!='\n')i++;
            //开始读入第一个pattern
            int j=i+1;
            string tmps_p="";//读取pattern的字符串
            while(j<(int)str.size()&&str[j]!='p'){
                if(str[j]==')'||str[j]=='(');
                else if(str[j]=='l'){
                    while(str[j]!=':')j++;//str[j]==':'
                    //：为下一层的标志
                    if(tmps_p.size()!=0)tmps_p+=str[j];
                }
                else if(str[j]=='m'){
                    tmps_p+=':';
                    tmps_p+=str[j];
                }
                else{
                    if(tmps_p.size()==0&&str[j]=='\n');
                    else tmps_p+=str[j];
                }
                j++;
            }
            i=j-1;
            //读完第一个pattern
            //开始处理pattern_str_p
            pattern tmp_pattern;
            tmp_pattern.layers.clear();
            layer tmp_layer;
            tmp_layer.clear();
            int num=0,flag=1;
            vector<int>nums;
            nums.clear();
            for(int k=0;k<(int)tmps_p.size();k++){//处理pattern_str_p
                if(tmps_p[k]=='\n'&&tmps_p[k-1]!=':'){//出现一个polygon
                    polygon tmp_poly;
                    tmp_poly.lines.clear();//临时存储该图形的所有线
                    nums.push_back(num*flag);
                    for(int l=0;l<=(int)nums.size()-4;l+=2){
                        int x1=nums[l],y1=nums[l+1],x2=nums[l+2],y2=nums[l+3];
                        line tmp_line;//连线
                        tmp_line.first=point(x1,y1);
                        tmp_line.second=point(x2,y2);
                        tmp_poly.lines.push_back(tmp_line);
                    }
                    line l1=line(point(nums[nums.size()-2],nums[nums.size()-1]),point(nums[0],nums[1]));//最后一个点连上初始的点
                    tmp_poly.lines.push_back(l1);
                    nums.clear();//清空这个图形
                    flag=1;
                    num=0;
                    //加入图形到临时layer
                    tmp_layer.push_back(tmp_poly);
                }
                else if(tmps_p[k]==':'){//一层结束
                    tmp_pattern.layers.push_back(tmp_layer);
                    tmp_layer.clear();
                    //开始读入下一层
                }
                else if(tmps_p[k]==','){
                    nums.push_back(num*flag);
                    flag=1;
                    num=0;
                }
                else if(tmps_p[k]=='m'){//读入marker
                    while(tmps_p[k]!='\n')k++;
                    int g=k+1;
                    int tmp_num=0;
                    vector<int>tmp_nums;tmp_nums.clear();
                    while(g<(int)tmps_p.size()&&tmps_p[g]!='\n'){
                        if(tmps_p[g]==','){
                            tmp_nums.push_back(tmp_num);
                            tmp_num=0;
                        }
                        else{
                            tmp_num=tmp_num*10+(tmps_p[g]-'0');
                        }
                        g++;
                    }
                    tmp_pattern.x=tmp_nums[4];
                    tmp_pattern.y=tmp_nums[5];
                    break;
                }
                else{
                    if(tmps_p[k]=='-'){
                        flag=-1;
                    }
                    else{
                        num=num*10+(tmps_p[k]-'0');
                    }
                }
            }
            //读完pattern
            patterns.push_back(tmp_pattern);
        }
    }

}
void print_template_file(){//查看模板文件
    for(int i=0;i<(int)template_file.layers.size();i++){
        cout<<"Layer "<<i<<":"<<endl;
        for(int j=0;j<(int)template_file.layers[i].size();j++){
            cout<<"Polygon "<<j<<":"<<endl;
            for(int k=0;k<(int)template_file.layers[i][j].lines.size();k++){
                cout<<"("<<template_file.layers[i][j].lines[k].first.first<<","<<template_file.layers[i][j].lines[k].first.second<<") -> ("<<template_file.layers[i][j].lines[k].second.first<<","<<template_file.layers[i][j].lines[k].second.second<<")"<<endl;
            }
        }
   }
}
void print_pattern_file(){//查看pattern文件
    for(int i=0;i<(int)patterns.size();i++){
        cout<<"Pattern "<<i<<":"<<endl;
        for(int j=0;j<(int)patterns[i].layers.size();j++){
            cout<<"Layer "<<j<<":"<<endl;
            for(int k=0;k<(int)patterns[i].layers[j].size();k++){
                cout<<"Polygon "<<k<<":"<<endl;
                for(int l=0;l<(int)patterns[i].layers[j][k].lines.size();l++){
                    cout<<"("<<patterns[i].layers[j][k].lines[l].first.first<<","<<patterns[i].layers[j][k].lines[l].first.second<<") -> ("<<patterns[i].layers[j][k].lines[l].second.first<<","<<patterns[i].layers[j][k].lines[l].second.second<<")"<<endl;
                }
            }
        }
        cout<<"Marker: ("<<patterns[i].x<<","<<patterns[i].y<<")"<<endl;
    }
}
bool is_intersect(line l1,line l2){//判断两条线是否相交
    point p1=l1.first,p2=l1.second,p3=l2.first,p4=l2.second;
    int x1=p1.first,y1=p1.second,x2=p2.first,y2=p2.second,x3=p3.first,y3=p3.second,x4=p4.first,y4=p4.second;
    int d1=(x2-x1)*(y3-y1)-(y2-y1)*(x3-x1);
    int d2=(x2-x1)*(y4-y1)-(y2-y1)*(x4-x1);
    int d3=(x4-x3)*(y1-y3)-(y4-y3)*(x1-x3);
    int d4=(x4-x3)*(y2-y3)-(y4-y3)*(x2-x3);
    if((d1>0&&d2<0)||(d1<0&&d2>0)||(d3>0&&d4<0)||(d3<0&&d4>0)){
        return true;
    }
    return false;
}
point get_line_intersection(line l1,line l2){//求两条线的交点
    point p1=l1.first,p2=l1.second,p3=l2.first,p4=l2.second;
    int x1=p1.first,y1=p1.second,x2=p2.first,y2=p2.second,x3=p3.first,y3=p3.second,x4=p4.first,y4=p4.second;
    int d1=(x2-x1)*(y3-y1)-(y2-y1)*(x3-x1);
    int d2=(x2-x1)*(y4-y1)-(y2-y1)*(x4-x1);
    int d3=(x4-x3)*(y1-y3)-(y4-y3)*(x1-x3);
    int d4=(x4-x3)*(y2-y3)-(y4-y3)*(x2-x3);
    if((d1>0&&d2<0)||(d1<0&&d2>0)||(d3>0&&d4<0)||(d3<0&&d4>0)){
        int x=(d1*(x3-x4)-(x1-x2)*(y3-y4))/((y3-y4)*(x1-x2)-(x3-x4)*(y1-y2));
        int y=(d1*(y3-y4)-(y1-y2)*(x3-x4))/((y3-y4)*(x1-x2)-(x3-x4)*(y1-y2));
        return point(x,y);
    }
    return point(-1,-1);
}
bool point_is_in_polygon(point p,polygon poly){//判断点是否在多边形内
    int x=p.first,y=p.second;
    int x_min=poly.x_min,y_min=poly.y_min,x_max=poly.x_max,y_max=poly.y_max;
    if(x<x_min||x>x_max||y<y_min||y>y_max){
        return false;
    }
    int n=poly.lines.size();
    int wn=0;
    for(int i=0;i<n;i++){
        line l=poly.lines[i];
        point p1=l.first,p2=l.second;
        int x1=p1.first,y1=p1.second,x2=p2.first,y2=p2.second;
        if(y1<=y&&y<y2||y2<=y&&y<y1){
            int d1=(x2-x1)*(y-y1)-(y2-y1)*(x-x1);
            int d2=(x2-x1)*(y-y2)-(y2-y1)*(x-x2);
            if(d1<0&&d2<0){
                wn++;
                }
            else if(d1>0&&d2>0){
                wn--;
            }
        }
    }
    if(wn!=0){
        return true;
    }
    return false;
}
bool polygon_is_in_polygon(polygon poly1,polygon poly2){//判断poly1是否在poly2内
    int x_min=poly2.x_min,y_min=poly2.y_min,x_max=poly2.x_max,y_max=poly2.y_max;
    for(int i=0;i<(int)poly1.lines.size();i++){
        line l1=poly1.lines[i];
        for(int j=0;j<(int)poly2.lines.size();j++){
            line l2=poly2.lines[j];
            if(is_intersect(l1,l2)){
                point p=get_line_intersection(l1,l2);
                if(point_is_in_polygon(p,poly2)){
                    return true;
                }
            }
        }
    }
    return false;
}
pattern maker_get_pattern_from_template(point marker[]){
    line marker_line[4];
    pattern res_pattern;
    res_pattern.layers.clear();
    for(int i=0;i<4;i++){
        if(i==3){
            marker_line[3]={marker[3],marker[0]};
        }
        marker_line[i]={marker[i],marker[i+1]};
    }
    for(int i=0;i<(int)template_file.layers.size();i++){
        layer tmp_layer=template_file.layers[i];
        //切第i层
        layer pushed_layer;pushed_layer.clear();
        for(int j=0;j<(int)tmp_layer.size();j++){//处理图形
            polygon pushed_polygon;pushed_polygon.lines.clear();
            polygon tmp_polygon=tmp_layer[j];
            vector<point>points;points.clear();
            //加入tmp_polygon所有点
            for(int cnt_line=0;i<(int)tmp_polygon.lines.size();i++){
                line tmp_polygon_line=tmp_polygon.lines[cnt_line];
                points.push_back(tmp_polygon_line.second);
            }
            int cnt_points_of_tmp_polygon=points.size();//tmp_polgon中点的个数
            int cnt_point_in_marker=0;//在marker区域中点的个数
            int mi_x=marker[0].first,ma_x=marker[2].first,mi_y=marker[0].second,ma_y=marker[2].second;
            for(int cnt_points=0;cnt_points<(int)points.size();cnt_points++){
                int x_point=points[cnt_points].first,y_point=points[cnt_points].second;
                if(x_point>=mi_x&&x_point<=ma_x&&y_point>=mi_y&&y_point<=ma_y){
                    cnt_point_in_marker++;
                }
            }
            //全部在marker内部
            if(cnt_point_in_marker==cnt_points_of_tmp_polygon){
                tmp_layer.push_back(tmp_polygon);
                continue;
            }
            //全部不在不能说明他不在里面
            //部分在,重新处理tmp_polygon
            //一条一条线开始处理
            vector<line>tmp_polygon_lines=tmp_polygon.lines;
            vector<point>point_to_new_polygon;point_to_new_polygon.clear();
            for(int cnt_line=0;cnt_line<(int)tmp_polygon_lines.size();cnt_line++){
                int cnt_in_marker=0;
                line l=tmp_polygon_lines[cnt_line];
                point p1=l.first,p2=l.second;
                int x1=p1.first,y1=p1.second;int x2=p2.first,y2=p2.second;
                if(x1>=mi_x&&x1<=ma_x&&y1>=mi_y&&y1<=ma_y){
                    if(point_to_new_polygon.back()==p1)continue;
                    point_to_new_polygon.push_back(p1);
                    cnt_in_marker++;
                }
                if(x2>=mi_x&&x2<=ma_x&&y2>=mi_y&&y2<=ma_y){
                    if(point_to_new_polygon.back()==p2)continue;
                    point_to_new_polygon.push_back(p2);
                    cnt_in_marker++;
                }
                if(cnt_in_marker==2){//两个点都在
                    continue;
                }
                else if(cnt_in_marker==0){//两个点都不在里面
                    if(x1==x2){//竖线
                        if(min(y1,y2)<mi_y||max(y1,y2)>ma_y)continue;
                        point_to_new_polygon.push_back({x1,mi_y});
                        point_to_new_polygon.push_back({x1,ma_y});
                    }
                    else{//横线
                        if(min(x1,x2)<mi_x||max(x1,x2)>ma_x)continue;
                        point_to_new_polygon.push_back({mi_x,y1});
                        point_to_new_polygon.push_back({ma_x,y1});
                    }
                }
                else{//只有一个在里面加入这个线和marker的交点
                    if(x1==x2){//竖线
                        point p_to;
                        p_to.first=x1;
                        p_to.second=min(y1,y2)<mi_y?mi_y:ma_y;
                        point_to_new_polygon.push_back(p_to);
                    }
                    else{//横线
                        point p_to;
                        p_to.first=min(x1,x2)<mi_x?mi_x:ma_x;
                        p_to.second=y1;
                        point_to_new_polygon.push_back(p_to);
                    }
                }
            }
            //point_to_new_polygon处理出pushed_polygon中的lines
            for(int cnt_point=0;cnt_point<(int)point_to_new_polygon.size();cnt_point++){
                if(cnt_point==(int)point_to_new_polygon.size()-1){
                    pushed_polygon.lines.push_back({point_to_new_polygon.back(),point_to_new_polygon[0]});
                }
                else{
                    pushed_polygon.lines.push_back({point_to_new_polygon[i],point_to_new_polygon[i+1]});
                }
            }
        }
        res_pattern.layers.push_back(pushed_layer);
    }
    res_pattern.x=marker[2].first;
    res_pattern.y=marker[2].second;
    return res_pattern;
}
int main(){
    //输入部分
    read_template_file("C:/Users/29415/Desktop/eda2024/public/small/small_case.txt");
    read_pattern_file("C:/Users/29415/Desktop/eda2024/public/small/small_pattern.txt");
    //调试输入
    //tmplate_file中右x_max,y_max,x_min,y_min遍历
    //用来遍历tmplate_file中的所有图形
    //pattern_file中x,y为marker的坐标
    for(int i=0;i<(int)patterns.size();i++){//每一个模板
        int id=i+1;//模板的id
        pattern tmp_pattern = patterns[i];
        int xx=tmp_pattern.x,yy=tmp_pattern.y;
        point marker[4];
        int cnt;
        for(int x=template_file.x_min-xx;x<=template_file.x_max+xx;x++){
            for(int y=template_file.y_min-yy;y<=template_file.y_max+yy;y++){
                cout<<x<<" "<<y<<endl;
                marker[0]={x,y};marker[1]={x+xx,y};marker[2]={x+xx,y+yy};marker[3]={x,y+yy};
                auto match_pattern=maker_get_pattern_from_template(marker);
            }
        }
    }
    return 0;
}