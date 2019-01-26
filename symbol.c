#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#define TableLength 30
enum tokenType { EOFILE=-1, LESS_THAN,
LESS_THAN_OR_EQUAL,GREATER_THAN,GREATER_THAN_OR_EQUAL,
EQUAL,NOT_EQUAL};
struct token 
{
char * lexeme;
int index;
unsigned int  rowno,colno;
char * type;
};
struct ListElement
{
	struct token tok;
	struct ListElement * next;

};
struct ListElement * TABLE[TableLength];
void Initialize()
{
	for(int i=0;i<TableLength;i++) TABLE[i]=NULL;
}
void printToken(struct token * tk){printf("<%s,%d,%d,%s>\n",tk->lexeme,tk->rowno,tk->colno,tk->type); fflush(stdout);}
int Hash(char * str)
{ int sum=0;

	// gi ,abhimanyu
	//sum of ascii values % TableLength
	for(int i=0;i<strlen(str);i++)
	{
       sum+=str[i];
	}
	return sum%TableLength;
}
void Display()
{
	for(int i=0;i<TableLength;i++)
	{

		printf("For Value %d :\n",i);
		struct ListElement * ele=TABLE[i];
        while(ele) {printToken(&ele->tok); ele=ele->next;}

	}
}
int Search(char * lexeme)
{
	for(int i=0;i<TableLength;i++)
	{
		if(TABLE[i]!= NULL)
		{
              struct ListElement * ele=TABLE[i];
              if(strcmp((ele->tok).lexeme,lexeme)==0)
              {
              	printf("Found matching lexeme %s\n",(ele->tok).lexeme);
              	return 1;
              }
		}
	}
	return 0;
}
void Insert(struct token * tk)
{
    if(Search(tk->lexeme) == 1)
    {
    	return;
    }
    int val=Hash(tk->lexeme);
    struct ListElement * cur=(struct ListElement *)malloc(sizeof(struct ListElement *)*999);
    tk->index=val;
    cur->tok=*tk;
    cur->next=NULL;
    if(TABLE[val]==NULL)
    {
    	TABLE[val]=cur;
    }
    else
    {
    	struct ListElement * ele;
    	ele=TABLE[val];
    	while(ele->next)
    	{
    		ele=ele->next;
    	}
    	ele->next=cur;
    }
}
struct token * createToken(char * lexemename,int row,int col,char * type)
{
struct token * tk=(struct token *)malloc(sizeof(struct token *)*999);
tk->lexeme=lexemename;
tk->rowno=row;
tk->colno=col;
tk->type=type;
return tk;
}
char * subString(char * str,int left,int right)
{  int save=0;
	//if(left == right){return str[left];}
	char * r=(char *)malloc(sizeof(char)*999);
	for(int i=left;i<=right;i++)
	{
		r[save++]=str[i];
	}
	r[save]='\0';
	printf("%s\n",r);
	return r;
}
struct token * tokenize(char * buf)
{
	struct token * tk=(struct token *)malloc(sizeof(struct token *)*999);
	int i=1;
	int n=strlen(buf)-1;
	int j=0;
	int left=1;
	//printf("%c\n",buf[n+1]);
	while(i<=n)
  {

    if(buf[i]=='>')
    {
    	printf("left %c right %c\n",buf[left],buf[i-1]);
     tk->type=subString(buf,left,i-1);	
    }
  	else if(buf[i]==','){
  		if(j==0)
  		{
  			printf("left %c right %c\n",buf[left],buf[i-1]);
        tk->lexeme=subString(buf,left,i-1);
         left=i+1;
  		}
  		else if(j==1)
  		{
  			printf("left %c right %c\n",buf[left],buf[i-1]);
  			char * r=subString(buf,left,i-1);
  			
          tk->rowno=atoi(subString(buf,left,i-1));
          left=i+1;
  		}
  		else if(j ==2)
  		{
  			printf("left %c right %c\n",buf[left],buf[i-1]);
            tk->colno=atoi(subString(buf,left,i-1));
            left=i+1;

  		}
  			j++;}
    i++;
  }

  return tk;
}
int main()
{

   for(int i=0;i<5;i++)
   {
	char * str=(char *)malloc(sizeof(char)*999);
	scanf(" %s",str);
	printf("Hash Value of %s is %d\n",str,Hash(str));
   }
FILE * fin=fopen("output.c","r");
if(fin == NULL){printf("can't open file\n"); return 0;}
printf("Opened file\n");
char buf[2455];
while(fscanf(fin,"%[^\n]*c",buf)==1)
{
//printf("%s\n",buf);
struct token * y=tokenize(buf);
printToken(tokenize(buf));
Insert(y);
for(int i=0;i<2455;i++) buf[i]=' ';
	
fseek(fin,1,SEEK_CUR);
}
printf("--------------------------------------------\n");
Display();

    



	return 0;
}
