#include<iostream>
#include<cstdlib>
#include<queue>
using namespace std;
class page_struct{
    public:
        int pagecontrol;
        int inmemory;
        page_struct(){
            pagecontrol = -1;
            inmemory = 0;
        }
};
class pagecontrol_struct{
    public:
        int used;
        int page; 
        pagecontrol_struct(){
            page = -1;
            used = 0;
        } 
};
int main(){
    int ap,pp,total_instruction;
    cout<<"请输入allocation page:";
    cin>>ap;
    cout<<"请输入process page: 5"<<endl;
    pp=5;
    cout<<"请输入总的序列数: 12"<<endl;
    total_instruction=12;
    page_struct page[pp];
    pagecontrol_struct pagecontrol[ap+1];
    int first = 0, last = 0;
    //queue<int> main;
    int main[12]={1,2,3,4,1,2,5,1,2,3,4,5};
    int point = 0;
    int diseffect = 0, position;
    //for(int i=0;i<total_instruction;i++){
        //main.push(rand()%pp);
    //}
    cout<<"随机序列为:";
    while(point<12){
        int judge=0;
        position = main[point];
        cout<<position<<" ";
        point++;
        for(int i=0;i<ap+1;i++){
            if(position != pagecontrol[i].page)judge=0; //判断是否在内存中
            else{
                judge = 1;
                break;                          //使用的页面已经在内存中
            }
        }
        if(judge)continue;                              //在内存中则不需要再分配，命中成功
        else{
            judge = 0;
            diseffect++;
            /*for(int i=0;i++;i<ap+1){
                if(pagecontrol[i].used);        //判断是否都被占用
                else{
                    judge = 1;
                    break;
                }                               //未占满的情况，并进行分配
            }*/
            if(first != (last+1)%(ap+1)){                                  //有空闲
                pagecontrol[last].page = position;
                pagecontrol[last].used = 1;
                page[position].pagecontrol = last;
                page[position].inmemory = 1;
                last = (last+1)%(ap+1);            //指向空单元 
            }
            else{
                page[pagecontrol[first].page].pagecontrol = -1;         //无空闲，需要页置换
                page[pagecontrol[first].page].inmemory = 0;
                pagecontrol[first].page = -1;
                pagecontrol[first].used = 0;
                first = (first+1)%(ap+1);           //指向非空单元
                pagecontrol[last].page = position;
                pagecontrol[last].used = 1;
                page[position].pagecontrol = last;
                page[position].inmemory = 1;
                last = (last+1)%(ap+1);            //指向空单元 
            }
        }
    }
    cout<<endl;
    float minzhong;
    minzhong = (1.00-(1.00*diseffect)/total_instruction)*100;               //计算命中率
    cout<<"命中率："<<minzhong<<"%"<<endl;
    return 0;
}