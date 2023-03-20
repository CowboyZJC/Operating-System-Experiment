#include "pagechange.h"
int LRU(queue<int> main,int ap,int pp){
    int last = 0,diseffect = 0,position = 0;
    page_struct page[pp];
    pagecontrol_struct pagecontrol[ap];
    while(main.size()>0){
        int judge=0;
        if(last==0);            //判断是否为第一次分配
        else{
            for(int i=0;i<last;i++)page[pagecontrol[i].page].used_information++;     //将每个已分配的内存页计数 
        }
        position = main.front();
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
    return diseffect;
};
int LRU(int main[],int ap,int pp,int total){
    int last = 0,diseffect = 0,position = 0,pos = 0;
    page_struct page[pp];
    pagecontrol_struct pagecontrol[ap];
    while(pos<total){
        int judge=0;
        if(last==0);            //判断是否为第一次分配
        else{
            for(int i=0;i<last;i++)page[pagecontrol[i].page].used_information++;     //将每个已分配的内存页计数 
        }
        position = main[pos];
        pos++;
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
    return diseffect;
}