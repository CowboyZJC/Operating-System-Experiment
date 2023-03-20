#include<iostream>
#include<cstdlib>
#include<queue>
using namespace std;
class page_struct{
    public:
        int pagecontrol;
        int inmemory;
        int used_information;
        page_struct(){
            pagecontrol = -1;
            inmemory = 0;
            used_information = 0;
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
    int ap,pp,total;
    cout<<"请输入allocation page:";
    cin>>ap;
    cout<<"请输入process page:";
    cin>>pp;
    cout<<"请输入总的序列数:";
    cin>>total;
    page_struct page[pp];
    pagecontrol_struct pagecontrol[ap];
    int last = 0;
    queue<int> main;
    int diseffect = 0, position;
    for(int i=0;i<total;i++){
        main.push(rand()%pp);
    }
    cout<<"随机序列为：";
    while(main.size()>0){
        int judge=0;
        if(last==0);            //判断是否为第一次分配
        else{
            for(int i=0;i<last;i++)page[pagecontrol[i].page].used_information++;     //将每个已分配的内存页计数 
        }
        position = main.front();
        cout<<position<<" ";
        main.pop();
        for(int i=0;i<last;i++){
            if(position != pagecontrol[i].page)judge=0; //判断是否在内存中
            else{
                judge = 1;
                break;                          //使用的页面已经在内存中
            }
        }
        if(judge){
            page[position].used_information=0;
            continue;
        }                              //在内存中则不需要再分配，命中成功, 并且该页面计数器置0
        else{
            judge = 0;
            diseffect++;
            /*for(int i=0;i++;i<ap){
                if(pagecontrol[i].used);        //判断是否都被占用
                else{
                    judge = 1;
                    break;
                }                               //未占满的情况，并进行分配
            }*/
            if(last<ap){                                  //有空闲
                pagecontrol[last].page = position;
                pagecontrol[last].used = 1;
                page[position].pagecontrol = last;
                page[position].inmemory = 1;
                page[position].used_information = 0; 
                last++;                                        //指向的是空单元
            }
            else{
                int j=0;
                for(int i=0;i<last;i++){
                    if(page[pagecontrol[i].page].used_information>page[pagecontrol[j].page].used_information)j=i;
                }
                page[pagecontrol[j].page].pagecontrol = -1;         //无空闲，需要页置换
                page[pagecontrol[j].page].inmemory = 0;
                page[pagecontrol[j].page].used_information = 0;
                page[position].pagecontrol = j;
                page[position].inmemory = 1;
                page[position].used_information = 0;
                pagecontrol[j].page = position;
                pagecontrol[j].used = 1;
            }
        }
    }
    cout<<endl;
    float minzhong;
    minzhong = (1.00-(1.00*diseffect)/total)*100;               //计算命中率
    cout<<"命中率："<<minzhong<<"%"<<endl;
    return 0;
}