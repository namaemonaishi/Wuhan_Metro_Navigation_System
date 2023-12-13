extern float StartTime;
extern int reroad;
extern int rehour1,rehour2;
extern float remin1,remin2;
extern float recrowd;
bool lineused[9]={0};
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

