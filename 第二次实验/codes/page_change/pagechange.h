#ifndef PAGECHANGE_H
#define PAGECHANGE_H
#include<iostream>
#include<cstdlib>
#include<queue>
using namespace std;
class page_struct{
    public:
        int pagecontrol;
        int inmemory;
        int used_information;
        page_struct();
};
class pagecontrol_struct{
    public:
        int used;
        int page; 
        pagecontrol_struct();
};
int FIFO(queue<int> main,int ap,int pp);
int FIFO(int main[],int ap,int pp,int total);
int LRU(queue<int> main,int ap,int pp);
int LRU(int main[],int ap,int pp,int total);
#endif