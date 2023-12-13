
extern float StartTime;
extern int reroad;
extern int rehour1,rehour2;
extern float remin1,remin2;
extern float recrowd;

bool judgename(char *s1,char *s2){
	char *p = s1;
	char *q = s2;
	while((*p) && (*q)){
		if((*p)!=(*q))	return false;
		p++;q++;
	}
	if((*p) || (*q))	return false;
	return true;
}//要求输入：两个字符类型的指针变量
//功能判断站点名称,输入的用户名等是否相等 

int yongji(float time,int road)
{	
	float retime1=rehour1*60.0+remin1;
	float retime2=rehour2*60.0+remin2;
	if(road==reroad)
	{
		if(recrowd<=0.5)
		return 1;
		else if((time<=retime2)&&(time>=retime1))
		return 0;
		else 
		return 1;
	}
	else if((road==2)||(road==4))
	return 1;
	else if((road==6)||(road==7))
	{
		if((time<=1320)&&(time>=1140))
		return 0;
		else
		return 1;
	}
	else if(road==8)
	{
		if((time<=900)&&(time>=570))
		return 0;
		else
		return 1;
	}
	else if((road==1)||(road==3))
	{
		if((time>=450)&&(time<=540))
		return 0;
		else if((time>990)&&(time<1110))
		return 0;
		else
		return 1;
	}
}//要求输入:当前所在的线路以及时间
//功能:超过50%拥挤度返回0,否则返回1 

float crowd(int linen,float t1,int dis)
{
	float allt;
	int i;
	float jishu;
	if((linen==2)||(linen==4))
	{
		return (dis*0.5);
	}
	else if((linen==6)||(linen==7))
	{
		allt=t1;
		if(t1<1140)
		for(i=0;i<dis;i++)
		{
			if((allt+2.2)<1140)
			{jishu=jishu+0.15;
			allt=allt+2.2;}
			else
			{jishu=jishu+0.65;
			allt=allt+4;}
		}
		else if((t1>=1140)&&(t1<1320))
		for(i=0;i<dis;i++)
		{
			if((allt+4)<1320)
			{jishu=jishu+0.65;
			allt=allt+4;}
			else
			{jishu=jishu+0.15;
			allt=allt+2.2;}
		}
		else
		jishu=0.15*dis;
		return jishu;
	}
	else if(linen==8)
	{
		allt=t1;
		if(t1<570)
		for(i=0;i<dis;i++)
		{
			if((allt+2.2)<570)
			{jishu=jishu+0.2;
			allt=allt+2.2;}
			else
			{jishu=jishu+0.65;
			allt=allt+4;}
		}
		else if((t1>=570)&&(t1<900))
		for(i=0;i<dis;i++)
		{
			if((allt+4)<900)
			{jishu=jishu+0.65;
			allt=allt+4;}
			else
			{jishu=jishu+0.2;
			allt=allt+2.2;} 
		}
		else
		jishu=dis*0.2;
		return jishu;
	}
	else if((linen==1)||(linen==3))
	{
		allt=t1;
		if(t1<450)
		for(i=0;i<dis;i++)
		{
			if((allt+3)<450)
			{jishu=jishu+0.4;
			allt=allt+3;}
			else if(((allt+4)>=450)&&((allt+4)<540))
			{jishu=jishu+0.8;
			allt=allt+4;}
			else 
			{jishu=jishu+0.4;
			allt=allt+3;}
		}
		else if((t1>=540)&&(t1<990))
		for(i=0;i<dis;i++)
		{
			if((allt+3)<990)
			{jishu=jishu+0.4;
			allt=allt+3;}
			else
			{jishu=jishu+0.75;
			allt=allt+4;}
		}
		else if((t1>=990)&&(t1<1110))
		for(i=0;i<dis;i++)
		{
			if((allt+4)<1110)
			{jishu=jishu+0.75;
			allt=allt+4;}
			else
			{jishu=jishu+0.4;
			allt=allt+3;}
		}
		else
		jishu=dis*0.4;
		return jishu;
	}
}//输入:当前时间,乘坐线路及站点数 
//功能:用于计算乘车平均拥挤因子的函数 

int all_price(float length){
	int money = 0;
	if(length<=4){
		return 2;
	}
	else if (length<=8){
		return 3;
	}
	else if (length<=12){
		return 4;
	}
	else if (length<=18){
		return 5;
	}
	else if (length<=24){
		return 6;
	}
	else if (length<=32){
		return 7;
	}
	else if (length<=40){
		return 8;
	}
	else if (length<=50){
		return 9;
	}
	else{
		length-=50;
		money +=9;
		if(length - (int)length == 0 && (int)length % 20 == 0){
			money += length / 20;
		}
		else{
			money += length / 20 + 1;
		}
	}
	return money;
}//要求输入:当前行驶的距离 
//功能:计算票价的函数 
