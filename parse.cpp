//A.2     C      ��   ��

/*��������л���:
*1Visual C++6.0,VisualC++.NET and Visual C++.NET 2003
*WinNT, Win 200, WinXP and  Win2003 
*2 gcc version 3.3.2  20031022(Red Hat Linux 3.3.2-1)
*Redhat Fedora core 1
*Intel 32 platform
*ʹ�÷���:
*���к�����PL/0 Դ�����ļ���
*�ش��Ƿ�������������
*�ش��Ƿ�������ֱ�
*fa.tmp ������������
*fa1.tmp  ���Դ�ļ�������ж�Ӧ���׵�ַ
*fa2.tmp  ������ 
*fas.tmp  ������ֱ�
*/
#include <stdio.h>
#include "pl0.h"
#include "string.h"
/*����ִ��ʱʹ�õ�ջ*/
#define stacksize 500
#include<stack>
using namespace std;
stack<double>num_stack;	//��������ջ 

int tab_num = 0; //�﷨����������
FILE *tree;//�﷨���ļ�ָ�� 
void printTab(int t); //����������ʾ��ʽ 

int main()
{
	//exprFile = fopen("expression_result.txt", "w");
	errorFile = fopen("error_result.txt", "w"); 
//	if(exprFile == NULL) {
//		printf("�޷������ļ�\n");
//		return 0;
//	}			
	bool nxtlev[symnum];
	printf("Input pl/0 file ?");
	scanf("%s",fname);                                     /*�����ļ���*/
	fin=fopen(fname,"r");
	tree = fopen("tree.txt", "w");
	if(fin)
	{
		init();                                          //��ʼ��*/
		err=0;
		cc=cx=ll=0;
		ch=' ';
		if(-1!=getsym())
		{
			fa=fopen("fa.tmp","w");
			fas=fopen("fas.tmp","w");
			addset(nxtlev,declbegsys,statbegsys,symnum);
			nxtlev[period]=true;
			fprintf(tree,"����\n");
			printTab(tab_num+1);
			fprintf(tree,"�ֳ���\n");
			    
			if(-1==block(0,0,nxtlev))			/*���ñ������*/
			{
				fclose(fa);
				fclose(fa1);
				fclose(fas);
				fclose(fin);
				printf("\n");
				return 0;
			}
			
			printTab(tab_num+1);
			fprintf(tree,".\n");
			
			fclose(fa);
			fclose(fa1);
			fclose(fas);
			if(sym!=period)
			{
				error(9);
			}
			if(err==0)
			{
				fa2=fopen("fa2.tmp", "w");
				fclose(fa2);
			}
			else
			{
				//printf("Errors in pl/0 program");
			}
		}
		fclose(tree);
		fclose(fin);
	}
	else
	{
		printf("Can't open file! \n");
	}
	printf("\n");
	
	if(error_flag) {
		fprintf(errorFile, "%d, ������ȷ��û�дʷ����﷨����\n", line_cnt);
	}
	fclose(errorFile);
	//fclose(exprFile);
	return 0;
}
 
void printTab(int t)
{
	while(t--)
	{
		fprintf(tree,"\t");
	}
}
/*
*��ʼ��
*/
void init()
{
	//���ջ 
	while(!num_stack.empty()) {
		num_stack.pop();
	}
	expr_flag = true; 
	line_cnt = 0;
	error_flag = true;
	//
	int i;
	for(i=0;i<=255;i++)
	{
		ssym[i]=nul;
	}
	ssym['+']=plus;
	ssym['-']=minus;
	ssym['*']=times;
	ssym['/']=slash;
	ssym['(']=lparen;
	ssym[')']=rparen;
	ssym['=']=eql;
	ssym[',']=comma;
	ssym['.']=period;
	ssym['#']=neq;
	ssym[';']=semicolon;
	/*���ñ���������,������ĸ˳��,�����۰����*/
	strcpy(&(word[0][0]),"begin");
	strcpy(&(word[1][0]),"call");
	strcpy(&(word[2][0]),"const");
	strcpy(&(word[3][0]),"do");
	strcpy(&(word[4][0]),"end");
	strcpy(&(word[5][0]),"if");
	strcpy(&(word[6][0]),"odd");
	strcpy(&(word[7][0]),"procedure");
	strcpy(&(word[8][0]),"read");
	strcpy(&(word[9][0]),"then");
	strcpy(&(word[10][0]),"var");
	strcpy(&(word[11][0]),"while");
	strcpy(&(word[12][0]),"write");
	/*���ñ����ַ���*/
	wsym[0]=beginsym;
	wsym[1]=callsym;
	wsym[2]=constsym;
	wsym[3]=dosym;
	wsym[4]=endsym;
	wsym[5]=ifsym;
	wsym[6]=oddsym;
	wsym[7]=procsym;
	wsym[8]=readsym;
	wsym[9]=thensym;
	wsym[10]=varsym;
	wsym[11]=whilesym;
	wsym[12]=writesym;

	/*���÷��ż�*/
	for(i=0;i<symnum;i++)
	{
		declbegsys[i]=false;
		statbegsys[i]=false;
		facbegsys[i]=false;
	}

	/*����������ʼ���ż�*/
	declbegsys[constsym]=true;
	declbegsys[varsym]=true;
	declbegsys[procsym]=true;
	/*������俪ʼ���ż�*/
	statbegsys[beginsym]=true;
	statbegsys[callsym]=true;
	statbegsys[ifsym]=true;
	statbegsys[whilesym]=true;
	/*�������ӿ�ʼ���ż�*/
	facbegsys[ident]=true;
	facbegsys[number]=true;
	facbegsys[lparen]=true;
}
 /*
  *������ʵ�ּ��ϵļ�������
  */
int inset(int e,bool* s)
{
    return s[e];
}
int addset(bool* sr,bool* s1,bool* s2,int n)
{
    int i;
    for(i=0;i<n;i++)
    {
       sr[i]=s1[i]||s2[i];  
    }
    return 0;
}
int subset(bool* sr,bool* s1,bool* s2,int n)
{
    int i;
    for(i=0;i<n;i++)
    {
       sr[i]=s1[i]&&(!s2[i]);
    }
    return 0; 
}
int mulset(bool* sr,bool* s1,bool* s2,int n)
{
    int i;
    for(i=0;i<n;i++)
    {
       sr[i]=s1[i]&&s2[i];  
    } 
    return 0;
}
/*
 *��������ӡ����λ�úʹ������
 */
void error(int n)
{
	error_flag = false;
	//printf("\nerror : %d\n", n); 
	char space[81];
	memset(space,32,81); 
	switch(n) {
		case 1: fprintf(errorFile, "%d, ����˵���е�=д��:=\n", line_cnt);
			break;
		case 2: fprintf(errorFile, "%d, ����˵���е�=��Ӧ������\n", line_cnt);
			break;
		case 3: fprintf(errorFile, "%d, ����˵���еı�ʶ����Ӧ��=\n", line_cnt);
			break;
		case 4: fprintf(errorFile, "%d, const��var��procedure��Ӧ�Ǳ�ʶ��\n", line_cnt);
			break;
		case 5: fprintf(errorFile, "%d, ©����.��;\n", line_cnt);
			break;
		case 6: fprintf(errorFile, "%d, ����˵����ķ��Ų���ȷ��Ӧ����俪ʼ��������̶������\n", line_cnt);
			break;
		case 7: fprintf(errorFile, "%d, Ӧ����俪ʼ��\n", line_cnt);
			break;
		case 8: fprintf(errorFile, "%d, ����������䲿�ֵĺ��������ȷ\n", line_cnt);
			break;								
		case 9: fprintf(errorFile, "%d, �����β���˾��.\n", line_cnt);
			break;
		case 10: fprintf(errorFile, "%d, ���֮��©��;\n", line_cnt);	
			break;
		case 11: fprintf(errorFile, "%d, ��ʶ��δ˵��\n", line_cnt);
			break;
		case 12: fprintf(errorFile, "%d, ��ֵ����У���ֵ���󲿱�ʶ������Ӧ�Ǳ���\n", line_cnt);
			break;
		case 13: fprintf(errorFile, "%d, ��ֵ����󲿱�ʶ����Ӧ�Ǹ�ֵ��:=\n", line_cnt);
			break;
		case 14: fprintf(errorFile, "%d, call��ӦΪ��ʶ��\n", line_cnt);
			break;
		case 15: fprintf(errorFile, "%d, call���ʶ������ӦΪ����\n", line_cnt);
			break;
		case 16: fprintf(errorFile, "%d, ��������ж���then\n", line_cnt);
			break;	
		case 17: fprintf(errorFile, "%d, ����end��;\n", line_cnt);
			break;
		case 18: fprintf(errorFile, "%d, while��ѭ������ж���do\n", line_cnt);
			break;
		case 19: fprintf(errorFile, "%d, ����ķ��Ų���ȷ\n", line_cnt);
			break;
		case 20: fprintf(errorFile, "%d, ӦΪ��ϵ�����\n", line_cnt);
			break;
		case 21: fprintf(errorFile, "%d, ���ʽ�ڱ�ʶ�����Բ����ǹ���\n", line_cnt);
			break;
		case 22: fprintf(errorFile, "%d, ���ʽ��©��������)\n", line_cnt);
			break;
		case 23: fprintf(errorFile, "%d, ���Ӻ�ķǷ�����\n", line_cnt);
			break;
		case 24: fprintf(errorFile, "%d, ���ʽ�Ŀ�ʼ�������Ǵ˷���\n", line_cnt);
			break;								
		case 31: fprintf(errorFile, "%d, ��Խ��\n", line_cnt);
			break;
		case 32: fprintf(errorFile, "%d, read��������еı�ʶ�����Ǳ���\n", line_cnt);
			break;				
		default: fprintf(errorFile, "%d, ����\n", line_cnt);
			break;
	} 	
	space[cc-1]=0;//����ʱ��ǰ�����Ѿ����꣬����cc-1
	err++;
}
/*
 *  ©���ո񣬶�ȡһ���ַ�
 *  ÿ�ζ�һ�У�����line��������line��getsymȡ�պ��ٶ�һ��
 *  ������getsym����
 */
int getch()
{
	if(cc==ll)
    {   	
		if(feof(fin))
		{
			//printf("program incomplete");
			//fprintf(errorFile, "%d, �����β���˾��.\n", line_cnt - 1);
			error(9); 
			return -1;
		}
	    if(error_flag && line_cnt != 0) {
			fprintf(errorFile, "%d, ������ȷ��û�дʷ����﷨����\n", line_cnt);
		}    
		line_cnt++;
		error_flag = true;	 

		ll=0;
		cc=0;
	//	printf("%d ",cx );
		fprintf(fa1,"%d ",cx);
		ch=' ';
		while(ch!=10)
		{
			//fscanf(fin,"%c",&ch)
            if(EOF==fscanf(fin,"%c",&ch))
			{
				line[ll]=0;
				break;
			}
			//����ע�� 
			if(ch == '/') {				
				fscanf(fin, "%c", &ch);
				if(ch == '*') {
					char pre = ' ';
					while(1) {
						fscanf(fin, "%c", &ch);
						if(pre == '*' && ch == '/')
							break;
						if(ch == '\n')
							line_cnt++; 
						pre = ch;
					} 
					fscanf(fin, "%c", &ch);
				} 
				else {
					fprintf(fa1,"%c",ch);
					line[ll] = '/';
					ll++;	
				}
			} 
			//		
			fprintf(fa1,"%c",ch);
			line[ll]=ch;
			ll++;
		}
		fprintf(fa1,"\n");
	}
	ch=line[cc];
	cc++;
	return 0;
}
 /*�ʷ���������ȡһ������
 */
int getsym()	
{		
	int i,j,k;
	while( ch==' '||ch==10||ch==9)	//ɾ��ǰ���ո� 
	{
		getchdo;
		printf("%c",ch);
	}
	if(ch>='a'&&ch<='z')	// ��ǰtoken�ĵ�һ���ַ�Ϊ��ĸ�������Ǳ�ʶ��������
	{ 
		k=0;	// ���ڼ�¼token����
		do{		// ����һ��token
			if(k<al)		//��֤token���Ȳ�������ʶ����󳤶� 
			{ 
				a[k]=ch;
				k++;
			}
			getchdo; 
			printf("%c",ch);
		}while(ch>='a'&&ch<='z'||ch>='0'&&ch<='9');
		a[k]=0;
		strcpy(id,a);	//��ŵ�ǰtoken������
		i=0;
		j=norw-1;
		do{				// �ڱ����ֱ��ж��ֲ��Ҹô��Ƿ����
			k=(i+j)/2;
			if(strcmp(id,word[k])<=0)
			{
				j=k-1;
			}
			if(strcmp(id,word[k])>=0)
			{
				i=k+1;
			}

		}while(i<=j);
		if(i-1>j)		// �ҵ����Ǳ�����
		{
			sym=wsym[k];
		}
		else			// ������Ϊ��ʶ��
		{
			sym=ident;
		}
	}
	else
	{
		if(ch>='0'&&ch<='9')		// ���ַ�Ϊ���֣��������Ϊ��ֵ
		{
			k=0;
			num=0;
			sym=number;
			do{						// ���϶�ȡ����������
				num=10*num+ch-'0';
				k++;
				getchdo;
				printf("%c",ch);
			}while(ch>='0'&&ch<='9'); /*��ȡ���ֵ�ֵ*/
			
			//���������� 
			if(ch == '.') {
				getchdo;
				printf("%c",ch);
				double cnt = 0.1; 
				do {
					num += (ch-'0') * cnt;
					cnt *= 0.1;
					getchdo;
					printf("%c",ch);
				}while(ch>='0'&&ch<='9');
			} 
			num_stack.push(num); 
			// 
			k--;
			if(k>nmax)				// λ������
			{
				error(30);
			}
		}
		else
		{
			if(ch==':')             /*��⸳ֵ����*/  // ���ַ�Ϊ��:����������Ǹ�ֵ
			{
				getchdo;
				printf("%c",ch);
				if(ch=='=')
				{
					sym=becomes;
					getchdo;
					printf("%c",ch);
				}
				else
				{
					sym=nul;            /*����ʶ��ķ���*/
				}
			}
			else
			{
				if(ch=='<')         /*���С�ڻ�С�ڵ��ڷ���*/
				{
					getchdo;
					printf("%c",ch);
					if(ch=='=')
					{
						sym=leq;
						getchdo;
						printf("%c",ch);
					}
					else
					{ 
						sym=lss;
					}
				}
				else
				{
					if(ch=='>')          /*�����ڻ���ڵ��ڷ���*/
					{
						getchdo;
						printf("%c",ch);
						if(ch=='=')
						{
							sym=geq;
							getchdo;
							printf("%c",ch);
						}
						else
						{
						    sym=gtr;
						}
					}
					else		// ������������������ѯ���ű���ֵ
					{
						sym=ssym[ch];/* �����Ų�������������ʱ��ȫ�����յ��ַ��Ŵ���*/
						//getchdo;
						//richard
						if(sym!=period)
						{
							getchdo;
							printf("%c",ch);
						}
						//end richard
					}
				}
			}
		}
	}
	return 0;
}

/*
*���Ե�ǰ�����Ƿ�Ϸ�
*
*��ĳһ���֣���һ����䣬һ�����ʽ����Ҫ����ʱʱ����ϣ����һ����������ĳ����
*���ò��ֵĺ�����ţ� test ���������⣬���Ҹ��𵱼�ⲻͨ��ʱ�Ĳ��ȴ�ʩ
*��������Ҫ���ʱָ����ǰ��Ҫ�ķ��ż��ϺͲ����õļ��ϣ���֮ǰδ��ɲ��ֵĺ��
*���ţ����Լ���ͨ��ʱ�Ĵ����
*
*S1��������Ҫ�ķ���
*s2:�������������Ҫ�ģ�����Ҫһ�������õļ���
*n:�����
*/
int test(bool* s1,bool* s2,int n)
{
    if(! inset(sym,s1))
    {
		error(n);
		/*����ⲻͨ��ʱ����ͣ��ȡ���ţ�ֱ����������Ҫ�ļ��ϻ򲹾ȵļ���*/
		while((! inset(sym,s1))&&(! inset(sym,s2)))
		{
			getsymdo; 
		}
    }
    return 0;
}
/*
 *�����������
 *
 *lev:��ǰ�ֳ������ڲ�
 *tx:���ֱ�ǰβָ��
 *fsys:��ǰģ�������ż���
 */
int block(int lev,int tx,bool* fsys)
{
    int i;
    int dx;                         /*���ַ��䵽����Ե�ַ*/
    int tx0;                        /*������ʼtx*/
    int cx0;                        /*������ʼcx*/
    bool nxtlev[symnum];            /*���¼������Ĳ����У����ż��Ͼ�Ϊֵ�Σ�������ʹ������
                                      ʵ�֣����ݽ�������ָ�룬Ϊ��ֹ�¼������ı��ϼ�������
                                      ���ϣ������µĿռ䴫�ݸ��¼�����*/
    dx=3;
    tx0=tx;                         /*��¼�������ֵĳ�ʼλ��*/
    table[tx].adr=cx;
    tab_num++;
    if(lev > levmax)
    {
		error(32);                                                                                                                                         
    }
    do{
        if(sym==constsym)         /*�յ������������ţ���ʼ����������*/
        {
			
			printTab(tab_num+1);
			fprintf(tree,"const\n");
			getsymdo;
			do{
				printTab(tab_num+1);
				fprintf(tree,"��������\n");
				constdeclarationdo(&tx,lev,&dx);   /*dx��ֵ�ᱻconstdeclaration�ı䣬ʹ��
                                                     ָ��*/
                while(sym==comma)
                {
                   printTab(tab_num+1);
                   fprintf(tree,",\n");
                   printTab(tab_num+1);
                   fprintf(tree,"��������\n");
                   getsymdo;
                   constdeclarationdo(&tx,lev,&dx);
                }
                if(sym==semicolon)
                {
                	printTab(tab_num+1);
                	fprintf(tree,";\n");
                  	getsymdo;
				}
				else
				{
					error(5); /*©���˶��Ż��߷ֺ�*/
				}
			}while(sym==ident);
		}
		if(sym==varsym)/*�յ������������ţ���ʼ�����������*/
		{
			printTab(tab_num+1);
			fprintf(tree,"var\n");
			getsymdo;
			do{
				printTab(tab_num+1);
				fprintf(tree,"��������\n");
				vardeclarationdo(&tx,lev,&dx);
				while(sym==comma)
				{
					printTab(tab_num+1);
					fprintf(tree,"��\n");
					printTab(tab_num+1);
					fprintf(tree,"��������\n");
					getsymdo;
					vardeclarationdo(&tx,lev,&dx);
				}
				if(sym==semicolon)
				{
					printTab(tab_num+1);
					fprintf(tree,";\n");
					getsymdo;
				}
				else
				{
					error(5);
				}
			}while(sym==ident);
		}
		while(sym==procsym)/*�յ������������ţ���ʼ�����������*/
		{
			printTab(tab_num+1);
			fprintf(tree,"procedure\n");
			getsymdo;
			if(sym==ident)
			{
				printTab(tab_num+1);
				fprintf(tree,"��ʶ��\n");
				printTab(tab_num+2);
				fprintf(tree,"%s\n",a);
				enter(procedur,&tx,lev,&dx);/*��¼��������*/
				getsymdo;
			}
			else
			{
				error(4);/*procedure��ӦΪ��ʶ��*/
			}
			if(sym==semicolon)
			{
				printTab(tab_num+1);
				fprintf(tree,";\n");
				getsymdo;
			}
			else
			{
				error(5);/*©���˷ֺ�*/
			}
			memcpy(nxtlev,fsys,sizeof(bool)*symnum);
			nxtlev[semicolon]=true;
			printTab(tab_num+1);
			fprintf(tree,"block\n");
			if(-1==block(lev+1,tx,nxtlev))
			{
				return -1;/*�ݹ����*/
			}
            if(sym==semicolon)
            {
            	printTab(tab_num+1);
            	fprintf(tree,";\n");
                getsymdo;
                memcpy(nxtlev,statbegsys,sizeof(bool)*symnum);
                nxtlev[ident]=true;
                nxtlev[procsym]=true;
                testdo(nxtlev,fsys,6);
             }
             else
             {
                 error(5);                       /*©���˷ֺ�*/
             }
        }
        memcpy(nxtlev,statbegsys,sizeof(bool)*symnum);
        nxtlev[ident]=true;
        nxtlev[period]=true;
        testdo(nxtlev,declbegsys,7);
    }while(inset(sym,declbegsys));                /*ֱ��û����������*/
    code[table[tx0].adr].a=cx;                    /*��ʼ���ɵ�ǰ���̴���*/
    table[tx0].adr=cx;                            /*��ǰ���̴����ַ*/
    table[tx0].size=dx;                           /*����������ÿ����һ�����������dx����1,���������Ѿ�����,dx���ǵ�ǰ�������ݵ�size*/
        
    cx0=cx;
    if(tableswitch)                               /*������ֱ�*/
    {
        printf("TABLE:\n");
        if(tx0+1>tx)
        {
			printf("NULL\n");
        }
        for(i=tx0+1;i<=tx;i++)
        {
            switch(table[i].kind)
            {
                case constant:
					printf("%d const %s",i,table[i].name);
					printf("val=%d\n",table[i].val);
					fprintf(fas,"%d const %s",i,table[i].name);
					fprintf(fas,"val=%d\n",table[i].val);
                    break;
                case variable:
                    printf("%d var%s",i,table[i].name);
                    printf("lev=%d addr=%d\n",table[i].level,table[i].adr);
                    fprintf(fas,"%d var %s",i,table[i].name);
                    fprintf(fas,"lev=%d addr=%d\n",table[i].level,table[i].adr);
                    break;
                case procedur:
                    printf("%d proc%s",i,table[i].name);
                    printf("lev=%d addr=%d size=%d\n",table[i].level,table[i].adr,table[i].size);
					fprintf(fas,"%d proc%s",i,table[i].name);
					fprintf(fas,"lev=%d adr=%d size=%d \n",table[i].level,table[i].adr,table[i].size);
					break;
			}
		}
		printf("\n");
	}
	/*���������Ϊ�ֺŻ�end*/
	memcpy(nxtlev,fsys,sizeof(bool)*symnum);/*ÿ��������ż��Ͷ������ϲ������ż��ͣ��Ա㲹��*/
	nxtlev[semicolon]=true;
	nxtlev[endsym]=true;
	printTab(tab_num+1);
	fprintf(tree,"���\n");
	statementdo(nxtlev,&tx,lev);
	memset(nxtlev,0,sizeof(bool)*symnum); /*�ֳ���û�в��ȼ���*/
	test(fsys,nxtlev,8);                  /*�����������ȷ��*/
	tab_num--;
	return 0;
}
/*
*�����ֱ��м���һ��
*
*k:��������const,var or procedure
*ptx:���ֱ�βָ���ָ�룬Ϊ�˿��Ըı����ֱ�βָ�����ֵ
*lev:�������ڵĲ�Σ��Ժ����е�lev��������
*pdx:Ϊ��ǰӦ����ı�������Ե�ַ�������Ҫ����1
*/
void enter (enum object k,int *ptx,int lev, int *pdx)
{
	(*ptx)++;
	strcpy(table[(*ptx)].name,id);       /*ȫ�ֱ���id���Ѵ��е�ǰ���ֵ�����*/
	table[(*ptx)].kind=k;
	switch(k)
	{
		case constant:                      /*��������*/
			if (num>amax)
			{
				error(31);
				num=0;
			}
			table[(*ptx)].val=num;
			break;
		case variable:                     /*��������*/
			table[(*ptx)].level=lev;
			table[(*ptx)].adr=(*pdx);
			(*pdx)++;
			break;                          /*��������*/
		case procedur:
			table[(*ptx)].level=lev;
			break;
	}

}
/*
 *�������ֵ�λ��
 *�ҵ��򷵻������ֱ��е�λ�ã����򷵻�0
 *
 *idt: Ҫ���ҵ�����
 *tx:����ǰ���ֱ�βָ��
 */
int position(char *  idt,int  tx)
{
	int i;
	strcpy(table[0].name,idt);
	i=tx;
	while(strcmp(table[i].name,idt)!=0)
	{
		i--;
	}
	return i;
}
/*
 *������������
 */
int constdeclaration(int *  ptx,int lev,int *  pdx)
{
	tab_num++;
	if(sym==ident)
	{
		printTab(tab_num+1);
		fprintf(tree,"��ʶ��\n");
		printTab(tab_num+2);
		fprintf(tree,"%s\n",a);
		getsymdo;
		if(sym==eql ||sym==becomes)
		{
			printTab(tab_num+1);
			fprintf(tree,"=\n");
			if(sym==becomes)
			{
				error(1);                     /*��=д�����ˣ�=*/
			}
			getsymdo;
			if(sym==number)
			{
				printTab(tab_num+1);
				fprintf(tree,"integer\n");
				printTab(tab_num+2);
				fprintf(tree,"%.3f\n",num);
				enter(constant,ptx,lev,pdx);
				getsymdo;
			}
			else
			{
				error(2);                  /*����˵��=��Ӧ������*/
			}         
		}
		else
		{
			error(3);                       /*����˵����ʶ��Ӧ��=*/
		}
	}
	else
	{
		error(4);                        /*const��Ӧ�Ǳ�ʶ*/
	}
	tab_num--;
	return 0;
}
/*
 *
 */
int vardeclaration(int * ptx,int lev,int * pdx)
{
	tab_num++;
	if(sym==ident)
 	{
 		printTab(tab_num+1);
 		fprintf(tree,"��ʶ��\n");
 		printTab(tab_num+2);
 		fprintf(tree,"%s\n",a);
 		enter(variable,ptx,lev,pdx);//��д���ֱ�
 		getsymdo;
 	}
 	else
 	{
 		error(4);
    }
    tab_num--;
    return 0;
}
/*
*��䴦��
*/
int statement(bool* fsys,int * ptx,int lev)
{
	int i,cx1,cx2;
   	bool nxtlev[symnum];
   	tab_num++;
   	if(sym==ident)
   	{
   		printTab(tab_num+1);
   		fprintf(tree,"��ʶ��\n");
   		printTab(tab_num+2);
   		fprintf(tree,"%s\n",a);
		i=position(id,*ptx);
   	 	if(i==0)
   	 	{
   	 		error(11);
   	 	}
   	 	else
   	 	{
   	 		if(table[i].kind!=variable)
   	 		{
   	 		 	error(12);
   	 		 	i=0;
   	 		}
   	 		else
   	 		{
   	 			getsymdo;
   	 			if(sym==becomes)
   	 			{
   	 				printTab(tab_num+1);
   	 				fprintf(tree,"=\n");
   	 				getsymdo;
   	 			}
   	 			else
   	 			{
   	 			 	error(13);
   	 			}
   	 			memcpy(nxtlev,fsys,sizeof(bool)* symnum);
   	 			printTab(tab_num+1);
   	 			fprintf(tree,"���ʽ\n");
   	 			expressiondo(nxtlev,ptx,lev);
   	 			//
   	 			if(expr_flag) {
   	 				//fprintf(exprFile, "%lf\n", num_stack.top());
				    num_stack.pop();   	 				
				} 
   	 			expr_flag = true;
   	 			//
   	 	    }
   		}
    }
    else
    {
    	if(sym==readsym)
    	{
    		printTab(tab_num+1);
    		fprintf(tree,"read\n");
    	 	getsymdo;
    	 	printTab(tab_num+1);
    	 	fprintf(tree,"(\n");
    	 	if(sym!=lparen)
    	 	{
    	 	   error(34);
			}
    	    else
			{
    	   		do{
    	   			if(sym==comma)
    	   			{
    	   			    printTab(tab_num+1);
						fprintf(tree,",\n");	
				    }
    	   			getsymdo;
    	   	   		if(sym==ident)
    	   	   		{
    	   	   			printTab(tab_num+1);
    	   	   			fprintf(tree,"��ʶ��\n");
    	   	   			printTab(tab_num+2);
    	   	   			fprintf(tree,"%s\n",a);
    	   	   	  		i=position(id, *ptx);
    	   	   		}
    	   	   		else
    	   	   		{
    	   	   	  		i=0;
    	   	   		}
    	   	   		if(i==0)
    	   	   		{
    	   	   	 		error(35);
    	   	   		}
					getsymdo;
				}while (sym==comma);	/*һ��read���ɶ�������� */
			}
			if(sym!=rparen)
			{
				error(33);			/* ��ʽ����Ӧ��������*/
				while(!inset(sym,fsys))/* �����ȣ�ֱ���յ��ϲ㺯���ĺ������*/
				{
					getsymdo;
				}
			}
			else
			{
				printTab(tab_num+1);
				fprintf(tree,")\n");
				getsymdo;
			}
		}
		else
		{
			if(sym==writesym)			/* ׼������write��䴦����read����*/
			{
				printTab(tab_num+1);
				fprintf(tree,"write\n");
				getsymdo;
				if(sym==lparen)
				{
					printTab(tab_num+1);
					fprintf(tree,"(\n");
					do{
						if(sym==comma)
						{
							printTab(tab_num+1);
							fprintf(tree,",\n");
						}
						getsymdo;
						memcpy(nxtlev,fsys,sizeof(bool)*symnum);
						nxtlev[rparen]=true;
						nxtlev[comma]=true;		/* write�ĺ������Ϊ��or��*/
						printTab(tab_num+1);
						fprintf(tree,"���ʽ\n");
						expressiondo(nxtlev,ptx,lev);/* ���ñ��ʽ�����˴���read��ͬ��readΪ��������ֵ*/
		   	 			//
		   	 			if(expr_flag) {
		   	 				//fprintf(exprFile, "%lf\n", num_stack.top());
						    num_stack.pop();   	 				
						} 
		   	 			expr_flag = true;
   	 					//
					}while(sym==comma);
					if(sym!=rparen)
					{
						error(33);/* write()ӦΪ�������ʽ*/
					}
					else
					{
						printTab(tab_num+1);
						fprintf(tree,")\n");
						getsymdo;
					}
				}
			}
			else
			{
				if(sym==callsym)		/* ׼������call��䴦��*/
				{
					printTab(tab_num+1);
					fprintf(tree,"call\n");
					getsymdo;
					if(sym!=ident)
					{
						error(14);           /*call��ӦΪ��ʶ��*/
					}
					else
					{
						printTab(tab_num+1);
						fprintf(tree,"��ʶ��\n");
						printTab(tab_num+2);
						fprintf(tree,"%s\n",a);
						i=position(id,*ptx);
						if(i==0)
						{
							error(11);          /*����δ�ҵ�*/
						}
						else
						{
							if(table[i].kind!=procedur)
							{
								error(15);      /*call���ʶ��ӦΪ����*/
							}
						}
						getsymdo;
					}
				}
				else
				{
					if(sym==ifsym)     /*׼������if��䴦��*/
					{
						printTab(tab_num+1);
						fprintf(tree,"if\n");
						getsymdo;
						memcpy(nxtlev,fsys,sizeof(bool)*symnum);
						nxtlev[thensym]=true;
						nxtlev[dosym]=true;    /*�������Ϊthen��do*/
						printTab(tab_num+1);
						fprintf(tree,"����\n");
						conditiondo(nxtlev,ptx,lev);   /*�������������߼����㣩����*/
						if(sym==thensym)
						{
							printTab(tab_num+1);
							fprintf(tree,"then\n");
							getsymdo;
						}
						else
						{
							error(16);          /*ȱ��then*/
						}
						cx1=cx;                /*���浱ǰָ���ַ*/	
						printTab(tab_num+1);
						fprintf(tree,"���\n");
						statementdo(fsys,ptx,lev);   /*����then������*/
						code[cx1].a=cx;      /*��statement�����cxΪthen�����ִ�����λ�ã�������ǰ��δ������ת��ַ*/
					}
					else
					{
						if(sym==beginsym)   /*׼�����ո�����䴦��*/
						{
							printTab(tab_num+1);
							fprintf(tree,"begin\n");
							getsymdo;
							memcpy(nxtlev,fsys,sizeof(bool)*symnum);
							nxtlev[semicolon]=true;
							nxtlev[endsym]=true;/*�������Ϊ�ֺŻ�end*/
							/*ѭ��������䴦������ֱ����һ�����Ų�����俪ʼ���Ż��յ�end*/
							
							printTab(tab_num+1);
							fprintf(tree,"���\n");
							statementdo(nxtlev,ptx,lev);
							while(inset(sym,statbegsys)||sym==semicolon)
							{
								if(sym==semicolon)
								{
									printTab(tab_num+1);
									fprintf(tree,";\n");
									getsymdo;
								}
								else
								{
									error(10);/*ȱ�ٷֺ�*/
								}
								printTab(tab_num+1);
								fprintf(tree,"���\n");
								statementdo(nxtlev,ptx,lev);
							}
							if(sym==endsym)
							{
								printTab(tab_num+1);
								fprintf(tree,"end\n");
								getsymdo;
							}
							else
							{
								error(17); /*ȱ��end��ֺ�*/
							}
						}
						else
						{
							if(sym==whilesym)/*׼������while��䴦��*/
							{
								printTab(tab_num+1);
								fprintf(tree,"while\n");
								cx1=cx;        /*�����ж�����������λ��*/
								getsymdo;
								memcpy(nxtlev,fsys,sizeof(bool)*symnum);
								nxtlev[dosym]=true;/*�������Ϊdo*/
								printTab(tab_num+1);
								fprintf(tree,"����\n");
								conditiondo(nxtlev,ptx,lev);  /*������������*/
								cx2=cx;       /*����ѭ����Ľ�������һ��λ��*/
								if(sym==dosym)
								{
									printTab(tab_num+1);
									fprintf(tree,"do\n");
									getsymdo;
								}
								else
								{
									error(18);      /*ȱ��do*/
								}
								printTab(tab_num+1);
								fprintf(tree,"���\n");
								statementdo(fsys,ptx,lev); /*ѭ����*/
								code[cx2].a=cx;   /*��������ѭ���ĵ�ַ����if����*/
							}
							else
							{
								memset(nxtlev,0,sizeof(bool)*symnum);/*�������޲��ȼ���*/
								testdo(fsys,nxtlev,19);/*�������������ȷ��*/
							}
						}
					}
				}
			}
		}
	}
	tab_num--;
	return 0;
}
/*
*���ʽ����
*/
int expression(bool*fsys,int*ptx,int lev)
{
	enum symbol addop;                    /*���ڱ���������*/
	bool nxtlev[symnum];
	tab_num++;
	if(sym==plus||sym==minus)             /*��ͷ�������ţ���ʱ��ǰ���ʽ������һ�����Ļ򸺵���*/
	{
		printTab(tab_num+1);
		if(sym==plus)
		{
			fprintf(tree,"+\n");
		} 
		else
		{
			fprintf(tree,"-\n");
		}
		addop=sym;                    /*���濪ͷ��������*/
		getsymdo;
		memcpy(nxtlev,fsys,sizeof(bool)*symnum);
		nxtlev[plus]=true;
		nxtlev[minus]=true;
		printTab(tab_num+1);
		fprintf(tree,"��\n");
		termdo(nxtlev,ptx,lev);                /*������*/
	}
	else                             /*��ʱ���ʽ��������ļӼ�*/
	{
		printTab(tab_num+1);
		fprintf(tree,"��\n");
		memcpy(nxtlev,fsys,sizeof(bool)*symnum);
		nxtlev[plus]=true;
		nxtlev[minus]=true;
		termdo(nxtlev,ptx,lev);            /*������*/
	}
	while(sym==plus||sym==minus)
	{
		printTab(tab_num+1);
		if(sym==plus)
		{
			fprintf(tree,"+\n");
		}
		else
		{
			fprintf(tree,"-\n");
		} 
		addop=sym;
		getsymdo;
		memcpy(nxtlev,fsys,sizeof(bool)*symnum);
		nxtlev[plus]=true;
		nxtlev[minus]=true;
		printTab(tab_num+1);
		fprintf(tree,"��\n");
		termdo(nxtlev,ptx,lev);              /*������*/
		if(addop==plus)
		{
            if(expr_flag == true) {
				double num1 = num_stack.top();
				num_stack.pop();
				double num2 = num_stack.top();
				num_stack.pop();
				num_stack.push(num1 + num2);           	
			}
		}
		else
		{
            if(expr_flag == true) {
				double num1 = num_stack.top();
				num_stack.pop();
				double num2 = num_stack.top();
				num_stack.pop();
				num_stack.push(num2 - num1);           	
			}		 
		}
	}
	tab_num--;
	return 0;
}
/*
*���
*/
int term(bool*fsys,int *ptx,int lev)
{
	tab_num++;
    enum symbol mulop;               /*���ڱ���˳�������*/
    bool nxtlev[symnum];
    memcpy(nxtlev,fsys,sizeof(bool)*symnum) ;
    nxtlev[times]=true;
    nxtlev[slash]=true;
    printTab(tab_num+1);
    fprintf(tree,"����\n");
    factordo(nxtlev,ptx,lev);       /*��������*/
    while(sym==times||sym==slash)
    {
    	printTab(tab_num+1);
    	if(sym==times)
		{
			fprintf(tree,"*\n");
		} 
		else
		{
		    fprintf(tree,"/\n");
		}
		mulop=sym;
        getsymdo;
        printTab(tab_num+1);
        fprintf(tree,"����\n");
        factordo(nxtlev,ptx,lev);
        if(mulop==times)
        {
            if(expr_flag == true) {
				double num1 = num_stack.top();
				num_stack.pop();
				double num2 = num_stack.top();
				num_stack.pop();
				num_stack.push(num1 * num2);           	
			}
        }
        else
        {
            if(expr_flag == true) {
				double num1 = num_stack.top();
				num_stack.pop();
				double num2 = num_stack.top();
				num_stack.pop();
				num_stack.push(num2 / num1);           	
			}        
        }
    }
    tab_num--;
     return 0;
}
/*
*���Ӵ���
*/
int factor(bool*fsys,int *ptx,int lev)
 {
 	tab_num++;
    int i;
    bool nxtlev[symnum];
    testdo(facbegsys,fsys,24);           /*������ӵĿ�ʼ���ú�*/
    while(inset(sym,facbegsys))          /*ѭ��ֱ���������ӿ�ʼ����*/
    {
        if(sym==ident)                   /*����Ϊ�������߱���*/
        {
        	printTab(tab_num+1);
        	fprintf(tree,"��ʶ��\n");
        	printTab(tab_num+2);
        	fprintf(tree,"%s\n",a);
        	expr_flag = false;
            i=position(id,*ptx);        /*��������*/
            if(i==0)
            {
                error(11);               /*��ʶ��δ����*/
            }
            else
            {
				switch(table[i].kind)
				{
					case constant:                                      /*����Ϊ����*/
						break;
					case variable:                                      /*����Ϊ����*/
						break;
					case procedur:                                      /*����Ϊ����*/
						error(21);                                       /*����Ϊ����*/
						break;
				}
			}
			getsymdo;
		}
		else
		{
			if(sym==number)                                             /*����Ϊ��*/
			{
				printTab(tab_num+1);
				fprintf(tree,"integer\n");
				printTab(tab_num+2);
				fprintf(tree,"%.3f\n",num);
				if(num>amax)
				{
					error(31);
					num=0;
				}
				getsymdo;
			}
			else
			{
				if(sym==lparen)                                           /*����Ϊ���ʽ*/
				{
					printTab(tab_num+1);
					fprintf(tree,"(\n");
					getsymdo;
					memcpy(nxtlev,fsys,sizeof(bool)*symnum);
					nxtlev[rparen]=true;
					printTab(tab_num+1);
					fprintf(tree,"���ʽ\n");
					expressiondo(nxtlev,ptx,lev);
					expr_flag = true;
					
					if(sym==rparen)
					{
						printTab(tab_num+1);
						fprintf(tree,")\n");
						getsymdo;
					}
					else
					{
						error(22);                                       /*ȱ��������*/
					}
				}
				testdo(fsys,facbegsys,23);                        /*���Ӻ��зǷ�����*/
			}
		}
	}
	tab_num--;
	return 0;
}
/*��������*/
int condition(bool* fsys,int* ptx,int lev)
{
	tab_num++;
    enum symbol relop;
    bool nxtlev[symnum];
    if(sym==oddsym)                        /*׼������odd���㴦��*/
   	{
   		printTab(tab_num+1);
   		fprintf(tree,"odd\n");
		getsymdo;
		printTab(tab_num+1);
		fprintf(tree,"���ʽ\n");		
		expressiondo(fsys,ptx,lev);
   	 	//
   	 	if(expr_flag) {
   	 		//fprintf(exprFile, "%lf\n", num_stack.top());
			num_stack.pop();   	 				
		} 
   	 	expr_flag = true;
   	 	//
    }
    else
    {
		memcpy(nxtlev,fsys,sizeof(bool)*symnum);
		nxtlev[eql]=true;
		nxtlev[neq]=true;
		nxtlev[lss]=true;
		nxtlev[leq]=true;
		nxtlev[gtr]=true;
		nxtlev[geq]=true;
		printTab(tab_num+1);
		fprintf(tree,"���ʽ\n");
		expressiondo(nxtlev,ptx,lev);
   	 	//
   	 	if(expr_flag) {
   	 		//fprintf(exprFile, "%lf\n", num_stack.top());
			num_stack.pop();   	 				
		} 
   	 	expr_flag = true;
   	 	// 				
							
		if(sym!=eql&&sym!=neq&&sym!=lss&&sym!=leq&&sym!=gtr&&sym!=geq)
		{
			error(20);
		}
		else
		{
			relop=sym;
			printTab(tab_num+1);
			if (relop == eql)
			{
				fprintf(tree, "=\n");
			}
			else if (relop == neq)
			{
				fprintf(tree, "#\n");
			}
			else if (relop == lss)
			{
				fprintf(tree, "<\n");
			}
			else if (relop == geq)
			{
				fprintf(tree, ">=\n");
			}
			else if (relop == gtr)
			{
				fprintf(tree, ">\n");
			}
			else if (relop == leq)
			{
				fprintf(tree, "<=\n");
			}
			getsymdo;
			printTab(tab_num+1);
			fprintf(tree, "���ʽ\n");
			expressiondo(fsys,ptx,lev);
   	 		//
   	 		if(expr_flag) {
   	 			//fprintf(exprFile, "%lf\n", num_stack.top());
				num_stack.pop();   	 				
			} 
   	 		expr_flag = true;
   	 		//					
		}
    }
    tab_num--;
    return 0;
}                                                  

/*ͨ�����̻�ַ����1����̵Ļ�ַ*/
int base(int l,int * s,int b)
{
	int b1;
	b1=b;
	 while(l>0)
	 {
		 b1=s[b1];
     	 l--;
	 }
	 return b1;
}
