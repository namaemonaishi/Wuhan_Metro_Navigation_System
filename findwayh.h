extern float StartTime;
extern int reroad;
extern int rehour1,rehour2;
extern float remin1,remin2;
extern float recrowd;
bool lineused[9]={0};
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

