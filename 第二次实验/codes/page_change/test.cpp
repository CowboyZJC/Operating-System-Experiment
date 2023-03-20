#include "pagechange.h"
int main(){
    int ap,pp,total;
    cout<<"请输入allocation page:";
    cin>>ap;
    cout<<"请输入process page:";
    cin>>pp;
    cout<<"请输入总的序列数:";
    cin>>total;
    cout<<"随机序列为：";
    queue<int> main;
    for(int i=0;i<total;i++){
        main.push(rand()%pp);
        cout<<main.back()<<" ";
    }
    cout<<endl;
    int diseffect1 = FIFO(main,ap,pp);
    int diseffect2 = LRU(main,ap,pp);
    float minzhong1,minzhong2;
    minzhong1 = (1.00-(1.00*diseffect1)/total)*100;               //计算命中率
    minzhong2 = (1.00-(1.00*diseffect2)/total)*100; 
    cout<<"FIFO命中率:"<<minzhong1<<"%"<<endl;
    cout<<"LRU命中率:"<<minzhong2<<"%"<<endl;
    return 0;
}