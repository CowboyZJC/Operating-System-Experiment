## 第二次实验

------------------

### 2.1 进程软中断实验

----------------------- 

1、根据流程图编写程序，猜想一下这个程序的运行结果，然后多次运行，观察Delete/quit键前后，会出现什么结果？分析原因。

根据实验辅导书的参考代码，写成work_1_1.c，多次运行结果如下:

![](C:\Users\Lenovo\Desktop\操作系统\第二次实验\photos\work_1_1.png)

结果分析：

第一次是通过ctrl + c来输入delete信号来让整个父进程直接退出，不输出父进程被杀死信号。第二次是通过输入ctrl + \来输入quit信号来让父进程从sleep状态中唤醒，因为代码中接收3信号，从而父进程输出（子进程不是因为该信号杀死是因为课本中代码根本不是通过信号量杀死，而是自行结束，与父进程无关，且早于父进程），第三次输出是父进程自己从sleep状态中唤醒，从而输出与第二次输出一样的效果。并且从第一次和第二三次输出子进程顺序不一，可以看出两者顺序并不固定。

为了验证上述结果分析中括号中内容，写代码work_1_2.c验证，把kill直接删除，输出结果如下：

![](C:\Users\Lenovo\Desktop\操作系统\第二次实验\photos\work_1_2.png)

结果分析：
结果仍相同，成功验证上述内容

2、如果程序运行界面上显示“Child process 1 is killed by parent !!  Child process 2 is killed by parent !!”，五秒之后显示“Parent process is killed !!”，怎样修改程序使得只有接收到相应的中断信号后再发生跳转，执行输出？

对work_1_1.c中代码进行修改(work_1_3.c)，对子进程均加入该语句：

```c
while(wait_flag);
```

让进程中只有运行stop函数，即wait_flag = 0，才能运行下一步，使用忙等实现

输出结果如下：

![](C:\Users\Lenovo\Desktop\操作系统\第二次实验\photos\work_1_3_2.png)

![](C:\Users\Lenovo\Desktop\操作系统\第二次实验\photos\work_1_3.png)

结果分析：
可以看出修改后子进程要通过接收信号才能结束，并输出。但是这里有一个问题，由于signal(3, stop)语句为父子进程共享，也就是说实际上输入quit信号后，子进程本身就会结束，而不是父进程的kill信号导致结束！然而如果是等待5s后的中断，此时为父进程发出的kill导致子进程结束。

为了验证上述分析，写代码work_1_4.c，将代码中stop修改如下：

```c
void stop(int signum){ 
    wait_flag = 0;         
    printf("\n %d stop \n",signum);                                                                                             
} 
```

输出结果如下：

![](C:\Users\Lenovo\Desktop\操作系统\第二次实验\photos\work_1_4_1.png)

![](C:\Users\Lenovo\Desktop\操作系统\第二次实验\photos\work_1_4_2.png)

![](C:\Users\Lenovo\Desktop\操作系统\第二次实验\photos\work_1_4_3.png)

![](C:\Users\Lenovo\Desktop\操作系统\第二次实验\photos\work_1_4_4.png)

结果分析：

可以发现程序运行有三种结果，第一种是子进程一接收quit信号终止，第二种是子进程一二均是kill发出的信号终止，第三种是都收到quit信号终止。说明如果不做信号屏蔽子进程会收到quit信号，无法确定是否是kill发出的信号才发生跳转。而5s后的中断可以发现只能是kill发出的中断信号，因为此时没有quit信号。

为了实现真正的只有收到相应的信号才发生跳转，且子进程结束有固定顺序，写work_1_5.c，代码如下：

```c
#include <stdio.h> 
#include <signal.h>     
#include <unistd.h>  
#include <sys/types.h>
int wait_flag; 
void stop(int signum); 
int main(){ 
    int pid1, pid2;                         // 定义两个进程号变量  
    signal(3,stop);                            // 或者 signal(14,stop); 
    while((pid1 = fork( )) == -1);        // 若创建子进程1不成功,则空循环
    if(pid1 > 0) {                       // 子进程创建成功,pid1为进程号 
        while((pid2 = fork( )) == -1);    // 创建子进程2      
            if(pid2 > 0) { 
                wait_flag = 1; 
                sleep(5);                          
                printf("\nkill 1!\n"); 
                kill(pid1,16);                     // 杀死进程1
                wait(0);                       // 等待第1个子进程1结束的信号 
                printf("\nkill 2!\n"); 
                kill(pid2,17);                       // 杀死进程2 
                wait(0);                       // 等待第2个子进程2结束的信号 
                printf("\n Parent process is killed !!\n"); 
                exit(0);                                      // 父进程结束 
            } 
            else { 
                signal(3,SIG_IGN);
                wait_flag = 1; 
                signal(17,stop);                           // 等待进程2被杀死的中断号17
                while(wait_flag);                                                                                   
                printf("\n Child process 2 is killed by parent !!\n"); 
                exit(0);                                                                                                    
            }                                                                                                                                                                
    }                                                                                                                    
    else {
        signal(3,SIG_IGN);                                                                                                               
        wait_flag = 1;                                                                                                  
        signal(16,stop);     // 等待进程1被杀死的中断号16  
        while(wait_flag);                                                                                     
        printf("\n Child process 1 is killed by parent !!\n");                                                        
        exit(0); 
    }                                                                                                                    
}   
void stop(int signum){ 
    wait_flag = 0; 
    printf("\n %d stop \n",signum);                                                                                                       
}           
```

输出结果如下：

![](C:\Users\Lenovo\Desktop\操作系统\第二次实验\photos\work_1_5_2.png)

![](C:\Users\Lenovo\Desktop\操作系统\第二次实验\photos\work_1_5_1.png)

结果分析：
如图可以看出程序运行顺序为，父进程先接收到信号或者等待5s后中断，然后发出kill信号, 之后子进程收到后才结束，最后父进程wait函数等到子进程结束再结束，且子进程两个信号区分开，用wait(0)改变来实现顺序结束，完成实验要求。

3、将本实验中通信产生的中断通过14 号信号值进行闹钟中断，将signal(3,stop)当中数字信号变为2，体会不同中断的执行样式，从而对软中断机制有一个更好的理解。

编写代码work_1_6.c，代码如下：

```c
#include <stdio.h> 
#include <signal.h>     
#include <unistd.h>  
#include <sys/types.h>
int wait_flag; 
void stop(int signum); 
int main(){ 
    int pid1, pid2;                           
    signal(2,stop);           
    signal(14,stop);                 
    while((pid1 = fork( )) == -1);        
    if(pid1 > 0) {                        
        while((pid2 = fork( )) == -1);         
            if(pid2 > 0) { 
                wait_flag = 1; 
                alarm(5);
                while(wait_flag);                          
                kill(pid1,14);                     
                kill(pid2,14);                       
                printf("\nkill!\n"); 
                wait(0);                        
                wait(0);                       
                printf("\n Parent process is killed !!\n"); 
                exit(0);                                      
            } 
            else { 
                signal(2,SIG_IGN);
                wait_flag = 1;                        
                while(wait_flag);                                                                                   
                printf("\n Child process 2 is killed by parent !!\n"); 
                exit(0);                                                                                                    
            }                                                                                                                                                                
    }                                                                                                                    
    else {
        signal(2,SIG_IGN);                                                                                                               
        wait_flag = 1;                                                                                                   
        while(wait_flag);                                                                                     
        printf("\n Child process 1 is killed by parent !!\n");                                                        
        exit(0); 
    }                                                                                                                    
}   
void stop(int signum){ 
    wait_flag = 0;     
    printf("\n %d stop \n",signum);                                                                                               
} 
```

输出结果如下：

![](C:\Users\Lenovo\Desktop\操作系统\第二次实验\photos\work_1_6_2.png)

![](C:\Users\Lenovo\Desktop\操作系统\第二次实验\photos\work_1_6.png)

结果分析：
完成实验3内容，说明kill可以通过发送任意信号给子进程接受，从而实现通信，signal中14原本是需要alarm信号才能完成，但是kill也能实现，同时父进程可以通过ctrl+c接收也可以通过alarm接受信号来完成下一步，由于fork（）只进行fork（）后面的代码，所以可以确定子进程不是由alarm的信号来导致结束。

如果将kill删除，并且把alarm(5)置于最外层或者父进程内，即test.c, 发现输出结果如下：

![](C:\Users\Lenovo\Desktop\操作系统\第二次实验\photos\test.png)

结果分析：

即说明alarm信号只由本程序所拥有，与signal函数性质不同，并且验证上述内容。

编写test_1.c，将alarm写在fork()后，输出结果如下：

![](C:\Users\Lenovo\Desktop\操作系统\第二次实验\photos\test_1_1.png)

![](C:\Users\Lenovo\Desktop\操作系统\第二次实验\photos\test_1_2.png)

结果分析：
即成功接收到时钟信号，也同时证明了alarm函数只有本程序会拥有。

实际上该课本给出的代码怎么修改，都会导致忙等，这是不好的，所以对程序修改，最终自己修改的work_1_7.c，代码如下：

```c
#include<stdio.h>
#include<sys/types.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<wait.h>

int pid1,pid2;
void kill_process(){
    kill(pid1, 16);
    kill(pid2, 17);
}
void killed(int signum){
    if(signum == 16){
        printf("Child process 1 is killed by parent!\n");
    }
    else if(signum == 17){
        printf("Child process 2 is killed by parent!\n");
    }
}

int main() 
{   
    while((pid1 = fork()) == -1);
    if(pid1>0) 
    {
        while((pid2 = fork()) == -1);
        if(pid2  >  0) 
        {
            signal(2, kill_process);
            signal(14, kill_process);
            alarm(5);
            wait(NULL);
            wait(NULL);
            printf("Parent process is killed!\n");
            exit(0);
        }
        else 
        {
            signal(2,SIG_IGN);
            signal(17, killed);
            pause();
            exit(0);
        }
    }
    else 
    {
        signal(2,SIG_IGN); //子进程中屏蔽ctrl+c的信号
        signal(16, killed);
        pause();
        exit(0);
    }
    return 0;
}
```

输出结果如下：

![](C:\Users\Lenovo\Desktop\操作系统\第二次实验\photos\work_1_8.png)

结果分析：

该程序使用了pause来等待信号，pause函数是让进程被挂起，等待传递的信号之后才会运行，与sleep类似，这样可以避免忙等，让内存充分利用，是一种更好的实现方式。

### 问题回答：

1. 你最初认为运行结果会怎么样？写出你猜测的结果；
   
   我认为应该是等待5s后，父进程发出信号使子进程结束，子进程结束后立即输出，父进程然后输出。

2. 实际的结果什么样？有什么特点？在接收不同中断前后有什么差别？请将5秒内中断和5秒后中断的运行结果截图，试对产生该现象的原因进行分析。
   
   实际结果是子进程先输出，等待5s后父进程才输出，产生该输出结果的原因是，该程序连等待信号都没有，而父进程在等待5s后，发现子进程已经结束，于是输出并结束。

3. 针对实验过程2，怎样修改的程序？修改前后程序的运行结果是什么？请截图说明。
   
   如上述可知, 修改程序将子进程做信号屏蔽，让子进程只有收到kill发出的信号才有用，修改后，成功实现目标。

4. 针对实验过程3，程序运行的结果是什么样子？时钟中断有什么不同？
   
   如上图所示，时钟中断既可以通过定时触发，也可以kill触发。

5. kill 命令在程序中使用了几次？每次的作用是什么？执行后的现象是什么？
   
   2次，每次的作用是发送信号给子进程，执行后子进程接收到父进程信号结束

6. 使用kill 命令可以在进程的外部杀死进程。进程怎样能主动退出？这两种退出方式哪种更好一些？
   
   进程调用return函数和exit函数都可以主动退出，而kill是强制退出。主动退出比较好，如果在某个子进程退出前父进程被kill强制退出，则子进程会被init进程接管；如果用kill命令杀死某个子进程而其父进程没有调用wait函数等待，则该子进程为处于僵死状态占用资源。

-----------------------------------

### 2.2 管道通信

--------------------------------

（1）先猜想一下这个程序的运行结果。分析管道通信是怎样实现同步与互斥的；

（2）然后按照注释里的要求把代码补充完整，运行程序；

于是将代码补全，写成work_2_1.c, 代码如下:

```c
#include  <unistd.h>                                                                                                       
#include  <signal.h>                                                                                                       
#include  <stdio.h>
#include  <stdlib.h>                                                                                                        
int pid1,pid2;     // 定义两个进程变量 
int main(){
    int fd[2];
    int i = 0; 
    char InPipe[10000];           // 定义读缓冲区
    char c1='1', c2='2'; 
    pipe(fd);                               // 创建管道 
    while((pid1 = fork( )) == -1);      // 如果进程1创建不成功,则空循环 
    if(pid1 == 0) {               // 如果子进程1创建成功,pid1为进程号 
        lockf(fd[1],1,0);            // 锁定管道 
        for(i=0;i<2000;i++){
            write(fd[1],&c1,1);     //  分2000次每次向管道写入字符’1’
        }             
        sleep(5);                           // 等待读进程读出数据 
        lockf(fd[1],0,0);                   // 解除管道的锁定 
        exit(0);                         // 结束进程1 
    } 
    else{ 
        while((pid2 = fork()) == -1);                  // 若进程2创建不成功,则空循环 
        if(pid2 == 0) { 
            lockf(fd[1],1,0); 
            for(i=0;i<2000;i++){
            write(fd[1],&c2,1);         //  分2000次每次向管道写入字符’2’
            }                             
            sleep(5); 
            lockf(fd[1],0,0); 
            exit(0); 
        } 
        else{ 
            wait(0);                                  // 等待子进程1 结束 
            wait(0);                                  // 等待子进程2 结束 
            lockf(fd[0],1,0);                     // 从管道中读出4000个字符
            read(fd[0], InPipe, 4000);
            InPipe[4000] = '\0'; 
            lockf(fd[0],0,0);                                                             //  加字符串结束符 
            printf("%s\n",InPipe);                     // 显示读出的数据
            exit(0);                                        // 父进程结束  
        }
    } 
}
```

输出结果为：

![](C:\Users\Lenovo\Desktop\操作系统\第二次实验\photos\work_2_1_1.png)

![](C:\Users\Lenovo\Desktop\操作系统\第二次实验\photos\work_2_1_2.png)

结果分析：

可以看出很好的实现了读写同步与写进程互斥的问题，但是子进程1和2的写入顺序还是不确定，有时候1在前，有时候2在前。

在work_2_2.c中取消锁，输出结果为：

![](C:\Users\Lenovo\Desktop\操作系统\第二次实验\photos\work_2_2.png)

结果分析：

可以看出12的写入顺序不确定，12会交替输出等等，竟争使用管道2;

在work_2_3.c中取消wait（），输出结果为：

![](C:\Users\Lenovo\Desktop\操作系统\第二次实验\photos\work_2_3.png)

结果分析：

没有做同步之后，读进程在没有写完之后就已经读出，导致2输一部分就已经结束，或者只读取1的内容

猜想：能不能通过信号量来实现进程1和进程2的同步？
通过查询资料有名信号量来实现进程同步通信

[linux有名信号量](https://dontla.blog.csdn.net/article/details/126441626)

代码如下：

```c
#include<unistd.h>                                                                                            
#include  <signal.h>                                                                                                       
#include  <stdio.h>
#include  <stdlib.h> 
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>                                                                                       
int pid1,pid2;     // 定义两个进程变量 
union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *arry;
};
int main(){
    int fd[2];
    int i = 0;
    union semun sem_union;
    sem_union.val = 0;
    int semid=semget(0x5000,1,0640|IPC_CREAT); 
    semctl(semid, 0, SETVAL, sem_union);
    char InPipe[10000];           // 定义读缓冲区
    char c1='1', c2='2'; 
    pipe(fd);                               // 创建管道 
    while((pid1 = fork( )) == -1);      // 如果进程1创建不成功,则空循环 
    if(pid1 == 0) {               // 如果子进程1创建成功,pid1为进程号 
        lockf(fd[1],1,0);            // 锁定管道 
        for(i=0;i<2000;i++){
            write(fd[1],&c1,1);     //  分2000次每次向管道写入字符’1’
        }             
        sleep(5);                           // 等待write读出数据 
        lockf(fd[1],0,0);                   // 解除管道的锁定
        struct sembuf sem_a;
        sem_a.sem_num = 0;
        sem_a.sem_op =  1;
        sem_a.sem_flg = SEM_UNDO;
        semop(semid, &sem_a, 1); 
        exit(0);                         // 结束进程1 
    } 
    else{ 
        while((pid2 = fork()) == -1);                  // 若进程2创建不成功,则空循环 
        if(pid2 == 0) {
            struct sembuf sem_b;
            sem_b.sem_num = 0;
            sem_b.sem_op = -1;
            sem_b.sem_flg = SEM_UNDO; 
            semop(semid, &sem_b, 1);
            lockf(fd[1],1,0); 
            for(i=0;i<2000;i++){
            write(fd[1],&c2,1);         //  分2000次每次向管道写入字符’2’
            }                             
            sleep(5);   //等待write
            lockf(fd[1],0,0);
            exit(0); 
        } 
        else{ 
            wait(0);                                  // 等待子进程1 结束 
            wait(0);                                  // 等待子进程2 结束 
            lockf(fd[0],1,0);                     // 从管道中读出4000个字符
            read(fd[0], InPipe, 4000);
            InPipe[4000] = '\0'; 
            lockf(fd[0],0,0);                                                             //  加字符串结束符 
            printf("%s\n",InPipe);                     // 显示读出的数据
            exit(0);                                        // 父进程结束  
        }
    } 
}
```

输出结果如下：

![](C:\Users\Lenovo\Desktop\操作系统\第二次实验\photos\work_2_4.png)

结果分析：

成功实现进程1和2之前的同步关系！

### 问题回答：

1. 你最初认为运行结果会怎么样？
   
   屏幕会输出2000个1和2000个2.

2. 实际的结果什么样？有什么特点？试对产生该现象的原因进行分析。
   
   多次实验发现有时先输出2000个1，有时输出2000个2, 输出顺序不固定，这是因为子进程1和子进程2执行的先后顺序是不一定的，如果进程1先执行则先输入1；如果进程2先执行则先输入2。但是并没有交错输出1或2，这是因为子进程在写入前线上了锁，保证只有一个进程在向管道输入数据。

3. 实验中管道通信是怎样实现同步与互斥的？如果不控制同步与互斥会发生什么后果。
   
   通过父进程wait子进程来实现读写同步，通过互斥锁来控制互斥，不控制的话，后果如上图所示。

---------------------

## 2.4 页面置换

----------------------------

### FIFO算法

最初为了先实现功能，编写代码FIFO.cpp,实现了交互的功能，输出结果如下：

![](C:\Users\Lenovo\Desktop\操作系统\第二次实验\photos\FIFO.png)

![](C:\Users\Lenovo\Desktop\操作系统\第二次实验\photos\FIFO2.png)

可以简单验证，基本符合功能要求,同时编写test_FIFO, 对Bleady异常进行观察：

![](C:\Users\Lenovo\Desktop\操作系统\第二次实验\photos\Bleady.png)

可以看出分配的页面数增多，反而命中率下降。

### LRU算法

最初为了先实现功能，编写代码LRU.cpp,实现了交互的功能，输出结果如下：

![](C:\Users\Lenovo\Desktop\操作系统\第二次实验\photos\LRU1.png)

![](C:\Users\Lenovo\Desktop\操作系统\第二次实验\photos\LRU2.png)

![](C:\Users\Lenovo\Desktop\操作系统\第二次实验\photos\LRU3.png)

可以简单验证，基本符合功能要求

### 代码重构

新建page_change文件夹，对代码进行重构:

FIFO.cpp:

```c
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
}
```

LRU.cpp:

```cpp
#include "pagechange.h"
int LRU(queue<int> main,int ap,int pp){
    int last = 0,diseffect = 0, position = 0;
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
}
```

struct.cpp:

```cpp
#include "pagechange.h"
page_struct::page_struct(){
    pagecontrol = -1;
    inmemory = 0;
    used_information = 0;
}
pagecontrol_struct::pagecontrol_struct(){
    page = -1;
    used = 0;
} 
```

pagechange.h:

```c
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
int LRU(queue<int> main,int ap,int pp);
#endif
```

编写test.cpp 进行调用，代码如下：

```cpp
#include "pagechange.h"
int main(){
    int ap,pp,total_instruction;
    cout<<"请输入allocation page:";
    cin>>ap;
    cout<<"请输入process page:";
    cin>>pp;
    cout<<"请输入总的序列数:";
    cin>>total_instruction;
    cout<<"随机序列为：";
    queue<int> main;
    for(int i=0;i<total_instruction;i++){
        main.push(rand()%pp);
        cout<<main.back()<<" ";
    }
    cout<<endl;
    int diseffect1 = FIFO(main,ap,pp);
    int diseffect2 = LRU(main,ap,pp);
    float minzhong1,minzhong2;
    minzhong1 = (1.00-(1.00*diseffect1)/total_instruction)*100;               //计算命中率
    minzhong2 = (1.00-(1.00*diseffect2)/total_instruction)*100; 
    cout<<"FIFO命中率:"<<minzhong1<<"%"<<endl;
    cout<<"LRU命中率:"<<minzhong2<<"%"<<endl;
    return 0;
}
```

输出结果为：

![](C:\Users\Lenovo\Desktop\操作系统\第二次实验\photos\pagechange.png)

结果较好，对两个函数封装较完整。

但是有疑问的一点是函数内操作对queque无效？拷贝构造函数的设计。
