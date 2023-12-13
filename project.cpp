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
	int changes2;//��¼����վ 
	struct station *prior;//ָ��ǰһ���ڵ� 
	struct station *next;
	int road;//��¼������· 
	int number; //��¼��վ�� 
	struct station *restation1;//ָ�򻻳˵Ľڵ� 
	struct station *restation2;
}station,*stationp;//��վ�Ĵ洢�ṹ 

typedef struct line{
	int maxpeople;//��¼���˳����� 
	int stationnumber;//��¼��վ�� 
	float length;//��¼�ܳ��� 
	float single_length;//��¼ƽ������ 
	stationp p;//ָ���һ��վ̨�ڵ� 
}line,*linep;
line metrol[9];//Ϊÿһ����·����һ��ͷ��㣬��ʾվ�㳤�ȣ�վ������ȹؼ���Ϣ 

struct AverageTest{
	bool operator()(const AverageTest &a) const{//��������� 
		return this->aver<a.aver;
	}
	float time;
	int price;
	float length;
	float aver;
	float crowd;
};//����������·��Ȩ�ȽϵĽ�� 

struct com{
	bool operator()(AverageTest a1,AverageTest a2){
		return a1.aver<a2.aver;
	}
};//�Ƚ� 

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

vector<stationp>point;//�洢һ·�Ͼ����Ļ�����·�Լ���� 
multimap<AverageTest,vector<stationp> ,compare1>ways;
multimap<AverageTest,vector<stationp> ,compare1 >ways2;

stationp findre(int a,char *s)
{
	stationp next;
	next=metrol[a].p;
	while(next&&(!judgename(next->name,s)))
	next=next->next;
	return next;
}//����:վ���,��ǰ��·��
//����:�ҵ��ɻ���վ����һ����·�ϵĽڵ� 

void findway(stationp start,stationp end,float length,float time,int x,float percent) 
{
	point.push_back(start);//ÿ����һ������վ�����䱣���������У� 
	lineused[start->road]=1;//ʹ�ø������������ڳɻ�����·����Ȧ����� 
	if(start==end)//����ʼ�ڵ�����ֹ�ڵ���ͬʱ,����·��Ϣ���� 
	{
		AverageTest test;
		test.length=length;
		test.time=time;
		test.price=all_price(length);
		test.aver=(float)test.price;
		test.crowd=percent/x;
		if(test.time<ENDTIME)//�������������ʱ���򲻻���� 
		ways.insert(pair<AverageTest,vector<stationp> >(test,point));//�Ƚ�����ʱ��,�������򱣴����· 
		point.pop_back();//������Ľڵ��˳����� 
		lineused[start->road]=0;//��ʹ�ù�����·����ÿ� 
		return;
	}
	if(start->road==end->road)//��������յ���ͬһ����·��,��ֱ�ӵ��� 
	{
		int disn=abs(start->number-end->number);
		findway(end,end,length+disn*metrol[end->road].single_length,addtime(timeall(start->road,time),disn,end->road)-1,x+disn,percent+crowd(start->road,time,disn));
	}
	else{//����,��ȥѰ�ҿ��ܴ��ڵĻ���վ�� 
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
//Ѱ·�ݹ麯��,Ҫ���������վָ��,Ŀ��վָ��,���߹���·������,�Ѿ�����ʱ��,��ӵ����. 

void findway1(stationp start,stationp end,float length,float time)//�ú���Ϊ������ӵ���̶ȵĺ���,����һ���������ȥ����ӵ�����ӵļ������ 
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
//����ͬ��
//����: ������ӵ���̶ȵĺ���,����һ���������ȥ����ӵ�����ӵļ������,������ͨѰ· 

void findway2(stationp start,stationp end,float length,float time,int x,float percent)//�ú���Ϊӵ��������ʱϵͳ����·������ 
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
//����ͬ��
//����: ����Ϊӵ��������ʱϵͳ����·������

void findway3(stationp start,stationp end,float length,float time,int x,float percent)//�ú���Ϊ50%��ܵ���·������ 
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
//����ͬ��
//����: ����Ϊ50%��ܵ���·������

void print_way(){
	if(ways.size() == 0){
		printf("�ܱ�Ǹ��û�к��ʵ���·��\n");return;//���û�з���Ҫ��������·,��ӡδ�ҵ� 
	}
	int i=1;
	map<AverageTest,vector<stationp> >::iterator waysit = ways.begin();
	for(;i<=ways.size();i++){
		if(i>3)	return;
		printf("��%d�ַ���:\n",i); 
		vector<stationp>::iterator roadit = (*waysit).second.begin();
		int line = (*roadit)->road;
		for(;roadit!=(*waysit).second.end();roadit++){
			if((*roadit)->road!= line){
				line = (*roadit)->road;
				printf("(������%d����)",line);//��ӡ��· 
			}
			printf("%s",(*roadit)->name);
			printf("--->");
		}
		printf("(����վ��)");
		putchar('\n');
		printf("�ܻ��ѽ�ǮΪ:%d",waysit->first.price);
		printf("   ����ʻ����Ϊ:%f",waysit->first.length);
		printf("   �ܻ���ʱ��Ϊ:%.1f",waysit->first.time-StartTime); 
		printf("	ӵ���ٷ���Ϊ:%.2f\n",waysit->first.crowd);
		waysit++;
	}
}
//���ڴ�ӡ��·�ĺ��� 

void print_way2(){
	if(ways2.size() == 0){
		printf("�ܱ�Ǹ��û�к��ʵ���·��\n");return;
	}
	int i=1;
	map<AverageTest,vector<stationp> >::iterator waysit = ways2.begin();
	for(;i<=ways2.size();i++){
		if(i>3)	return;
		printf("��%d�ַ���:\n",i); 
		vector<stationp>::iterator roadit = (*waysit).second.begin();
		int line = (*roadit)->road;
		for(;roadit!=(*waysit).second.end();roadit++){
			if((*roadit)->road != line){
				line = (*roadit)->road;
				printf("(������%d����)",line);
			}
			printf("%s",(*roadit)->name);
			printf("--->");
		}
		printf("(����վ��)");
		putchar('\n');
		printf("�ܻ��ѽ�ǮΪ:%d",waysit->first.price);
		printf("   ����ʻ����Ϊ:%f",waysit->first.length);
		printf("   �ܻ���ʱ��Ϊ:%.1f\n",waysit->first.time-StartTime); 
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
	char start[20];//function gongnneg cline ��ѡ�е���· start ��ʼվ exchange Ҫ���Ĳ��� 
	stationp p,q,temp,tempe,change;
	fp=fopen("D:\\File.txt","r");
	fp1=fopen("D:\\passwd.txt","r");
	if(fp==NULL)
	return 0;
	if(fp1==NULL)
	return 0;
	fscanf(fp1,"%s",username);
	fscanf(fp1,"%s",passwd0);
	printf("��ӭʹ�ñ�ϵͳ�����ȵ�¼��\n�������û�����") ;//��¼ϵͳ 
	scanf("%s",userin);
	printf("���������룺\n") ;
	scanf("%s",passwd);
	base64encode(passwd,5,passwdout);//����ͷ�ļ��еĿ⺯��,��������ַ�������base64���� 
	if(!(judgename(username,userin)&&judgename(passwd0,passwdout)))//�ж��û�������������Ƿ�ƥ�� 
	{
		printf("\n��½ʧ�ܣ������³���");
		return 0;
	}
	printf("\n��ϲ����½�ɹ�\n"); 
	for(i=1;i<=8;i++)
	{
		if(i==5)
		continue;
		fscanf(fp,"%d",&metrol[i].maxpeople);
		fscanf(fp,"%f",&metrol[i].length);
		fscanf(fp,"%d",&metrol[i].stationnumber);
		metrol[i].single_length=metrol[i].length*1.0/metrol[i].stationnumber;//��վ���ļ��ж�ȡ��·��Ϣ,���洢����·�ļ� 
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
		}//Ϊÿ��վ�������Ϣ 
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
	}//���ӻ���վ 
	//����վ�㲢����ͼ��ͬʱ��ת��վ��������� 
	//ÿ��վ��洢������һ������,�Ƿ�ɻ���,���˽ڵ����Ϣ,����һ�����ϵ�վ����һ��ͷ���洢���� 
	
	printf("��ӭ����ģ�����ϵͳ����ѡ������Ҫʹ�õĹ���\n");
	printf("1.�����ͼϵͳ\t2.�г�ʱ�̱�\n3.�����·ϵͳ\t4.��Ȩʱ�䷽ʽ\n5.�Զ���ӵ��ϵ�������ӵ����ʽ\n0.��ӡ��·");//ϵͳui 
	scanf("%d",&function);
	switch(function)
	{
		case 1:
			printf("��ָ����·\n");
			scanf("%d",&cline);
			if(cline==5)
			{
			printf("��������������= =");
			return 0;
			}
			printf("��ָ����ʼվ��\n");
			scanf("%s",start);
			temp=metrol[cline].p;//���뿪ʼ����·��վ�㲢���� 
			while(temp&&(!judgename(temp->name,start)))
			temp=temp->next;
			if(temp)
			printf("վ�����ҵ�������1��˳λ������һվ������2����λ������һվ,����3������·������4�˳�\n");
			else
			{
			printf("վ�㲻����\n");
			return 0;
			}
			scanf("%d",&exchange);
			while(exchange!=4)
			{
				if(exchange==1)
				{temp=temp->next;
				printf("����%s\n",temp->name);
				if(temp->restation1)
				printf("��ʾ����վ�ɻ���\n");
				scanf("%d",&exchange);
				}
				else if(exchange==2)
				{temp=temp->prior;
				printf("����%s\n",temp->name);
				if(temp->restation1)
				printf("��ʾ����վ�ɻ���\n");
				scanf("%d",&exchange);
				}
				else if(exchange==3)
				{if(temp->changes1==0)
				printf("��վ���ɻ���\n");//��ʾ���� 
				else
				{printf("����Ҫ���˼����ߣ�0.%d 1.%d \n",temp->changes1,temp->changes2);
				scanf("%d",&change);
				if(change==0)
				{temp=temp->restation1;
				printf("����%s����%d������\n",temp->name,temp->road);
				}
				else 
				{temp=temp->restation2;
				printf("����%s����%d������\n",temp->name,temp->road);} 
				}
				scanf("%d",&exchange);
			}
			}
			return 0;
			break;
		case 2:
			 printf("������Ҫ��������·\n");
			 scanf("%d",&cline);
			 if(cline==5)
			 {
			 printf("��������������= =");
			 return 0;
		 	 } 
			 printf("��ָ����ʼվ��\n");
			 scanf("%s",start);
			 temp=metrol[cline].p;
			 while(temp&&(!judgename(temp->name,start)))
			 temp=temp->next;
			 if(temp)
			 printf("վ�����ҵ�,");
			 else
			 {
			 printf("վ�㲻����\n");
			 return 0;
			 }
			 printf("������˳�ʱ��ΪXʱX��\n");
			 scanf("%d%f",&hour,&min);
			 min=60.0*hour+min; 
			 printf("��ܰ��ʾ����������µ���3����һ�ˣ�����ʱ��Ϊ��6����11\n");
			 tim=timeall(cline,min);
			 hour=(int)tim/60;
			 min=tim-hour*60;
			 if((hour>=11)||(hour<=5))
			 printf("��Ǹ����ǰʱ�ε����Ѿ�ֹͣ���У�\n"); 
			 else if(min<59)
			 printf("�������վ�������һ���������%dʱ%.1f�ֵ����%dʱ%.1f�ַ�����������׼��\n�����յ�վ�������һ���������%dʱ%.1f�ֵ����%dʱ%.1f�ַ���,������׼��\n",hour,min,hour,min+1,hour,min,hour,min+1);
			 else
			 printf("�������վ�������һ���������%dʱ%.1f�ֵ����%dʱ%.1f�ַ�����������׼��\n�����յ�վ�������һ���������%dʱ%.1f�ֵ����%dʱ%.1f�ַ���,������׼��\n",hour,min,hour+1,min-60,hour,min,hour+1,min-60);
			 break;
		case 4:
			printf("������Ҫ��������·\n");
			scanf("%d",&cline);
			if(cline==5)
			{
			printf("��������������= =");
			return 0;
		 	} 
			printf("��ָ����ʼվ��\n");
			scanf("%s",start);
			temp=metrol[cline].p;
			while(temp&&(!judgename(temp->name,start)))
			temp=temp->next;
			if(temp)
			printf("վ�����ҵ�");
			else
			{
			printf("վ�㲻����\n");
			return 0;
			}
			if(temp->changes1)
			{
			printf(",��ܰ��ʾ����վ�ɻ��ˣ�");
			printf("%d����",temp->changes1);
			if(temp->changes2)
			printf("��%d����",temp->changes2);
			printf("\n");
			} 
			
			printf("\n������Ҫ�������·\n");
			scanf("%d",&cline2);
			if(cline2==5)
			{
			printf("��������������= =");
			return 0;
		 	} 
			printf("��ָ������վ��\n");
			scanf("%s",start);
			tempe=metrol[cline2].p;
			while(tempe&&(!judgename(tempe->name,start)))
			tempe=tempe->next;
			if(tempe)
			printf("վ�����ҵ�");
			else
			{
			printf("վ�㲻����\n");
			return 0;
			}
			if(tempe->changes1)
			{
			printf(",��ܰ��ʾ����վ�ɻ��ˣ�");
			printf("%d����",tempe->changes1);
			if(tempe->changes2)
			printf("��%d����",tempe->changes2);
			printf("\n");
		}
			printf("\n���������ڵ�ʱ��XʱX��\n");
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
			printf("������Ҫ��������·\n");
			scanf("%d",&cline);
			if(cline==5)
			{
			printf("��������������= =");
			return 0;
		 	} 
			printf("��ָ����ʼվ��\n");
			scanf("%s",start);
			temp=metrol[cline].p;
			while(temp&&(!judgename(temp->name,start)))
			temp=temp->next;
			if(temp)
			printf("վ�����ҵ�");
			else
			{
			printf("վ�㲻����\n");
			return 0;
			}
			if(temp->changes1)
			{
			printf(",��ܰ��ʾ����վ�ɻ��ˣ�");
			printf("%d����",temp->changes1);
			if(temp->changes2)
			printf("��%d����",temp->changes2);
			printf("\n");
			} 
			
			printf("\n������Ҫ�������·\n");
			scanf("%d",&cline2);
			if(cline2==5)
			{
			printf("��������������= =");
			return 0;
		 	} 
			printf("��ָ������վ��\n");
			scanf("%s",start);
			tempe=metrol[cline2].p;
			while(tempe&&(!judgename(tempe->name,start)))
			tempe=tempe->next;
			if(tempe)
			printf("վ�����ҵ�");
			else
			{
			printf("վ�㲻����\n");
			return 0;
			}
			if(tempe->changes1)
			{
			printf(",��ܰ��ʾ����վ�ɻ��ˣ�");
			printf("%d����",tempe->changes1);
			if(tempe->changes2)
			printf("��%d����",tempe->changes2);
			printf("\n");
		}
			printf("\n���������ڵ�ʱ��XʱX��\n");
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
			printf("������Ҫ��������·\n");
			scanf("%d",&cline);
			if(cline==5)
			{
			printf("��������������= =");
			return 0;
		 	} 
			printf("��ָ����ʼվ��\n");
			scanf("%s",start);
			temp=metrol[cline].p;
			while(temp&&(!judgename(temp->name,start)))
			temp=temp->next;
			if(temp)
			printf("վ�����ҵ�");
			else
			{
			printf("վ�㲻����\n");
			return 0;
			}
			if(temp->changes1)
			{
			printf(",��ܰ��ʾ����վ�ɻ��ˣ�");
			printf("%d����",temp->changes1);
			if(temp->changes2)
			printf("��%d����",temp->changes2);
			printf("\n");
			} 
			
			printf("\n������Ҫ�������·\n");
			scanf("%d",&cline2);
			if(cline2==5)
			{
			printf("��������������= =");
			return 0;
		 	} 
			printf("��ָ������վ��\n");
			scanf("%s",start);
			tempe=metrol[cline2].p;
			while(tempe&&(!judgename(tempe->name,start)))
			tempe=tempe->next;
			if(tempe)
			printf("վ�����ҵ�");
			else
			{
			printf("վ�㲻����\n");
			return 0;
			}
			if(tempe->changes1)
			{
			printf(",��ܰ��ʾ����վ�ɻ��ˣ�");
			printf("%d����",tempe->changes1);
			if(tempe->changes2)
			printf("��%d����",tempe->changes2);
			printf("\n");
		}
			printf("\n���������ڵ�ʱ��XʱX��\n");
			float starttime2;
			scanf("%d%f",&hour,&min);
			starttime2=hour*60+min;
			StartTime=starttime2;
			printf("������Ҫ�޸ĵ���·��ʱ�μ�ӵ���ȣ���X��XʱX����XʱX��Xӵ���ȵ�˳��\n"); 
			scanf("%d%d%f%d%f%f",&reroad,&rehour1,&remin1,&rehour2,&remin2,&recrowd); 
			printf("\n���������Ƿ�Ҫʹ�ù��50%ӵ����ϵͳ,Ҫʹ��������1,��������0\n") ;
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
			printf("��������Ҫ��ӡ����·:");
			scanf("%d",&cline);
			if(cline==5)
			{
			printf("����������");
			return 0;
		}
			temp=metrol[cline].p;
			while(temp)
			{printf("%s--->",temp->name);
			temp=temp->next;
			}
			printf("(�յ�)");
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



 
