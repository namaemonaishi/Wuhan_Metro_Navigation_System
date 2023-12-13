#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<vector>
#include<map>
#include"time1.h"
#include"others.h"
#include "./qqee_clib/base64.h"
#define ENDTIME 1380
using namespace std;
FILE *fp,*fp1;
bool lineused[9]={0};
extern float StartTime;
extern int reroad;
extern int rehour1,rehour2;
extern float remin1,remin2;
extern float recrowd;
typedef struct station{
	char name[17];
	int changes1;
	int changes2;//记录换乘站 
	struct station *prior;//指向前一个节点 
	struct station *next;
	int road;//记录所在线路 
	int number; //记录车站号 
	struct station *restation1;//指向换乘的节点 
	struct station *restation2;
}station,*stationp;//车站的存储结构 

typedef struct line{
	int maxpeople;//记录最大乘车人数 
	int stationnumber;//记录总站数 
	float length;//记录总长度 
	float single_length;//记录平均长度 
	stationp p;//指向第一个站台节点 
}line,*linep;
line metrol[9];//为每一条线路创建一个头结点，显示站点长度，站点个数等关键信息 

struct AverageTest{
	bool operator()(const AverageTest &a) const{//运算符重载 
		return this->aver<a.aver;
	}
	float time;
	int price;
	float length;
	float aver;
	float crowd;
};//用来保存线路加权比较的结果 

struct com{
	bool operator()(AverageTest a1,AverageTest a2){
		return a1.aver<a2.aver;
	}
};//比较 

struct compare1{
	bool operator()(AverageTest a1,AverageTest a2){
		if((a1.time-a2.time)!=0){
			return a1.time<a2.time;
		}
		else{
			return a1.price<a2.price;
		}
	}
};

vector<stationp>point;//存储一路上经过的换乘线路以及起点 
multimap<AverageTest,vector<stationp> ,compare1>ways;
multimap<AverageTest,vector<stationp> ,compare1 >ways2;

stationp findre(int a,char *s)
{
	stationp next;
	next=metrol[a].p;
	while(next&&(!judgename(next->name,s)))
	next=next->next;
	return next;
}//输入:站点号,当前线路号
//功能:找到可换乘站在另一条线路上的节点 

void findway(stationp start,stationp end,float length,float time,int x,float percent) 
{
	point.push_back(start);//每进入一个换乘站，则将其保存入容器中， 
	lineused[start->road]=1;//使用该数组来避免在成环的线路中绕圈的情况 
	if(start==end)//当开始节点与终止节点相同时,将线路信息保存 
	{
		AverageTest test;
		test.length=length;
		test.time=time;
		test.price=all_price(length);
		test.aver=(float)test.price;
		test.crowd=percent/x;
		if(test.time<ENDTIME)//如果超出了运行时间则不会输出 
		ways.insert(pair<AverageTest,vector<stationp> >(test,point));//比较运行时间,若更短则保存该线路 
		point.pop_back();//将保存的节点退出容器 
		lineused[start->road]=0;//将使用过的线路标记置空 
		return;
	}
	if(start->road==end->road)//若起点与终点在同一条线路上,则直接到达 
	{
		int disn=abs(start->number-end->number);
		findway(end,end,length+disn*metrol[end->road].single_length,addtime(timeall(start->road,time),disn,end->road)-1,x+disn,percent+crowd(start->road,time,disn));
	}
	else{//否则,就去寻找可能存在的换乘站点 
		stationp nownode=metrol[start->road].p;
		for(int i=1;i<=metrol[start->road].stationnumber;i++)
		{
			if(i==start->number)
			{
				nownode=nownode->next;continue;
			}
			if(nownode->changes1&&lineused[nownode->changes1]==0)
			{
				stationp changednode=nownode->restation1;
				int disn=abs(i-start->number);
				findway(changednode,end,length+abs(i-start->number)*metrol[start->road].single_length,timeall(start->road,((addtime(timeall(start->road,time),disn,start->road))+2.5)),x+disn,percent+crowd(changednode->road,time,disn));
			}
			if(nownode->changes2&&lineused[nownode->changes2]==0)
			{
				stationp changednode=nownode->restation2;
				int disn=abs(i-start->number);
				findway(changednode,end,length+abs(i-start->number)*metrol[start->road].single_length,timeall(start->road,((addtime(timeall(start->road,time),disn,start->road))+2.5)),x+disn,percent+crowd(changednode->road,time,disn));
			}
			nownode=nownode->next;
		}
	}
	lineused[start->road]=0;
	point.pop_back();
	return;
}
//寻路递归函数,要求输入起点站指针,目标站指针,已走过的路径长度,已经过的时间,总拥挤度. 

void findway1(stationp start,stationp end,float length,float time)//该函数为不考虑拥挤程度的函数,与上一个函数相比去掉了拥挤因子的计算过程 
{
	point.push_back(start);
	lineused[start->road]=1;
	if(start==end)
	{
		AverageTest test;
		test.length=length;
		test.time=time;
		test.price=all_price(length);
		test.aver=(float)test.price;
		if(test.time<ENDTIME)
		ways2.insert(pair<AverageTest,vector<stationp> >(test,point));//?
		point.pop_back();
		lineused[start->road]=0;
		return;
	}
	if(start->road==end->road)
	{
		int disn=abs(start->number-end->number);
		findway1(end,end,length+disn*metrol[end->road].single_length,addtime1(timeall(start->road,time),disn,end->road)-1);
	}
	else{
		stationp nownode=metrol[start->road].p;
		for(int i=1;i<=metrol[start->road].stationnumber;i++)
		{
			if(i==start->number)
			{
				nownode=nownode->next;continue;
			}
			if(nownode->changes1&&lineused[nownode->changes1]==0)
			{
				stationp changednode=nownode->restation1;
				int disn=abs(i-start->number);
				findway1(changednode,end,length+abs(i-start->number)*metrol[start->road].single_length,timeall(start->road,((addtime1(timeall(start->road,time),disn,start->road))+2.5)));
			}
			if(nownode->changes2&&lineused[nownode->changes2]==0)
			{
				stationp changednode=nownode->restation2;
				int disn=abs(i-start->number);
				findway1(changednode,end,length+abs(i-start->number)*metrol[start->road].single_length,timeall(start->road,((addtime1(timeall(start->road,time),disn,start->road))+2.5)));
			}
			nownode=nownode->next;
		}
	}
	lineused[start->road]=0;
	point.pop_back();
	return;
}
//输入同上
//功能: 不考虑拥挤程度的函数,与上一个函数相比去掉了拥挤因子的计算过程,用于普通寻路 

void findway2(stationp start,stationp end,float length,float time,int x,float percent)//该函数为拥挤度优先时系统的线路排序函数 
{
	point.push_back(start);
	lineused[start->road]=1;
	if(start==end)
	{
		AverageTest test;
		test.length=length;
		test.time=time;
		test.price=all_price(length);
		test.aver=(float)test.price;
		test.crowd=percent/x;
		if(test.time<ENDTIME)
		ways.insert(pair<AverageTest,vector<stationp> >(test,point));//?
		point.pop_back();
		lineused[start->road]=0;
		return;
	}
	if(start->road==end->road)
	{
		int disn=abs(start->number-end->number);
		findway2(end,end,length+disn*metrol[end->road].single_length,addtime2(timeall(start->road,time),disn,end->road)-1,x+disn,percent+crowd(start->road,time,disn));
	}
	else{
		stationp nownode=metrol[start->road].p;
		for(int i=1;i<=metrol[start->road].stationnumber;i++)
		{
			if(i==start->number)
			{
				nownode=nownode->next;continue;
			}
			if(nownode->changes1&&lineused[nownode->changes1]==0)
			{
				stationp changednode=nownode->restation1;
				int disn=abs(i-start->number);
				findway2(changednode,end,length+abs(i-start->number)*metrol[start->road].single_length,timeall(start->road,((addtime2(timeall(start->road,time),disn,start->road))+2.5)),x+disn,percent+crowd(changednode->road,time,disn));
			}
			if(nownode->changes2&&lineused[nownode->changes2]==0)
			{
				stationp changednode=nownode->restation2;
				int disn=abs(i-start->number);
				findway2(changednode,end,length+abs(i-start->number)*metrol[start->road].single_length,timeall(start->road,((addtime2(timeall(start->road,time),disn,start->road))+2.5)),x+disn,percent+crowd(changednode->road,time,disn));
			}
			nownode=nownode->next;
		}
	}
	lineused[start->road]=0;
	point.pop_back();
	return;
}
//输入同上
//功能: 函数为拥挤度优先时系统的线路排序函数

void findway3(stationp start,stationp end,float length,float time,int x,float percent)//该函数为50%规避的线路排序函数 
{
	point.push_back(start);
	lineused[start->road]=1;
	if(start==end)
	{
		AverageTest test;
		test.length=length;
		test.time=time;
		test.price=all_price(length);
		test.aver=(float)test.price;
		test.crowd=percent/x;
		if(test.time<ENDTIME)
		ways.insert(pair<AverageTest,vector<stationp> >(test,point));//?
		point.pop_back();
		lineused[start->road]=0;
		return;
	}
	if((start->road==end->road)&&yongji(time,end->road)) 
	{
		int disn=abs(start->number-end->number);
		findway3(end,end,length+disn*metrol[end->road].single_length,addtime2(timeall(start->road,time),disn,end->road)-1,x+disn,percent+crowd(start->road,time,disn));
	}
	else{
		stationp nownode=metrol[start->road].p;
		for(int i=1;i<=metrol[start->road].stationnumber;i++)
		{
			if(i==start->number)
			{
				nownode=nownode->next;continue;
			}
			if(nownode->changes1&&(lineused[nownode->changes1]==0)&&(yongji(time,nownode->road)))
			{
				stationp changednode=nownode->restation1;
				int disn=abs(i-start->number);
				findway3(changednode,end,length+abs(i-start->number)*metrol[start->road].single_length,timeall(start->road,((addtime2(timeall(start->road,time),disn,start->road))+2.5)),x+disn,percent+crowd(changednode->road,time,disn));
			}
			if(nownode->changes2&&(lineused[nownode->changes2]==0)&&(yongji(time,nownode->road)))
			{
				stationp changednode=nownode->restation2;
				int disn=abs(i-start->number);
				findway3(changednode,end,length+abs(i-start->number)*metrol[start->road].single_length,timeall(start->road,((addtime2(timeall(start->road,time),disn,start->road))+2.5)),x+disn,percent+crowd(changednode->road,time,disn));
			}
			nownode=nownode->next;
		}
	}
	lineused[start->road]=0;
	point.pop_back();
	return;
}
//输入同上
//功能: 函数为50%规避的线路排序函数

void print_way(){
	if(ways.size() == 0){
		printf("很抱歉！没有合适的线路！\n");return;//如果没有符合要求存入的线路,打印未找到 
	}
	int i=1;
	map<AverageTest,vector<stationp> >::iterator waysit = ways.begin();
	for(;i<=ways.size();i++){
		if(i>3)	return;
		printf("第%d种方案:\n",i); 
		vector<stationp>::iterator roadit = (*waysit).second.begin();
		int line = (*roadit)->road;
		for(;roadit!=(*waysit).second.end();roadit++){
			if((*roadit)->road!= line){
				line = (*roadit)->road;
				printf("(换乘至%d号线)",line);//打印线路 
			}
			printf("%s",(*roadit)->name);
			printf("--->");
		}
		printf("(到达站点)");
		putchar('\n');
		printf("总花费金钱为:%d",waysit->first.price);
		printf("   总行驶距离为:%f",waysit->first.length);
		printf("   总花费时间为:%.1f",waysit->first.time-StartTime); 
		printf("	拥挤百分数为:%.2f\n",waysit->first.crowd);
		waysit++;
	}
}
//用于打印线路的函数 

void print_way2(){
	if(ways2.size() == 0){
		printf("很抱歉！没有合适的线路！\n");return;
	}
	int i=1;
	map<AverageTest,vector<stationp> >::iterator waysit = ways2.begin();
	for(;i<=ways2.size();i++){
		if(i>3)	return;
		printf("第%d种方案:\n",i); 
		vector<stationp>::iterator roadit = (*waysit).second.begin();
		int line = (*roadit)->road;
		for(;roadit!=(*waysit).second.end();roadit++){
			if((*roadit)->road != line){
				line = (*roadit)->road;
				printf("(换乘至%d号线)",line);
			}
			printf("%s",(*roadit)->name);
			printf("--->");
		}
		printf("(到达站点)");
		putchar('\n');
		printf("总花费金钱为:%d",waysit->first.price);
		printf("   总行驶距离为:%f",waysit->first.length);
		printf("   总花费时间为:%.1f\n",waysit->first.time-StartTime); 
		waysit++;
	}
}

int main(void)
{ 
	int i,j,function,cline,cline2,hour;
	float min,tim;
	int exchange;
	char username[11];
	char passwd[7];
	char passwd0[8];
	char passwdout[8]={0};
	char userin[11];
	char start[20];//function gongnneg cline 被选中的线路 start 开始站 exchange 要做的操作 
	stationp p,q,temp,tempe,change;
	fp=fopen("D:\\File.txt","r");
	fp1=fopen("D:\\passwd.txt","r");
	if(fp==NULL)
	return 0;
	if(fp1==NULL)
	return 0;
	fscanf(fp1,"%s",username);
	fscanf(fp1,"%s",passwd0);
	printf("欢迎使用本系统，请先登录：\n请输入用户名：") ;//登录系统 
	scanf("%s",userin);
	printf("请输入密码：\n") ;
	scanf("%s",passwd);
	base64encode(passwd,5,passwdout);//调用头文件中的库函数,对输入的字符串进行base64加密 
	if(!(judgename(username,userin)&&judgename(passwd0,passwdout)))//判断用户名与加密密码是否匹配 
	{
		printf("\n登陆失败，请重新尝试");
		return 0;
	}
	printf("\n恭喜您登陆成功\n"); 
	for(i=1;i<=8;i++)
	{
		if(i==5)
		continue;
		fscanf(fp,"%d",&metrol[i].maxpeople);
		fscanf(fp,"%f",&metrol[i].length);
		fscanf(fp,"%d",&metrol[i].stationnumber);
		metrol[i].single_length=metrol[i].length*1.0/metrol[i].stationnumber;//从站点文件中读取线路信息,并存储进线路文件 
		p=(stationp)malloc(sizeof(station));
		if(!p)
		return 0;
		fscanf(fp,"%s",p->name);
		fscanf(fp,"%d",&(p->changes1));
		fscanf(fp,"%d",&(p->changes2));
		p->next = NULL;
		p->road = i;
		p->restation1= NULL;
		p->restation2 = NULL;
		p->number = 1;
		metrol[i].p=p;
		q=p;
		for(j=2;j<=metrol[i].stationnumber;j++)
		{
			p=(stationp)malloc(sizeof(station));
			if(!p)
			return 0;
			fscanf(fp,"%s",p->name);
			fscanf(fp,"%d",&(p->changes1));
			fscanf(fp,"%d",&(p->changes2));
			p->road=i;
			p->number=j;
			p->prior=q;
			p->next=NULL;
			q->next=p;
			q=p;
		}//为每个站点添加信息 
	}

	for(i=1;i<=8;i++)
	{
		if(i==5) continue;
		temp=metrol[i].p;
		while(temp)
		{
			if(temp->changes1) temp->restation1=findre(temp->changes1,temp->name);
			if(temp->changes2) temp->restation2=findre(temp->changes2,temp->name);
			temp=temp->next;
		}
	}//连接换乘站 
	//读入站点并构建图，同时对转乘站点进行连接 
	//每个站点存储了在哪一条线上,是否可换乘,换乘节点等信息,并将一条线上的站点用一个头结点存储起来 
	
	printf("欢迎来到模拟地铁系统，请选择您想要使用的功能\n");
	printf("1.浏览地图系统\t2.列车时刻表\n3.最短线路系统\t4.加权时间方式\n5.自定义拥挤系数及规避拥挤方式\n0.打印线路");//系统ui 
	scanf("%d",&function);
	switch(function)
	{
		case 1:
			printf("请指定线路\n");
			scanf("%d",&cline);
			if(cline==5)
			{
			printf("请重新认真输入= =");
			return 0;
			}
			printf("请指定开始站点\n");
			scanf("%s",start);
			temp=metrol[cline].p;//输入开始的线路和站点并读入 
			while(temp&&(!judgename(temp->name,start)))
			temp=temp->next;
			if(temp)
			printf("站点已找到，输入1到顺位方向下一站，输入2到逆位方向下一站,输入3换乘线路，输入4退出\n");
			else
			{
			printf("站点不存在\n");
			return 0;
			}
			scanf("%d",&exchange);
			while(exchange!=4)
			{
				if(exchange==1)
				{temp=temp->next;
				printf("这是%s\n",temp->name);
				if(temp->restation1)
				printf("提示：本站可换乘\n");
				scanf("%d",&exchange);
				}
				else if(exchange==2)
				{temp=temp->prior;
				printf("这是%s\n",temp->name);
				if(temp->restation1)
				printf("提示：本站可换乘\n");
				scanf("%d",&exchange);
				}
				else if(exchange==3)
				{if(temp->changes1==0)
				printf("本站不可换乘\n");//提示换乘 
				else
				{printf("请问要换乘几号线？0.%d 1.%d \n",temp->changes1,temp->changes2);
				scanf("%d",&change);
				if(change==0)
				{temp=temp->restation1;
				printf("这是%s，在%d号线上\n",temp->name,temp->road);
				}
				else 
				{temp=temp->restation2;
				printf("这是%s，在%d号线上\n",temp->name,temp->road);} 
				}
				scanf("%d",&exchange);
			}
			}
			return 0;
			break;
		case 2:
			 printf("请输入要乘坐的线路\n");
			 scanf("%d",&cline);
			 if(cline==5)
			 {
			 printf("请重新认真输入= =");
			 return 0;
		 	 } 
			 printf("请指定开始站点\n");
			 scanf("%s",start);
			 temp=metrol[cline].p;
			 while(temp&&(!judgename(temp->name,start)))
			 temp=temp->next;
			 if(temp)
			 printf("站点已找到,");
			 else
			 {
			 printf("站点不存在\n");
			 return 0;
			 }
			 printf("请输入乘车时间为X时X分\n");
			 scanf("%d%f",&hour,&min);
			 min=60.0*hour+min; 
			 printf("温馨提示，正常情况下地铁3分钟一趟，运行时间为早6至晚11\n");
			 tim=timeall(cline,min);
			 hour=(int)tim/60;
			 min=tim-hour*60;
			 if((hour>=11)||(hour<=5))
			 printf("抱歉，当前时段地铁已经停止运行！\n"); 
			 else if(min<59)
			 printf("开往起点站方向的下一班地铁将在%d时%.1f分到达，于%d时%.1f分发车，请做好准备\n开往终点站方向的下一班地铁将在%d时%.1f分到达，于%d时%.1f分发车,请做好准备\n",hour,min,hour,min+1,hour,min,hour,min+1);
			 else
			 printf("开往起点站方向的下一班地铁将在%d时%.1f分到达，于%d时%.1f分发车，请做好准备\n开往终点站方向的下一班地铁将在%d时%.1f分到达，于%d时%.1f分发车,请做好准备\n",hour,min,hour+1,min-60,hour,min,hour+1,min-60);
			 break;
		case 4:
			printf("请输入要乘坐的线路\n");
			scanf("%d",&cline);
			if(cline==5)
			{
			printf("请重新认真输入= =");
			return 0;
		 	} 
			printf("请指定开始站点\n");
			scanf("%s",start);
			temp=metrol[cline].p;
			while(temp&&(!judgename(temp->name,start)))
			temp=temp->next;
			if(temp)
			printf("站点已找到");
			else
			{
			printf("站点不存在\n");
			return 0;
			}
			if(temp->changes1)
			{
			printf(",温馨提示，本站可换乘：");
			printf("%d号线",temp->changes1);
			if(temp->changes2)
			printf("、%d号线",temp->changes2);
			printf("\n");
			} 
			
			printf("\n请输入要到达的线路\n");
			scanf("%d",&cline2);
			if(cline2==5)
			{
			printf("请重新认真输入= =");
			return 0;
		 	} 
			printf("请指定结束站点\n");
			scanf("%s",start);
			tempe=metrol[cline2].p;
			while(tempe&&(!judgename(tempe->name,start)))
			tempe=tempe->next;
			if(tempe)
			printf("站点已找到");
			else
			{
			printf("站点不存在\n");
			return 0;
			}
			if(tempe->changes1)
			{
			printf(",温馨提示，本站可换乘：");
			printf("%d号线",tempe->changes1);
			if(tempe->changes2)
			printf("、%d号线",tempe->changes2);
			printf("\n");
		}
			printf("\n请输入现在的时间X时X分\n");
			float starttime;
			scanf("%d%f",&hour,&min);
			starttime=hour*60+min;
			StartTime=starttime;
			findway(temp,tempe,0,starttime,0,0);
			if(temp->changes1)
			{
				if(temp->changes1)
				findway(temp->restation1,tempe,0,starttime,0,0);
				if(temp->changes2)
				findway(temp->restation2,tempe,0,starttime,0,0);
			}
			print_way();
			break;
			case 3:
			printf("请输入要乘坐的线路\n");
			scanf("%d",&cline);
			if(cline==5)
			{
			printf("请重新认真输入= =");
			return 0;
		 	} 
			printf("请指定开始站点\n");
			scanf("%s",start);
			temp=metrol[cline].p;
			while(temp&&(!judgename(temp->name,start)))
			temp=temp->next;
			if(temp)
			printf("站点已找到");
			else
			{
			printf("站点不存在\n");
			return 0;
			}
			if(temp->changes1)
			{
			printf(",温馨提示，本站可换乘：");
			printf("%d号线",temp->changes1);
			if(temp->changes2)
			printf("、%d号线",temp->changes2);
			printf("\n");
			} 
			
			printf("\n请输入要到达的线路\n");
			scanf("%d",&cline2);
			if(cline2==5)
			{
			printf("请重新认真输入= =");
			return 0;
		 	} 
			printf("请指定结束站点\n");
			scanf("%s",start);
			tempe=metrol[cline2].p;
			while(tempe&&(!judgename(tempe->name,start)))
			tempe=tempe->next;
			if(tempe)
			printf("站点已找到");
			else
			{
			printf("站点不存在\n");
			return 0;
			}
			if(tempe->changes1)
			{
			printf(",温馨提示，本站可换乘：");
			printf("%d号线",tempe->changes1);
			if(tempe->changes2)
			printf("、%d号线",tempe->changes2);
			printf("\n");
		}
			printf("\n请输入现在的时间X时X分\n");
			float starttime1;
			scanf("%d%f",&hour,&min);
			starttime1=hour*60+min;
			StartTime=starttime1;
			findway1(temp,tempe,0,starttime1);
			if(temp->changes1)
			{
				if(temp->changes1)
				findway1(temp->restation1,tempe,0,starttime1);
				if(temp->changes2)
				findway1(temp->restation2,tempe,0,starttime1);
			}
			print_way2();
			break;
			case 5:
			printf("请输入要乘坐的线路\n");
			scanf("%d",&cline);
			if(cline==5)
			{
			printf("请重新认真输入= =");
			return 0;
		 	} 
			printf("请指定开始站点\n");
			scanf("%s",start);
			temp=metrol[cline].p;
			while(temp&&(!judgename(temp->name,start)))
			temp=temp->next;
			if(temp)
			printf("站点已找到");
			else
			{
			printf("站点不存在\n");
			return 0;
			}
			if(temp->changes1)
			{
			printf(",温馨提示，本站可换乘：");
			printf("%d号线",temp->changes1);
			if(temp->changes2)
			printf("、%d号线",temp->changes2);
			printf("\n");
			} 
			
			printf("\n请输入要到达的线路\n");
			scanf("%d",&cline2);
			if(cline2==5)
			{
			printf("请重新认真输入= =");
			return 0;
		 	} 
			printf("请指定结束站点\n");
			scanf("%s",start);
			tempe=metrol[cline2].p;
			while(tempe&&(!judgename(tempe->name,start)))
			tempe=tempe->next;
			if(tempe)
			printf("站点已找到");
			else
			{
			printf("站点不存在\n");
			return 0;
			}
			if(tempe->changes1)
			{
			printf(",温馨提示，本站可换乘：");
			printf("%d号线",tempe->changes1);
			if(tempe->changes2)
			printf("、%d号线",tempe->changes2);
			printf("\n");
		}
			printf("\n请输入现在的时间X时X分\n");
			float starttime2;
			scanf("%d%f",&hour,&min);
			starttime2=hour*60+min;
			StartTime=starttime2;
			printf("请输入要修改的线路，时段及拥挤度，以X线X时X分至X时X分X拥挤度的顺序\n"); 
			scanf("%d%d%f%d%f%f",&reroad,&rehour1,&remin1,&rehour2,&remin2,&recrowd); 
			printf("\n请输入您是否要使用规避50%拥挤度系统,要使用则输入1,否则输入0\n") ;
			int choose;
			scanf("%d",&choose);
			if(choose)
			{
			findway3(temp,tempe,0,starttime2,0,0);
			if(temp->changes1)
			{
				if(temp->changes1)
				findway3(temp->restation1,tempe,0,starttime2,0,0);
				if(temp->changes2)
				findway3(temp->restation2,tempe,0,starttime2,0,0);
			}
			}
			else  
			{
			findway2(temp,tempe,0,starttime2,0,0);
			if(temp->changes1)
			{
				if(temp->changes1)
				findway2(temp->restation1,tempe,0,starttime2,0,0);
				if(temp->changes2)
				findway2(temp->restation2,tempe,0,starttime2,0,0);
			}
			}
			print_way();
			break;
			case 0:
			printf("请输入你要打印的线路:");
			scanf("%d",&cline);
			if(cline==5)
			{
			printf("请重新输入");
			return 0;
		}
			temp=metrol[cline].p;
			while(temp)
			{printf("%s--->",temp->name);
			temp=temp->next;
			}
			printf("(终点)");
			break;
			} 
	fclose(fp);
	fclose(fp1);
	for(i=1;i<=8;i++)
	{
		if(i==5) continue;
		p=metrol[i].p;
		while(p)
		{
			q=p;
			p=p->next;
			free(q);
		}
	}
	return 0;
}



 
