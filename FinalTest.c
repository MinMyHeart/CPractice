#include<stdio.h>
#include"stdlib.h"
#include"io.h"


struct Meeting
{
	char dpName[16];//部门名称	
	char name[16];//会议名称
	char num[8]; //编号
	char date[16];//日期
	char locale[20];//地点
	char recorder[20];//记录人
	char host[20];//主持人
	char member[100];//成员
	char summary[144];//摘要
	char content[454];//会议内容
	struct Meeting* next;

};

typedef struct Meeting Nodes;
typedef Nodes* Links;

int main()
{	
	int Save(Links,int);
	int ExitS(Links,int);
	void Display();
	void Modify(Links,int*);
	void Browse(Links);
	void Search(Links);
	Links Load(Links);	
	Links Add(Links);
	Links Remove(Links);
	Links search_minutes(Links,char[5]);
	Links to_new_node(Links);

	Links Head=NULL;
	int choice,isSave = 1,isExit = 0;//选择，判断是否保存，判断是否可以退出

	mkdir("minutes");
	Display();
	Head=to_new_node(Head);	
	Head=Load(Head);
	while(!isExit)
	{
		scanf("%d",&choice);
		while(choice<0 || choice>6)
		{
			Display();
			printf("------------输入有误，请重新输入数字：\n");
			scanf("%d",&choice);
		}	
		fflush(stdin);
		switch(choice)
		{
			case 0: isExit = ExitS(Head,isSave);
					 break;
			case 1: Head = Add(Head);
					 isSave=0;
					 break;
			case 2: Modify(Head,&isSave);
					 break;
			case 3: Head=Remove(Head);
					 break;
			case 4: Browse(Head);
					 break;
			case 5: Search(Head);
					 break;
			case 6: isSave=Save(Head,isSave);
					 break;
			default: break;
		}
	}
	return 0;
}
void Display() 
{		
	system("cls");
	printf("**************************************************************************\n");
	printf("---------------------------会议记录管理系统-------------------------------\n");
	printf("**************************************************************************\n");
	printf("||0：退出程序             ||1：添加会议记录      ||2：编辑会议记录      ||\n");
	printf("||3：删除会议记录         ||4：浏览会议记录      ||5：查找会议记录      ||\n");
	printf("--------------------------||6：保存              ||-----------------------\n");
}
//创建一个新节点，没什么好说的
Links to_new_node(Links newNode)
{
	newNode=(Links)malloc(sizeof(Nodes));
	if(NULL == newNode)
	{
		printf("to_new_node error!\n");
	}
	memset(newNode,0,sizeof(Nodes)-sizeof(Links));
	newNode->next=NULL;
	return newNode;
}
Links Load(Links Head)
{
	struct _finddata_t fileDat;
	char drty[35]="minutes\\",check;
	long Handle;
	FILE *fp=NULL;
	int drtylnth;
	Links Adds=Head,pre=NULL;
	drtylnth=strlen(drty);
	//遍历全部文件
	if((Handle= _findfirst("minutes\\*.dat",&fileDat))!=-1L)
	{
		do
		{
			if(NULL!=(fp=fopen(strcat(drty,fileDat.name),"rb")))
			{
				//判断是否为空文件的条件语句
				if(fread(&check,sizeof(char),1,fp)==0)
				{	
					fclose(fp);
					remove(drty);
					continue;
				}
				fseek(fp,0,SEEK_SET);
				//读取文件时的循环
				while(!feof(fp))
				{
					fread(Adds,sizeof(Nodes)-sizeof(Links),1,fp);
					if((Adds->dpName[0] == '\0'))//解决!feof()多读取一部分内容的问题
					{
						if(Adds != Head)
						{	
							free(Adds);
							Adds=pre;
						}
						break;
					}
					pre=Adds;
					Adds->next=to_new_node(Adds->next);
					Adds=Adds->next;
				}
				fclose(fp);
				fp=NULL;
				//
				pre=Adds;
				Adds->next=to_new_node(Adds->next);
				Adds=Adds->next;
				memset(&drty[drtylnth],0,strlen(drty)-drtylnth);
			}
			else
			{
				printf("Error in Load!\n");
				return NULL;
			}				
		}while(_findnext(Handle,&fileDat)==0);
		_findclose(Handle);
		free(pre->next);
		pre->next=NULL;
		printf("加载完成！\n");
	}
	else
	{
		printf("还未存在任何会议记录！\n");
	}
	if(Head->dpName[0] == '\0')
	{
		free(Head);
		Head=NULL;
	}
	return Head;
}
int ExitS(Links Head,int isSave)
{
	char a;
	Links freeN;
	FILE *fp;
	int choice,drtylnth;	
	if(!isSave)
	{
		printf("---------------------会议记录还未保存，是否直接退出？---------------------\n");
		printf("||0：直接退出             ||1：保存并退出        ||2：取消退出    ||      \n");
		scanf("%d",&choice);
		while(choice<0 || choice>2)
		{
			printf("输入有误，请重新输入。\n");
			scanf("%d",&choice);
		}
		fflush(stdin);
		switch(choice)
		{
			case 0: break;	 
			case 1: Save(Head,isSave);
					break;
			case 2: Display(); 	
					return 0;	
			default: break;		 	
		}
	}
	while(Head !=NULL)
	{
		freeN=Head;
		Head=Head->next;
		free(freeN);
	}
	return 1;
}
Links Add(Links Head)
{
	Links newNode;
	Links to_new_node(Links);
	FILE *fp ;
	char fileLocale[35]="minutes\\";	
	char choice;
	int strlength,loclength;
	Display();
	printf("-----------------------------创建会议记录---------------------------------\n");
	if(Head==NULL)
	{
		Head=to_new_node(Head);
	}
	do
	{
		newNode=to_new_node(newNode);
		printf("部门名称：");
		gets(newNode->dpName);
		fflush(stdin);
		printf("会议名称：");
		scanf("%s",newNode->name);
		fflush(stdin);
		printf("会议编号：");
		gets(newNode->num);
		fflush(stdin);		
		printf("会议日期：");
		gets(newNode->date);
		fflush(stdin);		
		printf("会议地点：");
		gets(newNode->locale);
		fflush(stdin);
		printf("会议成员：");
		gets(newNode->member);
		fflush(stdin);
		printf("记录人：");
		gets(newNode->recorder);
		fflush(stdin);
		printf("主持人：");
		gets(newNode->host);
		fflush(stdin);		
		printf("会议摘要：");
		gets(newNode->summary);
		fflush(stdin);
		printf("会议记录：");
		gets(newNode->content);
		fflush(stdin);
		//接下来这一步为存储数据做准备！
		strlength=strlen(newNode->dpName);
		loclength=strlen(fileLocale);

		fp=fopen(strcat(fileLocale,strcat(newNode->dpName,".dat")),"a+");
		fclose(fp);
		fp=NULL;

		//由于strcat改变了newNode->dpName以及filelocale所以要用memset初始化
		memset(&newNode->dpName[strlength],0,sizeof(newNode->dpName)-strlength);
		memset(&fileLocale[loclength],0,sizeof(fileLocale)-loclength);	


		newNode->next=Head->next;	
		Head->next=newNode;

		Display();
		printf("创建成功！是否继续创建（Y/N）？\n");
		scanf("%c",&choice);
		fflush(stdin);		
		choice=toupper(choice);
		while(choice != 'Y' && choice != 'N')
		{
			printf("输入有误，请重新输入。\n");
			scanf("%c",&choice);
			choice=toupper(choice);
			fflush(stdin);		
		}
	}while((toupper(choice))=='Y');
	Display();
	printf("请继续输入数字进行操作：\n");
	//判断Head指向的内存是否有内容
	if(Head->dpName[0] == '\0')
	{
		newNode->next=NULL;
		return newNode;
	}
	return Head;
}
void Modify(Links Head,int *isSave)
{
	Links pre,target;
	char curLocale[35]="minutes\\";
	char operation[5]="编辑";
	int choice;
	pre=search_minutes(Head,operation);
	if(pre != NULL)
	{
		target=pre->next;
		pre=NULL;
		//option
		printf("**************************************************************************\n");
		printf("||0：部门名称             ||1：会议名称          ||2：会议编号          ||\n");
		printf("||3：日期                 ||4：地点              ||5：记录人            ||\n");
		printf("||6：主持人               ||7：会议成员          ||8：会议摘要          ||\n");
		printf("--------------------------||9：会议记录          ||-----------------------\n");
		printf("请输入要修改的选项：-----------------------\n");
		scanf("%d",&choice);
		fflush(stdin);
		while(choice < 0 || choice > 9){
			printf("输入有误，请重新输入：-----------------------\n");
			scanf("%d",&choice);
			fflush(stdin);
		}
		switch(choice)
		{
		    case 0:	printf("请输入修改后的部门名称：");
					gets(target->dpName);
					break;
			case 1:	printf("请输入修改后的会议名称：");
					gets(target->name);
					break;
			case 2:	printf("请输入修改后的会议编号：");
					gets(target->num);
					break;
			case 3:	printf("请输入修改后的日期  ：");
					gets(target->date);
					break;
			case 4: printf("请输入修改后的地点：");
					gets(target->locale);
					break;
			case 5: printf("请输入修改后的记录人：");
				    gets(target->recorder);
					break;
			case 6: printf("请输入修改后的主持人 ：");
					gets(target->host);
					break;
			case 7: printf("请输入修改后的会议成员：");
					gets(target->member);
					break;
			case 8: printf("请输入修改后的会议摘要：");
					gets(target->summary);
					break;
			case 9: printf("请输入修改后的会议记录：");
					gets(target->content);
					break;
		}
		target=NULL;
		*isSave=0;

		Display();
		printf("修改成功！\n");	
		printf("请输入数字继续进行操作:\n");
	}
	return;
}
Links Remove(Links Head)
{
	Links pre,target=NULL;
	char operation[5]="删除";
	pre=search_minutes(Head,operation);
	if(pre==NULL)
	{
		printf("请输入数字继续进行操作:\n");
		return Head;
	}
	//判断用户删除的是否是Head，若是，则释放previous，并且将Head向target移动
	else if(pre->next == Head)
	{
		target=(pre->next)->next;
		Head=target;
		free(pre->next);
		free(pre);
	}
	else
	{
		target=(pre->next)->next;
		free(pre->next);
		pre->next=target;
	}
	Save(Head,0);
	Display();
	printf("删除成功！\n");
	return Head;
}
void Browse(Links Head)
{
	Links list=NULL;
	char** sort(Links,int,int,int);
	char **stList=NULL;	
	int i,item,way,count=0;//这里使用int类型接收，因为最多只有三个选项同时存在
	Display();
	if(Head==NULL)
	{
		printf("---------------------------  没有会议记录 --------------------------------\n");
		printf("----------------------------已取消浏览操作--------------------------------\n");
		printf("请继续输入数字进行操作：\n");
		return;
	}
	for(list=Head;list!=NULL;list=list->next)
	{
		count++;
	}
	stList=sort(Head,count,1,1);
	//这里本来应该用do-while
	while(1)
	{
		Display();
		printf("-----------------------------浏览会议记录---------------------------------\n");
		printf("||部门名称    ||会议名称    ||会议编号 ||会议日期    ||会议地点    ||记录人  ||\n");
		for(i=0;i<count;i++)
		{	
			for(list=Head;list !=NULL;list=list->next)
			{
				if(!strcmp(list->dpName,stList[i]) || !strcmp(list->num,stList[i]))
				{
					printf("||%-12s||%-12s||%-6s   ||%-12s||%-12s||%-8s||\n",list->dpName,list->name,list->num,list->date,list->locale,list->recorder);		
				}
				else
					continue;
			}
		}
		if(count <=3)//个数过少无需排序
			break;
		//排序选项
		printf("-------------------------  选择排序浏览选项 ------------------------------\n");
		printf("||0：排序部门名称         ||1：排序会议编号      ||2：退出浏览          ||\n");
		scanf("%d",&item);
		if(item == 2)
			break;
		printf("||0：倒序                                        ||1：正序              ||\n");
		scanf("%d",&way);
		stList=sort(Head,count,item,way);
	}	
	printf("-------------------------------已退出浏览-----------------------------------\n");
	printf("请继续输入数字进行操作：\n");	
	return;
}
void Search(Links Head)
{
	Links pShow=NULL,pre;
	char operation[5]="查找";
	pre=search_minutes(Head,operation);
	if(pre==NULL)
	{
		return;
	}
	else
	{
		pShow=pre->next;
	}
	//判断用户查询的是否是Head，若是，则释放previous
	if(pre->next==Head)
	{
		free(pre);
	}
	Display();
	printf("-----------------------------查找会议记录---------------------------------\n");
	printf("------------------------||部门名称：%-12s||------------------------\n",pShow->dpName);
	printf("||会议名称：%-12s||会议编号：%-12s||会议日期：%-12s||\n",pShow->name,pShow->num,pShow->date);
	printf("||会议地点：%-12s||记录人  ：%-8s    ||主持人  ：%-8s    ||\n",pShow->locale,pShow->member,pShow->host);
	printf("||会议成员：%-50s\n||会议摘要：%-50s\n||会议记录：%-454s\n",pShow->recorder,pShow->summary,pShow->content);	
	printf("请继续输入数字进行操作\n");
	pShow=NULL;
	return;
}
int Save(Links Head,int isSave)
{
	int drtylnth,aplnth;
	long Handle;
	struct _finddata_t fileDat;		
	char drty[35]="minutes\\";
	Links pSave=NULL;
	FILE *fp=NULL;
	if(isSave ==1)
	{
		Display();
		return 1;
	}
	//遍历文件以便保存
	if((Handle=_findfirst("minutes\\*.dat",&fileDat))!=-1L)
	{
		drtylnth=strlen(drty);	
		do
		{
			if((fp=fopen(strcat(drty,fileDat.name),"wb"))!=NULL)
			{
				for(pSave = Head;pSave!=NULL;pSave=pSave->next)
				{
					aplnth=strlen(pSave->dpName);
					if(!(strcmp(strcat(pSave->dpName,".dat"),fileDat.name)))
					{
						memset(&(pSave->dpName)[aplnth],0,strlen(pSave->dpName)-aplnth);
						fwrite(pSave,sizeof(Nodes)-sizeof(Links),1,fp);
					}
					else
					{
						memset(&(pSave->dpName)[aplnth],0,strlen(pSave->dpName)-aplnth);
						continue;
					}
				}
			}
			else
			{
				printf("Error in Save!");
			}		
			fclose(fp);
			fp=NULL;
			memset(&drty[drtylnth],0,strlen(drty)-drtylnth);				
		}while(!_findnext(Handle,&fileDat));	
		Display();
		printf("保存成功！\n");	
	}
	else
	{
		Display();
		printf("没有任何会议记录需要保存！\n");
		return 0;
	}
	return 1;
}
Links search_minutes(Links Head,char operation[5])
{
	//查找Head，返回pHead;返回pre;不存在记录，返回NULL;
	Links pre,pHead=NULL;
	Links to_new_node(Links);
	void Display();
	char checkStr[15];
	char choice;
	if(Head == NULL)
	{
		Display();
		printf("---------------------------  没有会议记录 --------------------------------\n");
		printf("----------------------------已取消%s操作--------------------------------\n",operation);
		printf("请继续输入数字进行操作：\n");		
		return NULL;
	}
	pHead=to_new_node(pHead);
	pre=pHead;
	do
	{
		printf("输入会议编号或者会议名称：");		
		scanf("%s",checkStr);
		getchar();	
		for(pHead->next=Head;pHead->next != NULL;pHead->next=(pHead->next)->next)
		{
			if(!(strcmp(checkStr,(pHead->next)->name)) || !(strcmp(checkStr,(pHead->next)->num)))
			{
				if(pre != pHead)
				{
					free(pHead);
					pHead = NULL;
				}
				return pre;
			}
			pre=pHead->next;		
		}
		printf("找不到对应的会议记录！是否重新输入（Y/N）？\n");
		scanf("%c",&choice);
		getchar();
		fflush(stdin);			
		choice=toupper(choice);
		while(choice != 'Y' && choice != 'N')
		{
			printf("输入有误，请重新输入！\n");
			scanf("%c",&choice);
			getchar();
			fflush(stdin);
			choice=toupper(choice);
		}
	}while(choice == 'Y');
	Display();
	return NULL;
}
char** sort(Links stHead,int count,int item,int way)
{
	char **stList=NULL;
	char *tmp=NULL;
	Links stp=stHead;	
	int i,j;
	int ret;
	stList=(char**)malloc(count*(sizeof(char*)));
	for(stp=stHead,i=0;i<count;	stp=stp->next,i++)
	{
		if(item)
			stList[i]=stp->num;
		else
			stList[i]=stp->dpName;
	}
	stHead=NULL;
	//冒泡排序(正)
	if(way)
	{	
		for(i=0;i<count-1;i++)
		{
			for(j=0;j<count-1-i;j++)
			{	
				ret=strcmp(stList[j],stList[j+1]);
				if(!ret || ret<0)
				{
					continue;
				}
				else
				{
					tmp=stList[j];
					stList[j]=stList[j+1];
					stList[j+1]=tmp;
				}
			}
		}
	}	
	//冒泡排序(倒)
	else
	{	for(i=0;i<count-1;i++)
		{
			for(j=0;j<count-1-i;j++)
			{	
				ret=strcmp(stList[j],stList[j+1]);
				if(!ret || ret>0)
				{
					continue;
				}
				else
				{
					tmp=stList[j];
					stList[j]=stList[j+1];
					stList[j+1]=tmp;
				}
			}
		}
	}
	return stList;
}