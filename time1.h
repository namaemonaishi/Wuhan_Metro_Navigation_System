int rehour1,rehour2;
float remin1,remin2;
float recrowd;
float StartTime;
int reroad;
float addtime(float t1,int dis,int linen)
{
	float allt;
	int i;
	if((linen==2)||(linen==4))
	{
		allt=dis*(1+2);
		return (allt+t1);
	}
	else if((linen==6)||(linen==7))
	{
		allt=t1;
		if(t1<1140)
		for(i=0;i<dis;i++)
		{
			if((allt+2.2)<1140)
			allt=allt+2.2;
			else
			allt=allt+4;
		}
		else if((t1>=1140)&&(t1<1320))
		for(i=0;i<dis;i++)
		{
			if((allt+4)<1320)
			allt=allt+4;
			else
			allt=allt+2.2;
		}
		else
		allt=allt+dis*2.2;
		return allt;
	}
	else if(linen==8)
	{
		allt=t1;
		if(t1<570)
		for(i=0;i<dis;i++)
		{
			if((allt+2.2)<570)
			allt=allt+2.2;
			else
			allt=allt+4;
		}
		else if((t1>=570)&&(t1<900))
		for(i=0;i<dis;i++)
		{
			if((allt+4)<900)
			allt=allt+4;
			else
			allt=allt+2.2;
		}
		else
		allt=allt+dis*2.2;
		return allt;
	}
	else if((linen==1)||(linen==3))
	{
		allt=t1;
		if(t1<450)
		for(i=0;i<dis;i++)
		{
			if((allt+3)<450)
			allt=allt+3;
			else if(((allt+4)>=450)&&((allt+4)<540))
			allt=allt+4;
			else 
			allt=allt+3;
		}
		else if((t1>=540)&&(t1<990))
		for(i=0;i<dis;i++)
		{
			if((allt+3)<990)
			allt=allt+3;
			else
			allt=allt+4;
		}
		else if((t1>=990)&&(t1<1110))
		for(i=0;i<dis;i++)
		{
			if((allt+4)<1110)
			allt=allt+4;
			else
			allt=allt+3;
		}
		else
		allt=allt+3*dis;
		return allt;
	}
}//输入:时间，线路，坐过的站点数量
//功能:返回下车时间 (考虑拥挤度)

float addtime1(float t1,int dis,int linen)
{
	return (t1+dis*3);
}//作用同上(不考虑拥挤度) 

float addtime2(float t1,int dis,int linen)
{
	float allt,reper;
	int i;
	float retime1=rehour1*60.0+remin1;
	float retime2=rehour2*60.0+remin2;
	if(recrowd<=0.2)
	reper=2.2;
	else if((recrowd>0.2)&&(recrowd<0.75))
	reper=3;
	else if(recrowd>=0.75)
	reper=4;
	if(linen==reroad)
	{
		allt=t1;
		if(t1<retime1)
		for(i=0;i<dis;i++)
		{
			if((allt+3)<retime1)
			allt=allt+3;
			else
			allt=allt+reper;
		}
		else if((t1>=retime1)&&(t1<retime2))
		for(i=0;i<dis;i++)
		{
			if((allt+reper)<retime2)
			allt=allt+reper;
			else
			allt=allt+3;
		}
		else
		allt=allt+dis*3;
		return allt;
	}
	else if((linen==2)||(linen==4))
	{
		allt=dis*(1+2);
		return (allt+t1);
	}
	else if((linen==6)||(linen==7))
	{
		allt=t1;
		if(t1<1140)
		for(i=0;i<dis;i++)
		{
			if((allt+2.2)<1140)
			allt=allt+2.2;
			else
			allt=allt+4;
		}
		else if((t1>=1140)&&(t1<1320))
		for(i=0;i<dis;i++)
		{
			if((allt+4)<1320)
			allt=allt+4;
			else
			allt=allt+2.2;
		}
		else
		allt=allt+dis*2.2;
		return allt;
	}
	else if(linen==8)
	{
		allt=t1;
		if(t1<570)
		for(i=0;i<dis;i++)
		{
			if((allt+2.2)<570)
			allt=allt+2.2;
			else
			allt=allt+4;
		}
		else if((t1>=570)&&(t1<900))
		for(i=0;i<dis;i++)
		{
			if((allt+4)<900)
			allt=allt+4;
			else
			allt=allt+2.2;
		}
		else
		allt=allt+dis*2.2;
		return allt;
	}
	else if((linen==1)||(linen==3))
	{
		allt=t1;
		if(t1<450)
		for(i=0;i<dis;i++)
		{
			if((allt+3)<450)
			allt=allt+3;
			else if(((allt+4)>=450)&&((allt+4)<540))
			allt=allt+4;
			else 
			allt=allt+3;
		}
		else if((t1>=540)&&(t1<990))
		for(i=0;i<dis;i++)
		{
			if((allt+3)<990)
			allt=allt+3;
			else
			allt=allt+4;
		}
		else if((t1>=990)&&(t1<1110))
		for(i=0;i<dis;i++)
		{
			if((allt+4)<1110)
			allt=allt+4;
			else
			allt=allt+3;
		}
		else
		allt=allt+3*dis;
		return allt;
	}
}//输入:自定义的拥挤度,时间,站点数,线路号
//功能:计算自定义拥挤度时的总时间 

float timeall(int cline,float t)
{
	float a;
	while(a<t)
	a=a+3;
	return a;
}//输入:当前时间
//功能:计算下一班车到达的时间 

