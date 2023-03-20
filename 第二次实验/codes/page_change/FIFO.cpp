#include "pagechange.h"
int FIFO(queue<int> main,int ap,int pp){
    int diseffect = 0,first = 0,last = 0,position = 0;
    page_struct page[pp];
    pagecontrol_struct pagecontrol[ap+1];
    while(main.size()>0){
        int judge=0;
        position = main.front();
        main.pop();
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
    return diseffect;
};
int FIFO(int main[],int ap,int pp,int total){
    int diseffect = 0,first = 0,last = 0,position = 0,pos = 0;
    page_struct page[pp];
    pagecontrol_struct pagecontrol[ap+1];
    while(pos<total){
        int judge=0;
        position = main[pos];
        pos++;
        for(int i = first%(ap+1);(i%(ap+1))<last;i++){
            if(position != pagecontrol[i%(ap+1)].page)judge=0; //判断是否在内存中
            else{
                judge = 1;
                break;                          //使用的页面已经在内存中
            }
        }
        if(judge)continue;                              //在内存中则不需要再分配，命中成功
        else{
            judge = 0;
            diseffect++;
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
    return diseffect;
}