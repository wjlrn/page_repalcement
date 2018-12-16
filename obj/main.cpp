#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<iterator>
#include<vector>
#include<time.h>
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<vector>
#include<cmath>
#include<cstddef>
#define INF (~(0x1<<31))
#define MAX 100
using namespace std;
/***************************结构体********************************************************/
//页表的表现形式
struct ready_node{
    int id;      //页号
    int status;  //状态位，1：表示已调入内存，0：表示未调入内存
    int access;  //访问字段：记录本页在一段时间内被访问的次数
    int modify;  //修改位：标识该页在调入内存后是否被修改过
};
/*采用两个vector容器，一个存要访问的页面，一个存在内存中的页面，将这两个都定义为全局变量*/
vector<ready_node> acclist;
vector<ready_node> memorylist;
int missNum = 0;
int blk, n;
size_t k = 0; //循环指针
/*判断页面是否在内存中*/
int find(vector<ready_node> mem, int reqseq)
{
    int i;
    for (i = 0 ;i < int(mem.size());i++)
        if (mem[i].id== reqseq)
        return i;
    return i;
}
/****************************初始化函数：初始化待访问页面序列******************************/
void Init(int jpk){
    int i;
    ready_node node;
    srand(time(NULL));
    switch(jpk){
        case 1:
            freopen("input.txt","r",stdin);
            for(i = 0; i < n; i++)
            {
                if(scanf("%d",&node.id)!=EOF)
                {
                    node.access = 0;
                    node.modify = (rand()%2);
                    node.status = 0;
                    acclist.push_back(node);
                }
            }
            break;
        case 2:
            for(i = 0; i < n; i++)
            {
                if(scanf("%d",&node.id)!=EOF)
                {
                    node.access = 0;
                    node.modify = (rand()%2);
                    node.status = 0;
                    acclist.push_back(node);
                }
            }
            break;
        case 3:
            for(i = 0; i < n; i++)
            {
                node.id = (rand()%10);
                node.access = 0;
                node.modify = (rand()%2);
                node.status = 0;
                acclist.push_back(node);
            }
            break;
    }
}
/******************打印函数，显示内存区域页面驻留情况和待访问页面序列********************/
void Print(){
    int i;
    printf("\t内存区域驻留的页面情况：\t\n");
    printf("页面号\t状态位\t被访问次数\t修改位\n");
    for(i = 0; i < (int)(memorylist.size()); i++)
    {
        printf("%d\t%d\t%d\t\t%d\n",memorylist[i].id,memorylist[i].status,memorylist[i].access,memorylist[i].modify);
    }
    printf("------------------------------------\n");

    printf("\t待访问页面序列：\n");
    printf("页面号\t状态位\t被访问次数\t修改位\n");
    for(i = 0; i < (int)(acclist.size()); i++)
    {
        printf("%d\t%d\t%d\t\t%d\n",acclist[i].id,acclist[i].status,acclist[i].access,acclist[i].modify);
    }
    printf("------------------------------------\n");
}
void FIFO(){
    int reqseq;
    ready_node node;
    vector<ready_node>::iterator t; //定义迭代器
    vector<ready_node>::iterator f;
    for(t = acclist.begin(); t != acclist.end(); )
    {
        reqseq = (*t).id;
        printf("请求访问页面号reqseq = %d\n",reqseq);
        if(find(memorylist, reqseq)==int(memorylist.size()) || int(memorylist.size())==0)//没找到所要访问的页面
        {
            missNum++;
            printf("missNum = %d\n",missNum);
            /*此时判断内存中是否存满页面：存满则删除头页面*/
            if(int(memorylist.size()) == blk){
               memorylist.erase(memorylist.begin());
            }
            /*在待访问页面序列中删除该页面，并增加其他页面相关的数据*/
            for(f=acclist.begin(); f!=acclist.end(); ++f){
                if((*f).id==(*t).id){
                    (*f).access += 1;
                }
            }
            node.id = (*t).id;
            node.access = (*t).access;
            node.modify = (*t).modify;
            node.status = (*t).status;
            memorylist.push_back(node);
            t = acclist.erase(t);
            printf("id=%d\n",(*t).id);
        }else{                             //找到要访问的页面
            for(f=acclist.begin(); f!=acclist.end(); ++f){
                if((*f).id==(*t).id){
                    printf("(*f).id = %d\n",(*f).id);
                    (*f).access += 1;
                    printf("(*f).accessbbbb = %d \n",(*f).access);
                }
            }
            /*在内存页面中修改，关于该页面的相关数据*/
            for(f=memorylist.begin(); f!=memorylist.end(); ++f){
                if((*f).id==(*t).id){
                    printf("(*f).id = %d\n",(*f).id);
                    (*f).access += 1;
                    printf("(*f).accessbbbb = %d \n",(*f).access);
                }
            }
            t = acclist.erase(t);
            printf("id=%d\n",(*t).id);
        }
        Print();
    }
    printf("缺页次数为：%d, 页面请求总次数为：%d\n",missNum, n);
    printf("缺页率为：%f\n",(1.0*missNum)/n);
}
void FIFO2(){
    int reqseq;
    ready_node node;
    vector<ready_node>::iterator t; //定义迭代器
    vector<ready_node>::iterator f;
    for(t = acclist.begin(); t != acclist.end();)
    {
        reqseq = (*t).id;
        printf("请求访问页面号reqseq = %d\n",reqseq);
        if(find(memorylist, reqseq)==int(memorylist.size()) || int(memorylist.size())==0)//没找到所要访问的页面
        {
            missNum++;
            printf("missNum = %d\n",missNum);
            /*此时判断内存中是否存满页面：存满则删除头页面*/
            if(int(memorylist.size()) == blk){
                if(int(k) == blk)
                    k = 0;
               f = memorylist.begin();
               memorylist[k].id = (*t).id;
               memorylist[k].access = (*t).access + 1;
               memorylist[k].modify = (*t).modify;
               memorylist[k].status = (*t).status;
               k++;
            }else{
                node.id = (*t).id;
                node.access = (*t).access;
                node.modify = (*t).modify;
                node.status = (*t).status;
                memorylist.push_back(node);
            }
            /*在待访问页面序列中删除该页面，并增加其他页面相关的数据*/
            for(f=acclist.begin(); f!=acclist.end(); ++f){
                if((*f).id==(*t).id){
                    (*f).access += 1;
                }
            }
            t = acclist.erase(t);
            printf("id=%d\n",(*t).id);
        }else{                             //找到要访问的页面
            for(f=acclist.begin(); f!=acclist.end(); ++f){
                if((*f).id==(*t).id){
                    printf("(*f).id = %d\n",(*f).id);
                    (*f).access += 1;
                    printf("(*f).accessbbbb = %d \n",(*f).access);
                }
            }
            /*在内存页面中修改，关于该页面的相关数据*/
            for(f=memorylist.begin(); f!=memorylist.end(); ++f){
                if((*f).id==(*t).id){
                    printf("(*f).id = %d\n",(*f).id);
                    (*f).access += 1;
                    printf("(*f).accessbbbb = %d \n",(*f).access);
                }
            }
            t = acclist.erase(t);
            printf("id=%d\n",(*t).id);
        }
        Print();
    }
    printf("缺页次数为：%d, 页面请求总次数为：%d\n",missNum, n);
    printf("缺页率为：%f\n",(1.0*missNum)/n);
}
void OPT(){
    int reqseq, i, j, m, n;
    int temp = 0;
    int* sign;
    ready_node node;
    vector<ready_node>::iterator t; //定义迭代器
    vector<ready_node>::iterator f;
    sign = (int*)malloc(blk*sizeof(int));
    for(t = acclist.begin(); t != acclist.end();){
        reqseq = (*t).id;
        printf("请求访问页面号reqseq = %d,",reqseq);
        if(find(memorylist, reqseq)==int(memorylist.size()) || int(memorylist.size())==0)//没找到所要访问的页面
        {
            missNum++;
            /*此时判断内存中是否存满页面：存满则删除头页面*/
            if(int(memorylist.size()) == blk){
                /*找到要删除的元素*/
                for(i = 0; i < int(memorylist.size()); i++){
                    sign[i] = 0;
                }
                printf("int(acclist.size())=%d\n",int(acclist.size()));
                //在待访问页面序列中找到最长时间内不再被访问的页面
                for(j = 0; j < int(memorylist.size()); j++){
                    for(m = 0; m < int(acclist.size()); m++){//从当前页开始计算
                        if(memorylist[j].id == acclist[m].id){
                            break;
                        }else{
                            sign[j] += 1;
                        }
                    }
                    printf("memorylist[%d].id=%d,  sign[%d]=%d\n",j,memorylist[j].id,j,sign[j]);
                }
                /*对页面标记进行排序，temp记录最久未被访问的页面*/
                for(n = 0; n < int(memorylist.size()); n++){
                    printf("n=%d\n",n);
                    if(sign[temp]>sign[n]){
                        temp = n;
                    }else if(sign[temp]==sign[n]){
                        printf("sign[%d]=%d\n",temp,sign[temp]);
                        break;
                    }
                }
                /*找到当前位置的迭代器*/
                for(f = memorylist.begin(); f != memorylist.end(); ++f){
                    if((*f).id == memorylist[temp].id)
                        break;
                }
                (*f).id = (*t).id;
                (*f).access = (*t).access;
                (*f).modify = (*t).modify;
                (*f).status = (*t).status;
                t = acclist.erase(t);
            }else{
                /*在待访问页面序列中删除该页面，并增加其他页面相关的数据*/
                for(f=acclist.begin(); f!=acclist.end(); ++f){
                    if((*f).id==(*t).id){
                        (*f).access += 1;
                        printf("(*f).access = %d \n",(*f).access);
                    }
                }
                node.id = (*t).id;
                node.access = (*t).access;
                node.modify = (*t).modify;
                node.status = (*t).status;
                memorylist.push_back(node);
                t = acclist.erase(t);
                printf("id=%d\n",(*t).id);
            }
        }else{                             //找到要访问的页面
            for(f=acclist.begin(); f!=acclist.end(); ++f){
                if((*f).id==(*t).id){
                    printf("(*f).id = %d\n",(*f).id);
                    (*f).access += 1;
                    printf("(*f).accessbbbb = %d \n",(*f).access);
                }
            }
            /*在内存页面中修改，关于该页面的相关数据*/
            for(f=memorylist.begin(); f!=memorylist.end(); ++f){
                if((*f).id==(*t).id){
                    printf("(*f).id = %d\n",(*f).id);
                    (*f).access += 1;
                    printf("(*f).accessbbbb = %d \n",(*f).access);
                }
            }
            t = acclist.erase(t);
            printf("id=%d\n",(*t).id);
        }

        Print();
    }
    printf("缺页次数为：%d, 页面请求总次数为：%d\n",missNum, n);
    printf("缺页率为：%f\n",(1.0*missNum)/n);
}
void LRU(){
    int reqseq;
    int pos;
    ready_node node;
    vector<ready_node>::iterator t; //定义迭代器
    vector<ready_node>::iterator f;
    for(t = acclist.begin(); t != acclist.end();){
        reqseq = (*t).id;
        printf("请求访问页面号reqseq = %d,",reqseq);
        pos = find(memorylist, reqseq);
        if(pos==int(memorylist.size()) || int(memorylist.size())==0)//没找到所要访问的页面
        {
            missNum++;
            printf("missNum = %d \n",missNum);
            /*此时判断内存中是否存满页面：存满则删除最先一个页面*/
            if(int(memorylist.size()) == blk){
               memorylist.erase(memorylist.begin());
            }
            /*在待访问页面序列中删除该页面，并增加其他页面相关的数据*/
            for(f=acclist.begin(); f!=acclist.end(); ++f){
                if((*f).id==(*t).id){
                    printf("(*f).id = %d\n",(*f).id);
                    (*f).access += 1;
                    printf("(*f).access = %d \n",(*f).access);
                }
            }
            node.id = (*t).id;
            node.access = (*t).access;
            node.modify = (*t).modify;
            node.status = (*t).status;
            memorylist.push_back(node);
            t = acclist.erase(t);
            printf("id=%d\n",(*t).id);
        }else{
            for(f=acclist.begin(); f!=acclist.end(); ++f){
                if((*f).id==(*t).id){
                    printf("(*f).id = %d\n",(*f).id);
                    (*f).access += 1;
                    printf("(*f).accessbbbb = %d \n",(*f).access);
                }
            }
            /*在内存页面中修改，关于该页面的相关数据*/
            for(f=memorylist.begin(); f!=memorylist.end(); ++f){
                if((*f).id==(*t).id){
                    printf("(*f).id = %d\n",(*f).id);
                    (*f).access += 1;
                    printf("(*f).accessbbbb = %d \n",(*f).access);
                }
            }
            node.id = (*t).id;
            node.access = (*t).access;
            node.modify = (*t).modify;
            node.status = (*t).status;
            /*在内存中找到访问的页面，并将其移向后方*/
            memorylist.erase(memorylist.begin() + pos);
            memorylist.push_back(node);
            t = acclist.erase(t);
        }
        Print();
    }
    printf("缺页次数为：%d, 页面请求总次数为：%d\n",missNum, n);
    printf("缺页率为：%f\n",(1.0*missNum)/n);
}
void LFU(){
    int reqseq;
    ready_node node;
    vector<ready_node>::iterator t; //定义迭代器
    vector<ready_node>::iterator f;
    for(t = acclist.begin(); t != acclist.end(); )
    {
        reqseq = (*t).id;
        printf("请求访问页面号reqseq = %d,",reqseq);
        if(find(memorylist, reqseq)==int(memorylist.size()) || int(memorylist.size())==0)//没找到所要访问的页面
        {
            missNum++;
            printf("missNum=%d \n",missNum);
            /*此时判断内存中是否存满页面：存满则删除头页面*/
            if(int(memorylist.size()) == blk){
                int min = INF;
                printf("min=%d\n",min);
                for(f = memorylist.begin(); f!=memorylist.end(); ++f){
                    if(min > (*f).access){
                        min = (*f).access;
                        printf("min=%d\n",min);
                    }
                }
                printf("min=%d\n",min);
                for(f = memorylist.begin(); f!=memorylist.end(); ++f){
                    if(min == (*f).access){
                        break;
                    }
                }
                (*f).id = (*t).id;
                (*f).access = (*t).access;
                (*f).modify = (*t).modify;
                (*f).status = (*t).status;
                t = acclist.erase(t);
            }else{
                /*在待访问页面序列中删除该页面，并增加其他页面相关的数据*/
                for(f=acclist.begin(); f!=acclist.end(); ++f){
                    if((*f).id==(*t).id){
                        printf("(*f).id = %d\n",(*f).id);
                        (*f).access += 1;
                        printf("(*f).access = %d \n",(*f).access);
                    }
                }
                node.id = (*t).id;
                node.access = (*t).access;
                node.modify = (*t).modify;
                node.status = (*t).status;
                memorylist.push_back(node);
                t = acclist.erase(t);
                printf("id=%d\n",(*t).id);
            }
        }else{                             //找到要访问的页面
            for(f=acclist.begin(); f!=acclist.end(); ++f){
                if((*f).id==(*t).id){
                    printf("(*f).id = %d\n",(*f).id);
                    (*f).access += 1;
                    printf("(*f).accessbbbb = %d \n",(*f).access);
                }
            }
            /*在内存页面中修改，关于该页面的相关数据*/
            for(f=memorylist.begin(); f!=memorylist.end(); ++f){
                if((*f).id==(*t).id){
                    printf("(*f).id = %d\n",(*f).id);
                    (*f).access += 1;
                    printf("(*f).accessbbbb = %d \n",(*f).access);
                }
            }
            t = acclist.erase(t);
            printf("id=%d\n",(*t).id);
        }
        Print();
    }
    printf("缺页次数为：%d, 页面请求总次数为：%d\n",missNum, n);
    printf("缺页率为：%f\n",(1.0*missNum)/n);
}
void SimpleClock(){
    int reqseq;
    int i;
    int flag = 0;
    ready_node node;
    vector<ready_node>::iterator t; //定义迭代器
    vector<ready_node>::iterator f;
    for(t = acclist.begin(); t != acclist.end(); )
    {
        reqseq = (*t).id;
        /*当内存中页面未存满时的处理情况：直接将页面加入内存中*/
        printf("请求访问页面号reqseq = %d,",reqseq);
        if(find(memorylist,reqseq)==int(memorylist.size()) || int(memorylist.size())==0){//页面没在内存中
            missNum++;
            printf("missNum=%d\n",missNum);
            if(int(memorylist.size()) == blk){//当内存中页面存满时的处理情况：使用循环指针k来进行判断：检查页面的访问位如果访问位为0，就将该页换出，为1，则将其置为0，将其留在内存中
                flag = 0;
                while(!flag){
                    for(int i = 0; i < int(memorylist.size());i++){ //遍历整个内存：分两种情况，一种是访问位为0，一种是访问位为1
                        if(memorylist[i].status == 0) //将该页置换出去
                        {
                            memorylist.erase(memorylist.begin()+i);
                            flag = 1;
                            break;
                        }else{  //此时页面的访问位为1，将其访问位置为0
                            memorylist[i].status = 0;
                        }
                    }
                }
                node.id = (*t).id;
                node.access = (*t).access;
                node.modify = (*t).modify;
                node.status = (*t).status;
                memorylist.push_back(node);
                t = acclist.erase(t);
            }else{
                /*先修改待处理页面信息，将其页面访问次数加1，同时将状态位(即访问位)置为1*/
                for(f=acclist.begin(); f!=acclist.end(); ++f){
                    if((*f).id==(*t).id){
                        (*f).access += 1;
                        (*f).status = 1;
                    }
                }
                node.id = (*t).id;
                node.access = (*t).access;
                node.modify = (*t).modify;
                node.status = (*t).status;
                memorylist.push_back(node);
                t = acclist.erase(t);
            }
        }else{
            //内存中有该页面:直接修改数据，内存区域和待访问页面区
            for(i = 0; i < int(memorylist.size()); i++){ //遍历内存区域
                if(memorylist[i].id == (*t).id){
                    memorylist[i].status = 1;
                    memorylist[i].access += 1;
                }
            }
            for(i = 0; i < int(acclist.size()); i++){
                if(acclist[i].id == (*t).id){
                    acclist[i].status = 1;
                    memorylist[i].access += 1;
                }
            }
            node.id = (*t).id;
            node.access = (*t).access;
            node.modify = (*t).modify;
            node.status = (*t).status;
            memorylist.push_back(node);
            t = acclist.erase(t);
        }
        Print();
    }
    printf("缺页次数为：%d, 页面请求总次数为：%d\n",missNum, n);
    printf("缺页率为：%f\n",(1.0*missNum)/n);
}
/*改进型置换算法：在考虑页面的使用情况外，还需要增加置换代价这一因素*/
void ImprovedClock(){
    int reqseq;
    int flag; //表示是否找到置换页面
    int i;
    ready_node node;
    vector<ready_node>::iterator t; //定义迭代器
    vector<ready_node>::iterator f;
    for(t = acclist.begin(); t != acclist.end();){
        reqseq = (*t).id;
        printf("请求访问页面号reqseq = %d,",reqseq);
        /*当内存中页面未存满时的处理情况：直接将页面加入内存中*/
        printf("missNum = %d \n",missNum);
        if(find(memorylist, reqseq)==int(memorylist.size()) || int(memorylist.size())==0){//当内存中页面存满时的处理情况：使用循环指针k来进行判断,此时有页面在内存中和不在内存中两种情况
            missNum++;
            if(int(memorylist.size()) == blk){
                flag = 0;
                while(!flag){
                    //第一类情况:寻找内存块中访问位和修改位都为0的页面
                    for(i = 0; i < int(memorylist.size());i++){ //遍历整个内存：分两种情况，一种是访问位为0，一种是访问位为1
                        if(memorylist[i].status == 0 && memorylist[i].modify == 0) //将该页置换出去
                        {
                            memorylist.erase(memorylist.begin()+i);
                            t = acclist.erase(t);
                            flag = 1;
                            break;
                        }
                    }
                    if(flag){break;}
                    //第二类情况：此时没有找到第一类情况的页面，因此重新进行循环，寻找内存块中访问位为0，修改位为1的页面
                    for(i = 0; i < int(memorylist.size());i++){ //遍历整个内存：分两种情况，一种是访问位为0，一种是访问位为1
                        if(memorylist[i].status == 0 && memorylist[i].modify == 1) //将该页置换出去
                        {
                            memorylist.erase(memorylist.begin()+i);
                            flag = 1;
                            break;
                        }
                    }
                    if(flag){break;}
                    //前两类情况都没有找到，将
                    for(i = 0; i < int(memorylist.size());i++){
                        memorylist[i].status = 0;
                    }
                }
            }
            /*到此步已经删除一个页面,就能添加新的页面了*/
            node.id = (*t).id;
            node.access = (*t).access + 1;
            node.modify = (*t).modify;
            node.status = 1;
            memorylist.push_back(node);
            t = acclist.erase(t);
        }else{
            //内存中有该页面:直接修改数据，内存区域和待访问页面区
            for(i = 0; i < int(memorylist.size()); i++){ //遍历内存区域
                if(memorylist[i].id == (*t).id){
                    memorylist[i].status = 1;
                    memorylist[i].access += 1;
                }
            }
            for(i = 0; i < int(acclist.size()); i++){
                if(acclist[i].id == (*t).id){
                    acclist[i].status = 1;
                    memorylist[i].access += 1;
                }
            }
            node.id = (*t).id;
            node.access = (*t).access + 1;
            node.modify = (*t).modify;
            node.status = 1;
            memorylist.push_back(node);
            t = acclist.erase(t);
            t = acclist.erase(t);
        }
        Print();
    }
    printf("缺页次数为：%d, 页面请求总次数为：%d\n",missNum, n);
    printf("缺页率为：%f\n",(1.0*missNum)/n);
}
int main()
{
    int kmp, jpk;
    printf("请输入物理内存块数：");
    scanf("%d",&blk);
    printf("请输入页面序列个数：");
    scanf("%d",&n);
    printf("-------------------------------------\n");
    printf("-------1.先进先出页面置换算法--------\n");
    printf("-------2.最佳页面置换算法------------\n");
    printf("-------3.LRU页面置换算法-------------\n");
    printf("-------4.LFU页面置换算法-------------\n");
    printf("-------5.简单Clock页面置换算法-------\n");
    printf("-------6.改进型Clock页面置换算法-----\n");
    printf("-------------------------------------\n");
    printf("请选择相应的页面置换算法：");
    scanf("%d",&kmp);
    printf("-------------------------------------\n");
    printf("-------1.文件输入方式----------------\n");
    printf("-------2.手动输入方式----------------\n");
    printf("-------3.随机数生成方式--------------\n");
    printf("-------------------------------------\n");
    printf("请选择相应的输入方式：");
    scanf("%d",&jpk);
    Init(jpk);
    Print();
    switch(kmp){
        case 1:{FIFO2();break;}
        case 2:{OPT();break;}
        case 3:{LRU();break;}
        case 4:{LFU();break;}
        case 5:{SimpleClock();break;}
        case 6:{ImprovedClock();break;}
    }
    printf("************************************\n");
    //FIFO();
    //LFU();
    //LRU();
    //OPT();
    //SimpleClock();
    //ImprovedClock();
    return 0;
}
