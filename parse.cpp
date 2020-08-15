//A.2     C      版   本

/*编译和运行环境:
*1Visual C++6.0,VisualC++.NET and Visual C++.NET 2003
*WinNT, Win 200, WinXP and  Win2003 
*2 gcc version 3.3.2  20031022(Red Hat Linux 3.3.2-1)
*Redhat Fedora core 1
*Intel 32 platform
*使用方法:
*运行后输入PL/0 源程序文件名
*回答是否输出虚拟机代码
*回答是否输出名字表
*fa.tmp 输出虚拟机代码
*fa1.tmp  输出源文件及其各行对应的首地址
*fa2.tmp  输出结果 
*fas.tmp  输出名字表
*/
#include <stdio.h>
#include "pl0.h"
#include "string.h"
/*解释执行时使用的栈*/
#define stacksize 500
#include<stack>
using namespace std;
stack<double>num_stack;	//定义数字栈 

int tab_num = 0; //语法树缩进数量
FILE *tree;//语法树文件指针 
void printTab(int t); //缩进控制显示格式 

int main()
{
	//exprFile = fopen("expression_result.txt", "w");
	errorFile = fopen("error_result.txt", "w"); 
//	if(exprFile == NULL) {
//		printf("无法创建文件\n");
//		return 0;
//	}			
	bool nxtlev[symnum];
	printf("Input pl/0 file ?");
	scanf("%s",fname);                                     /*输入文件名*/
	fin=fopen(fname,"r");
	tree = fopen("tree.txt", "w");
	if(fin)
	{
		init();                                          //初始化*/
		err=0;
		cc=cx=ll=0;
		ch=' ';
		if(-1!=getsym())
		{
			fa=fopen("fa.tmp","w");
			fas=fopen("fas.tmp","w");
			addset(nxtlev,declbegsys,statbegsys,symnum);
			nxtlev[period]=true;
			fprintf(tree,"程序\n");
			printTab(tab_num+1);
			fprintf(tree,"分程序\n");
			    
			if(-1==block(0,0,nxtlev))			/*调用编译程序*/
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
		fprintf(errorFile, "%d, 输入正确，没有词法或语法错误\n", line_cnt);
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
*初始化
*/
void init()
{
	//清空栈 
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
	/*设置保留字名字,按照字母顺序,便于折半查找*/
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
	/*设置保留字符号*/
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

	/*设置符号集*/
	for(i=0;i<symnum;i++)
	{
		declbegsys[i]=false;
		statbegsys[i]=false;
		facbegsys[i]=false;
	}

	/*设置声明开始符号集*/
	declbegsys[constsym]=true;
	declbegsys[varsym]=true;
	declbegsys[procsym]=true;
	/*设置语句开始符号集*/
	statbegsys[beginsym]=true;
	statbegsys[callsym]=true;
	statbegsys[ifsym]=true;
	statbegsys[whilesym]=true;
	/*设置因子开始符号集*/
	facbegsys[ident]=true;
	facbegsys[number]=true;
	facbegsys[lparen]=true;
}
 /*
  *用数组实现集合的集合运算
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
 *出错处理，打印出错位置和错误编码
 */
void error(int n)
{
	error_flag = false;
	//printf("\nerror : %d\n", n); 
	char space[81];
	memset(space,32,81); 
	switch(n) {
		case 1: fprintf(errorFile, "%d, 常数说明中的=写成:=\n", line_cnt);
			break;
		case 2: fprintf(errorFile, "%d, 常数说明中的=后应是数字\n", line_cnt);
			break;
		case 3: fprintf(errorFile, "%d, 常数说明中的标识符后应是=\n", line_cnt);
			break;
		case 4: fprintf(errorFile, "%d, const，var，procedure后应是标识符\n", line_cnt);
			break;
		case 5: fprintf(errorFile, "%d, 漏掉了.或;\n", line_cnt);
			break;
		case 6: fprintf(errorFile, "%d, 过程说明后的符号不正确（应是语句开始符，或过程定义符）\n", line_cnt);
			break;
		case 7: fprintf(errorFile, "%d, 应是语句开始符\n", line_cnt);
			break;
		case 8: fprintf(errorFile, "%d, 程序体内语句部分的后跟符不正确\n", line_cnt);
			break;								
		case 9: fprintf(errorFile, "%d, 程序结尾丢了句号.\n", line_cnt);
			break;
		case 10: fprintf(errorFile, "%d, 语句之间漏了;\n", line_cnt);	
			break;
		case 11: fprintf(errorFile, "%d, 标识符未说明\n", line_cnt);
			break;
		case 12: fprintf(errorFile, "%d, 赋值语句中，赋值号左部标识符属性应是变量\n", line_cnt);
			break;
		case 13: fprintf(errorFile, "%d, 赋值语句左部标识符后应是赋值号:=\n", line_cnt);
			break;
		case 14: fprintf(errorFile, "%d, call后应为标识符\n", line_cnt);
			break;
		case 15: fprintf(errorFile, "%d, call后标识符属性应为过程\n", line_cnt);
			break;
		case 16: fprintf(errorFile, "%d, 条件语句中丢了then\n", line_cnt);
			break;	
		case 17: fprintf(errorFile, "%d, 丢了end或;\n", line_cnt);
			break;
		case 18: fprintf(errorFile, "%d, while型循环语句中丢了do\n", line_cnt);
			break;
		case 19: fprintf(errorFile, "%d, 语句后的符号不正确\n", line_cnt);
			break;
		case 20: fprintf(errorFile, "%d, 应为关系运算符\n", line_cnt);
			break;
		case 21: fprintf(errorFile, "%d, 表达式内标识符属性不能是过程\n", line_cnt);
			break;
		case 22: fprintf(errorFile, "%d, 表达式中漏掉右括号)\n", line_cnt);
			break;
		case 23: fprintf(errorFile, "%d, 因子后的非法符号\n", line_cnt);
			break;
		case 24: fprintf(errorFile, "%d, 表达式的开始符不能是此符号\n", line_cnt);
			break;								
		case 31: fprintf(errorFile, "%d, 数越界\n", line_cnt);
			break;
		case 32: fprintf(errorFile, "%d, read语句括号中的标识符不是变量\n", line_cnt);
			break;				
		default: fprintf(errorFile, "%d, 出错\n", line_cnt);
			break;
	} 	
	space[cc-1]=0;//出错时当前符号已经读完，所以cc-1
	err++;
}
/*
 *  漏掉空格，读取一个字符
 *  每次读一行，存入line缓冲区，line被getsym取空后再读一行
 *  被函数getsym调用
 */
int getch()
{
	if(cc==ll)
    {   	
		if(feof(fin))
		{
			//printf("program incomplete");
			//fprintf(errorFile, "%d, 程序结尾丢了句号.\n", line_cnt - 1);
			error(9); 
			return -1;
		}
	    if(error_flag && line_cnt != 0) {
			fprintf(errorFile, "%d, 输入正确，没有词法或语法错误\n", line_cnt);
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
			//过滤注释 
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
 /*词法分析，获取一个符号
 */
int getsym()	
{		
	int i,j,k;
	while( ch==' '||ch==10||ch==9)	//删除前导空格 
	{
		getchdo;
		printf("%c",ch);
	}
	if(ch>='a'&&ch<='z')	// 当前token的第一个字符为字母，可能是标识符或保留字
	{ 
		k=0;	// 用于记录token长度
		do{		// 读完一个token
			if(k<al)		//保证token长度不超过标识符最大长度 
			{ 
				a[k]=ch;
				k++;
			}
			getchdo; 
			printf("%c",ch);
		}while(ch>='a'&&ch<='z'||ch>='0'&&ch<='9');
		a[k]=0;
		strcpy(id,a);	//存放当前token的名字
		i=0;
		j=norw-1;
		do{				// 在保留字表中二分查找该串是否存在
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
		if(i-1>j)		// 找到则标记保留字
		{
			sym=wsym[k];
		}
		else			// 否则标记为标识符
		{
			sym=ident;
		}
	}
	else
	{
		if(ch>='0'&&ch<='9')		// 首字符为数字，则将其解析为数值
		{
			k=0;
			num=0;
			sym=number;
			do{						// 不断读取、保存数字
				num=10*num+ch-'0';
				k++;
				getchdo;
				printf("%c",ch);
			}while(ch>='0'&&ch<='9'); /*获取数字的值*/
			
			//浮点数处理 
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
			if(k>nmax)				// 位数超限
			{
				error(30);
			}
		}
		else
		{
			if(ch==':')             /*检测赋值符号*/  // 首字符为“:”，则可能是赋值
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
					sym=nul;            /*不能识别的符号*/
				}
			}
			else
			{
				if(ch=='<')         /*检测小于或小于等于符号*/
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
					if(ch=='>')          /*检测大于或大于等于符号*/
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
					else		// 均不是上述情况，则查询符号表，赋值
					{
						sym=ssym[ch];/* 当符号不满足上述条件时，全部按照单字符号处理*/
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
*测试当前符号是否合法
*
*在某一部分（如一条语句，一个表达式）将要结束时时我们希望下一个符号属于某集合
*（该部分的后跟符号） test 负责这项检测，并且负责当检测不通过时的补救措施
*程序在需要检测时指定当前需要的符号集合和补救用的集合（如之前未完成部分的后跟
*符号），以及不通过时的错误号
*
*S1：我们需要的符号
*s2:如果不是我们需要的，则需要一个补救用的集合
*n:错误号
*/
int test(bool* s1,bool* s2,int n)
{
    if(! inset(sym,s1))
    {
		error(n);
		/*当检测不通过时，不停获取符号，直到它属于需要的集合或补救的集合*/
		while((! inset(sym,s1))&&(! inset(sym,s2)))
		{
			getsymdo; 
		}
    }
    return 0;
}
/*
 *编译程序主体
 *
 *lev:当前分程序所在层
 *tx:名字表当前尾指针
 *fsys:当前模块后跟符号集合
 */
int block(int lev,int tx,bool* fsys)
{
    int i;
    int dx;                         /*名字分配到的相对地址*/
    int tx0;                        /*保留初始tx*/
    int cx0;                        /*保留初始cx*/
    bool nxtlev[symnum];            /*在下级函数的参数中，符号集合均为值参，但由于使用数组
                                      实现，传递进来的是指针，为防止下级函数改变上级函数的
                                      集合，开辟新的空间传递给下级函数*/
    dx=3;
    tx0=tx;                         /*记录本层名字的初始位置*/
    table[tx].adr=cx;
    tab_num++;
    if(lev > levmax)
    {
		error(32);                                                                                                                                         
    }
    do{
        if(sym==constsym)         /*收到常量声明符号，开始处理常量声明*/
        {
			
			printTab(tab_num+1);
			fprintf(tree,"const\n");
			getsymdo;
			do{
				printTab(tab_num+1);
				fprintf(tree,"常量定义\n");
				constdeclarationdo(&tx,lev,&dx);   /*dx的值会被constdeclaration改变，使用
                                                     指针*/
                while(sym==comma)
                {
                   printTab(tab_num+1);
                   fprintf(tree,",\n");
                   printTab(tab_num+1);
                   fprintf(tree,"常量定义\n");
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
					error(5); /*漏掉了逗号或者分号*/
				}
			}while(sym==ident);
		}
		if(sym==varsym)/*收到变量声名符号，开始处理变量声名*/
		{
			printTab(tab_num+1);
			fprintf(tree,"var\n");
			getsymdo;
			do{
				printTab(tab_num+1);
				fprintf(tree,"变量定义\n");
				vardeclarationdo(&tx,lev,&dx);
				while(sym==comma)
				{
					printTab(tab_num+1);
					fprintf(tree,"，\n");
					printTab(tab_num+1);
					fprintf(tree,"变量定义\n");
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
		while(sym==procsym)/*收到过程声名符号，开始处理过程声名*/
		{
			printTab(tab_num+1);
			fprintf(tree,"procedure\n");
			getsymdo;
			if(sym==ident)
			{
				printTab(tab_num+1);
				fprintf(tree,"标识符\n");
				printTab(tab_num+2);
				fprintf(tree,"%s\n",a);
				enter(procedur,&tx,lev,&dx);/*记录过程名字*/
				getsymdo;
			}
			else
			{
				error(4);/*procedure后应为标识符*/
			}
			if(sym==semicolon)
			{
				printTab(tab_num+1);
				fprintf(tree,";\n");
				getsymdo;
			}
			else
			{
				error(5);/*漏掉了分号*/
			}
			memcpy(nxtlev,fsys,sizeof(bool)*symnum);
			nxtlev[semicolon]=true;
			printTab(tab_num+1);
			fprintf(tree,"block\n");
			if(-1==block(lev+1,tx,nxtlev))
			{
				return -1;/*递归调用*/
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
                 error(5);                       /*漏掉了分号*/
             }
        }
        memcpy(nxtlev,statbegsys,sizeof(bool)*symnum);
        nxtlev[ident]=true;
        nxtlev[period]=true;
        testdo(nxtlev,declbegsys,7);
    }while(inset(sym,declbegsys));                /*直到没有声明符号*/
    code[table[tx0].adr].a=cx;                    /*开始生成当前过程代码*/
    table[tx0].adr=cx;                            /*当前过程代码地址*/
    table[tx0].size=dx;                           /*声明部分中每增加一条声明都会给dx增加1,声明部分已经结束,dx就是当前过程数据的size*/
        
    cx0=cx;
    if(tableswitch)                               /*输出名字表*/
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
	/*语句后跟符号为分号或end*/
	memcpy(nxtlev,fsys,sizeof(bool)*symnum);/*每个后跟符号集和都包含上层后跟符号集和，以便补救*/
	nxtlev[semicolon]=true;
	nxtlev[endsym]=true;
	printTab(tab_num+1);
	fprintf(tree,"语句\n");
	statementdo(nxtlev,&tx,lev);
	memset(nxtlev,0,sizeof(bool)*symnum); /*分程序没有补救集合*/
	test(fsys,nxtlev,8);                  /*检测后跟符号正确性*/
	tab_num--;
	return 0;
}
/*
*在名字表中加入一项
*
*k:名字种类const,var or procedure
*ptx:名字表尾指针的指针，为了可以改变名字表尾指针的数值
*lev:名字所在的层次，以后所有的lev都是这样
*pdx:为当前应分配的变量的相对地址，分配后要增加1
*/
void enter (enum object k,int *ptx,int lev, int *pdx)
{
	(*ptx)++;
	strcpy(table[(*ptx)].name,id);       /*全局变量id中已存有当前名字的名字*/
	table[(*ptx)].kind=k;
	switch(k)
	{
		case constant:                      /*常量名字*/
			if (num>amax)
			{
				error(31);
				num=0;
			}
			table[(*ptx)].val=num;
			break;
		case variable:                     /*变量名字*/
			table[(*ptx)].level=lev;
			table[(*ptx)].adr=(*pdx);
			(*pdx)++;
			break;                          /*过程名字*/
		case procedur:
			table[(*ptx)].level=lev;
			break;
	}

}
/*
 *查找名字的位置
 *找到则返回在名字表中的位置，否则返回0
 *
 *idt: 要查找的名字
 *tx:：当前名字表尾指针
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
 *常量声明处理
 */
int constdeclaration(int *  ptx,int lev,int *  pdx)
{
	tab_num++;
	if(sym==ident)
	{
		printTab(tab_num+1);
		fprintf(tree,"标识符\n");
		printTab(tab_num+2);
		fprintf(tree,"%s\n",a);
		getsymdo;
		if(sym==eql ||sym==becomes)
		{
			printTab(tab_num+1);
			fprintf(tree,"=\n");
			if(sym==becomes)
			{
				error(1);                     /*把=写出成了：=*/
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
				error(2);                  /*常量说明=后应是数字*/
			}         
		}
		else
		{
			error(3);                       /*常量说明标识后应是=*/
		}
	}
	else
	{
		error(4);                        /*const后应是标识*/
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
 		fprintf(tree,"标识符\n");
 		printTab(tab_num+2);
 		fprintf(tree,"%s\n",a);
 		enter(variable,ptx,lev,pdx);//填写名字表
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
*语句处理
*/
int statement(bool* fsys,int * ptx,int lev)
{
	int i,cx1,cx2;
   	bool nxtlev[symnum];
   	tab_num++;
   	if(sym==ident)
   	{
   		printTab(tab_num+1);
   		fprintf(tree,"标识符\n");
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
   	 			fprintf(tree,"表达式\n");
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
    	   	   			fprintf(tree,"标识符\n");
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
				}while (sym==comma);	/*一条read语句可读多个变量 */
			}
			if(sym!=rparen)
			{
				error(33);			/* 格式错误，应是右括号*/
				while(!inset(sym,fsys))/* 出错补救，直到收到上层函数的后跟符号*/
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
			if(sym==writesym)			/* 准备按照write语句处理，与read类似*/
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
						nxtlev[comma]=true;		/* write的后跟符号为）or，*/
						printTab(tab_num+1);
						fprintf(tree,"表达式\n");
						expressiondo(nxtlev,ptx,lev);/* 调用表达式处理，此处与read不同，read为给变量赋值*/
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
						error(33);/* write()应为完整表达式*/
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
				if(sym==callsym)		/* 准备按照call语句处理*/
				{
					printTab(tab_num+1);
					fprintf(tree,"call\n");
					getsymdo;
					if(sym!=ident)
					{
						error(14);           /*call后应为标识符*/
					}
					else
					{
						printTab(tab_num+1);
						fprintf(tree,"标识符\n");
						printTab(tab_num+2);
						fprintf(tree,"%s\n",a);
						i=position(id,*ptx);
						if(i==0)
						{
							error(11);          /*过程未找到*/
						}
						else
						{
							if(table[i].kind!=procedur)
							{
								error(15);      /*call后标识符应为过程*/
							}
						}
						getsymdo;
					}
				}
				else
				{
					if(sym==ifsym)     /*准备按照if语句处理*/
					{
						printTab(tab_num+1);
						fprintf(tree,"if\n");
						getsymdo;
						memcpy(nxtlev,fsys,sizeof(bool)*symnum);
						nxtlev[thensym]=true;
						nxtlev[dosym]=true;    /*后跟符号为then或do*/
						printTab(tab_num+1);
						fprintf(tree,"条件\n");
						conditiondo(nxtlev,ptx,lev);   /*调用条件处理（逻辑运算）函数*/
						if(sym==thensym)
						{
							printTab(tab_num+1);
							fprintf(tree,"then\n");
							getsymdo;
						}
						else
						{
							error(16);          /*缺少then*/
						}
						cx1=cx;                /*保存当前指令地址*/	
						printTab(tab_num+1);
						fprintf(tree,"语句\n");
						statementdo(fsys,ptx,lev);   /*处理then后的语句*/
						code[cx1].a=cx;      /*经statement处理后，cx为then后语句执行完的位置，它正是前面未定的跳转地址*/
					}
					else
					{
						if(sym==beginsym)   /*准备按照复合语句处理*/
						{
							printTab(tab_num+1);
							fprintf(tree,"begin\n");
							getsymdo;
							memcpy(nxtlev,fsys,sizeof(bool)*symnum);
							nxtlev[semicolon]=true;
							nxtlev[endsym]=true;/*后跟符号为分号或end*/
							/*循环调用语句处理函数，直到下一个符号不是语句开始符号或收到end*/
							
							printTab(tab_num+1);
							fprintf(tree,"语句\n");
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
									error(10);/*缺少分号*/
								}
								printTab(tab_num+1);
								fprintf(tree,"语句\n");
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
								error(17); /*缺少end或分号*/
							}
						}
						else
						{
							if(sym==whilesym)/*准备按照while语句处理*/
							{
								printTab(tab_num+1);
								fprintf(tree,"while\n");
								cx1=cx;        /*保存判断条件超作的位置*/
								getsymdo;
								memcpy(nxtlev,fsys,sizeof(bool)*symnum);
								nxtlev[dosym]=true;/*后跟符号为do*/
								printTab(tab_num+1);
								fprintf(tree,"条件\n");
								conditiondo(nxtlev,ptx,lev);  /*调用条件处理*/
								cx2=cx;       /*保存循环体的结束的下一个位置*/
								if(sym==dosym)
								{
									printTab(tab_num+1);
									fprintf(tree,"do\n");
									getsymdo;
								}
								else
								{
									error(18);      /*缺少do*/
								}
								printTab(tab_num+1);
								fprintf(tree,"语句\n");
								statementdo(fsys,ptx,lev); /*循环体*/
								code[cx2].a=cx;   /*反填跳出循环的地址，与if类似*/
							}
							else
							{
								memset(nxtlev,0,sizeof(bool)*symnum);/*语句结束无补救集合*/
								testdo(fsys,nxtlev,19);/*检测语句结束的正确性*/
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
*表达式处理
*/
int expression(bool*fsys,int*ptx,int lev)
{
	enum symbol addop;                    /*用于保存正负号*/
	bool nxtlev[symnum];
	tab_num++;
	if(sym==plus||sym==minus)             /*开头的正负号，此时当前表达式被看作一个正的或负的项*/
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
		addop=sym;                    /*保存开头的正负号*/
		getsymdo;
		memcpy(nxtlev,fsys,sizeof(bool)*symnum);
		nxtlev[plus]=true;
		nxtlev[minus]=true;
		printTab(tab_num+1);
		fprintf(tree,"项\n");
		termdo(nxtlev,ptx,lev);                /*处理项*/
	}
	else                             /*此时表达式被看作项的加减*/
	{
		printTab(tab_num+1);
		fprintf(tree,"项\n");
		memcpy(nxtlev,fsys,sizeof(bool)*symnum);
		nxtlev[plus]=true;
		nxtlev[minus]=true;
		termdo(nxtlev,ptx,lev);            /*处理项*/
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
		fprintf(tree,"项\n");
		termdo(nxtlev,ptx,lev);              /*处理项*/
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
*项处理
*/
int term(bool*fsys,int *ptx,int lev)
{
	tab_num++;
    enum symbol mulop;               /*用于保存乘除法符号*/
    bool nxtlev[symnum];
    memcpy(nxtlev,fsys,sizeof(bool)*symnum) ;
    nxtlev[times]=true;
    nxtlev[slash]=true;
    printTab(tab_num+1);
    fprintf(tree,"因子\n");
    factordo(nxtlev,ptx,lev);       /*处理因子*/
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
        fprintf(tree,"因子\n");
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
*因子处理
*/
int factor(bool*fsys,int *ptx,int lev)
 {
 	tab_num++;
    int i;
    bool nxtlev[symnum];
    testdo(facbegsys,fsys,24);           /*检测因子的开始符好号*/
    while(inset(sym,facbegsys))          /*循环直到不是因子开始符号*/
    {
        if(sym==ident)                   /*因子为常量或者变量*/
        {
        	printTab(tab_num+1);
        	fprintf(tree,"标识符\n");
        	printTab(tab_num+2);
        	fprintf(tree,"%s\n",a);
        	expr_flag = false;
            i=position(id,*ptx);        /*查找名字*/
            if(i==0)
            {
                error(11);               /*标识符未声明*/
            }
            else
            {
				switch(table[i].kind)
				{
					case constant:                                      /*名字为常量*/
						break;
					case variable:                                      /*名字为变量*/
						break;
					case procedur:                                      /*名字为过程*/
						error(21);                                       /*不能为过程*/
						break;
				}
			}
			getsymdo;
		}
		else
		{
			if(sym==number)                                             /*因子为数*/
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
				if(sym==lparen)                                           /*因子为表达式*/
				{
					printTab(tab_num+1);
					fprintf(tree,"(\n");
					getsymdo;
					memcpy(nxtlev,fsys,sizeof(bool)*symnum);
					nxtlev[rparen]=true;
					printTab(tab_num+1);
					fprintf(tree,"表达式\n");
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
						error(22);                                       /*缺少右括号*/
					}
				}
				testdo(fsys,facbegsys,23);                        /*银子后有非法符号*/
			}
		}
	}
	tab_num--;
	return 0;
}
/*条件处理*/
int condition(bool* fsys,int* ptx,int lev)
{
	tab_num++;
    enum symbol relop;
    bool nxtlev[symnum];
    if(sym==oddsym)                        /*准备按照odd运算处理*/
   	{
   		printTab(tab_num+1);
   		fprintf(tree,"odd\n");
		getsymdo;
		printTab(tab_num+1);
		fprintf(tree,"表达式\n");		
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
		fprintf(tree,"表达式\n");
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
			fprintf(tree, "表达式\n");
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

/*通过过程基址求上1层过程的基址*/
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
