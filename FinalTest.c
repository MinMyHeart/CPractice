#include<stdio.h>
#include"stdlib.h"
#include"io.h"


struct Meeting
{
	char dpName[16];//��������	
	char name[16];//��������
	char num[8]; //���
	char date[16];//����
	char locale[20];//�ص�
	char recorder[20];//��¼��
	char host[20];//������
	char member[100];//��Ա
	char summary[144];//ժҪ
	char content[454];//��������
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
	int choice,isSave = 1,isExit = 0;//ѡ���ж��Ƿ񱣴棬�ж��Ƿ�����˳�

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
			printf("------------���������������������֣�\n");
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
	printf("---------------------------�����¼����ϵͳ-------------------------------\n");
	printf("**************************************************************************\n");
	printf("||0���˳�����             ||1����ӻ����¼      ||2���༭�����¼      ||\n");
	printf("||3��ɾ�������¼         ||4����������¼      ||5�����һ����¼      ||\n");
	printf("--------------------------||6������              ||-----------------------\n");
}
//����һ���½ڵ㣬ûʲô��˵��
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
	//����ȫ���ļ�
	if((Handle= _findfirst("minutes\\*.dat",&fileDat))!=-1L)
	{
		do
		{
			if(NULL!=(fp=fopen(strcat(drty,fileDat.name),"rb")))
			{
				//�ж��Ƿ�Ϊ���ļ����������
				if(fread(&check,sizeof(char),1,fp)==0)
				{	
					fclose(fp);
					remove(drty);
					continue;
				}
				fseek(fp,0,SEEK_SET);
				//��ȡ�ļ�ʱ��ѭ��
				while(!feof(fp))
				{
					fread(Adds,sizeof(Nodes)-sizeof(Links),1,fp);
					if((Adds->dpName[0] == '\0'))//���!feof()���ȡһ�������ݵ�����
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
		printf("������ɣ�\n");
	}
	else
	{
		printf("��δ�����κλ����¼��\n");
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
		printf("---------------------�����¼��δ���棬�Ƿ�ֱ���˳���---------------------\n");
		printf("||0��ֱ���˳�             ||1�����沢�˳�        ||2��ȡ���˳�    ||      \n");
		scanf("%d",&choice);
		while(choice<0 || choice>2)
		{
			printf("�����������������롣\n");
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
	printf("-----------------------------���������¼---------------------------------\n");
	if(Head==NULL)
	{
		Head=to_new_node(Head);
	}
	do
	{
		newNode=to_new_node(newNode);
		printf("�������ƣ�");
		gets(newNode->dpName);
		fflush(stdin);
		printf("�������ƣ�");
		scanf("%s",newNode->name);
		fflush(stdin);
		printf("�����ţ�");
		gets(newNode->num);
		fflush(stdin);		
		printf("�������ڣ�");
		gets(newNode->date);
		fflush(stdin);		
		printf("����ص㣺");
		gets(newNode->locale);
		fflush(stdin);
		printf("�����Ա��");
		gets(newNode->member);
		fflush(stdin);
		printf("��¼�ˣ�");
		gets(newNode->recorder);
		fflush(stdin);
		printf("�����ˣ�");
		gets(newNode->host);
		fflush(stdin);		
		printf("����ժҪ��");
		gets(newNode->summary);
		fflush(stdin);
		printf("�����¼��");
		gets(newNode->content);
		fflush(stdin);
		//��������һ��Ϊ�洢������׼����
		strlength=strlen(newNode->dpName);
		loclength=strlen(fileLocale);

		fp=fopen(strcat(fileLocale,strcat(newNode->dpName,".dat")),"a+");
		fclose(fp);
		fp=NULL;

		//����strcat�ı���newNode->dpName�Լ�filelocale����Ҫ��memset��ʼ��
		memset(&newNode->dpName[strlength],0,sizeof(newNode->dpName)-strlength);
		memset(&fileLocale[loclength],0,sizeof(fileLocale)-loclength);	


		newNode->next=Head->next;	
		Head->next=newNode;

		Display();
		printf("�����ɹ����Ƿ����������Y/N����\n");
		scanf("%c",&choice);
		fflush(stdin);		
		choice=toupper(choice);
		while(choice != 'Y' && choice != 'N')
		{
			printf("�����������������롣\n");
			scanf("%c",&choice);
			choice=toupper(choice);
			fflush(stdin);		
		}
	}while((toupper(choice))=='Y');
	Display();
	printf("������������ֽ��в�����\n");
	//�ж�Headָ����ڴ��Ƿ�������
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
	char operation[5]="�༭";
	int choice;
	pre=search_minutes(Head,operation);
	if(pre != NULL)
	{
		target=pre->next;
		pre=NULL;
		//option
		printf("**************************************************************************\n");
		printf("||0����������             ||1����������          ||2��������          ||\n");
		printf("||3������                 ||4���ص�              ||5����¼��            ||\n");
		printf("||6��������               ||7�������Ա          ||8������ժҪ          ||\n");
		printf("--------------------------||9�������¼          ||-----------------------\n");
		printf("������Ҫ�޸ĵ�ѡ�-----------------------\n");
		scanf("%d",&choice);
		fflush(stdin);
		while(choice < 0 || choice > 9){
			printf("�����������������룺-----------------------\n");
			scanf("%d",&choice);
			fflush(stdin);
		}
		switch(choice)
		{
		    case 0:	printf("�������޸ĺ�Ĳ������ƣ�");
					gets(target->dpName);
					break;
			case 1:	printf("�������޸ĺ�Ļ������ƣ�");
					gets(target->name);
					break;
			case 2:	printf("�������޸ĺ�Ļ����ţ�");
					gets(target->num);
					break;
			case 3:	printf("�������޸ĺ������  ��");
					gets(target->date);
					break;
			case 4: printf("�������޸ĺ�ĵص㣺");
					gets(target->locale);
					break;
			case 5: printf("�������޸ĺ�ļ�¼�ˣ�");
				    gets(target->recorder);
					break;
			case 6: printf("�������޸ĺ�������� ��");
					gets(target->host);
					break;
			case 7: printf("�������޸ĺ�Ļ����Ա��");
					gets(target->member);
					break;
			case 8: printf("�������޸ĺ�Ļ���ժҪ��");
					gets(target->summary);
					break;
			case 9: printf("�������޸ĺ�Ļ����¼��");
					gets(target->content);
					break;
		}
		target=NULL;
		*isSave=0;

		Display();
		printf("�޸ĳɹ���\n");	
		printf("���������ּ������в���:\n");
	}
	return;
}
Links Remove(Links Head)
{
	Links pre,target=NULL;
	char operation[5]="ɾ��";
	pre=search_minutes(Head,operation);
	if(pre==NULL)
	{
		printf("���������ּ������в���:\n");
		return Head;
	}
	//�ж��û�ɾ�����Ƿ���Head�����ǣ����ͷ�previous�����ҽ�Head��target�ƶ�
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
	printf("ɾ���ɹ���\n");
	return Head;
}
void Browse(Links Head)
{
	Links list=NULL;
	char** sort(Links,int,int,int);
	char **stList=NULL;	
	int i,item,way,count=0;//����ʹ��int���ͽ��գ���Ϊ���ֻ������ѡ��ͬʱ����
	Display();
	if(Head==NULL)
	{
		printf("---------------------------  û�л����¼ --------------------------------\n");
		printf("----------------------------��ȡ���������--------------------------------\n");
		printf("������������ֽ��в�����\n");
		return;
	}
	for(list=Head;list!=NULL;list=list->next)
	{
		count++;
	}
	stList=sort(Head,count,1,1);
	//���ﱾ��Ӧ����do-while
	while(1)
	{
		Display();
		printf("-----------------------------��������¼---------------------------------\n");
		printf("||��������    ||��������    ||������ ||��������    ||����ص�    ||��¼��  ||\n");
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
		if(count <=3)//����������������
			break;
		//����ѡ��
		printf("-------------------------  ѡ���������ѡ�� ------------------------------\n");
		printf("||0������������         ||1�����������      ||2���˳����          ||\n");
		scanf("%d",&item);
		if(item == 2)
			break;
		printf("||0������                                        ||1������              ||\n");
		scanf("%d",&way);
		stList=sort(Head,count,item,way);
	}	
	printf("-------------------------------���˳����-----------------------------------\n");
	printf("������������ֽ��в�����\n");	
	return;
}
void Search(Links Head)
{
	Links pShow=NULL,pre;
	char operation[5]="����";
	pre=search_minutes(Head,operation);
	if(pre==NULL)
	{
		return;
	}
	else
	{
		pShow=pre->next;
	}
	//�ж��û���ѯ���Ƿ���Head�����ǣ����ͷ�previous
	if(pre->next==Head)
	{
		free(pre);
	}
	Display();
	printf("-----------------------------���һ����¼---------------------------------\n");
	printf("------------------------||�������ƣ�%-12s||------------------------\n",pShow->dpName);
	printf("||�������ƣ�%-12s||�����ţ�%-12s||�������ڣ�%-12s||\n",pShow->name,pShow->num,pShow->date);
	printf("||����ص㣺%-12s||��¼��  ��%-8s    ||������  ��%-8s    ||\n",pShow->locale,pShow->member,pShow->host);
	printf("||�����Ա��%-50s\n||����ժҪ��%-50s\n||�����¼��%-454s\n",pShow->recorder,pShow->summary,pShow->content);	
	printf("������������ֽ��в���\n");
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
	//�����ļ��Ա㱣��
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
		printf("����ɹ���\n");	
	}
	else
	{
		Display();
		printf("û���κλ����¼��Ҫ���棡\n");
		return 0;
	}
	return 1;
}
Links search_minutes(Links Head,char operation[5])
{
	//����Head������pHead;����pre;�����ڼ�¼������NULL;
	Links pre,pHead=NULL;
	Links to_new_node(Links);
	void Display();
	char checkStr[15];
	char choice;
	if(Head == NULL)
	{
		Display();
		printf("---------------------------  û�л����¼ --------------------------------\n");
		printf("----------------------------��ȡ��%s����--------------------------------\n",operation);
		printf("������������ֽ��в�����\n");		
		return NULL;
	}
	pHead=to_new_node(pHead);
	pre=pHead;
	do
	{
		printf("��������Ż��߻������ƣ�");		
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
		printf("�Ҳ�����Ӧ�Ļ����¼���Ƿ��������루Y/N����\n");
		scanf("%c",&choice);
		getchar();
		fflush(stdin);			
		choice=toupper(choice);
		while(choice != 'Y' && choice != 'N')
		{
			printf("�����������������룡\n");
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
	//ð������(��)
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
	//ð������(��)
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