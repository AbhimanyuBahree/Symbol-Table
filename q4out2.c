#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#define TableLength 30
int dou=0;
int a=0;
int globalcount=0;
int globalcount2=0;
char * buf[2340];
char lexemes[10000][10000];
int lindex=0;
struct symbol
{
char * name;
char * type;
int size;
char scope;
int noa;
char * arguments;
char * returntype;
int rowno;
int colno;
int index;
};
char types[1000][1000]={"int","float","double","FUNC","char","long"};
struct ListElement
{
 struct symbol sym;
 struct ListElement * next;

};
struct ListElement * TABLE[TableLength];
struct symbol * CreateSymbol(char * name,char * type,int size,char scope,int noa,char * arguments,char * returntype)
{

 struct symbol * sym=(struct symbol *)malloc(sizeof(struct symbol *)*999);
 sym->name=name;
 sym->type=type;
 sym->size=size;
 sym->scope=scope;
 sym->noa=noa;
 sym->arguments=arguments;
 sym->returntype=returntype;
 return sym;

}
void printSymbol(struct symbol sym)
{
printf("%s\t%s\t%d\t%c\t%d\t%s\t%s\t",sym.name,sym.type,sym.size,sym.scope,sym.noa,sym.arguments,sym.returntype);
}
void Initialize()
{
 for(int i=0;i<TableLength;i++) TABLE[i]=NULL;
}
int Hash(char * str)
{ int sum=0;

 for(int i=0;i<strlen(str);i++)
 {
 sum+=str[i];
 }
 return sum%TableLength;
}

int Search(char * lexeme)
{
 for(int i=0;i<TableLength;i++)
 {
 if(TABLE[i]!= NULL)
 {
 struct ListElement * ele=TABLE[i];
 if(strcmp((ele->sym).name,lexeme)==0)
 {
 printf("Found matching lexeme %s\n",(ele->sym).name);
 return 1;
 }
 }
 }
 return 0;
}
void Display()
{
 for(int i=0;i<TableLength;i++)
 {

 printf("For Value %d :\n",i);
 struct ListElement * ele=TABLE[i];
 while(ele) {printSymbol(ele->sym); ele=ele->next;}

 }
}

void Insert(struct symbol * sym)
{
 if(Search(sym->name) == 1)
 {
 return;
 }
 int val=Hash(sym->name);
 struct ListElement * cur=(struct ListElement *)malloc(sizeof(struct ListElement *)*999);
 sym->index=val;
 cur->sym=*sym;
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

int checkDelimiter(char * filename,char ch)
{
FILE * fin=fopen(filename,"r");
if(fin == NULL) {printf("Can't open file\n"); return 0;}
fseek(fin,-1,SEEK_END);
char d=(char)getc(fin);
if(ch==d) {fclose(fin); return 1;}
 else {fclose(fin);return 0;}
}
void appendch(char * filename,char ch)
{
 if(checkDelimiter(filename,ch)) return;

FILE * fin=fopen(filename,"a");
if(fin == NULL) {printf("Can't open file\n"); return 0;}
fprintf(fin,"%c",ch);
fclose(fin);
}

char * concatstr(char * str1,char * str2)
{
char * str3 = (char *)malloc(1+strlen(str1)+strlen(str2));
strcpy(str3,str1);
strcat(str3,str2); 
return str3;
}

char * concatchar(char * str1,char ch)
{
 char * str=(char *)malloc(1+1+strlen(str1));
 strcpy(str,str1);
 str[strlen(str1)]=ch;
 str[strlen(str1)+1]='\0';
 return str;
}
void readDocument(char * filename,char * delimiter){
FILE * fi=fopen(filename,"r");
if(fi == NULL) {printf("Can't open file\n"); return; }
char * str4=concatstr(concatchar("%[^",delimiter),"]%*c");
fscanf(fi,str4,buf);
fclose(fi);
return buf;
}

bool isKeyword(char* str) 
{ 
 if (!strcmp(str, "if") || !strcmp(str, "else") || 
 !strcmp(str, "while") || !strcmp(str, "do") || 
 !strcmp(str, "break") || !strcmp(str, "for")||
 !strcmp(str, "continue") || !strcmp(str, "int") 
 || !strcmp(str, "double") || !strcmp(str, "float") 
 || !strcmp(str, "return") || !strcmp(str, "char") 
 || !strcmp(str, "case") || !strcmp(str, "char") 
 || !strcmp(str, "sizeof") || !strcmp(str, "long") 
 || !strcmp(str, "short") || !strcmp(str, "typedef") 
 || !strcmp(str, "switch") || !strcmp(str, "unsigned") 
 || !strcmp(str, "void") || !strcmp(str, "static") 
 || !strcmp(str, "struct") || !strcmp(str, "goto")) 
 return (true); 
 return (false); 
} 
int isDelimiter(char ch)
{

 if (ch == ' ' || ch == '+' || ch == '-' || ch == '*' || 
 ch == '/' || ch == ',' || ch == ';' || ch == '>' || 
 ch == '<' || ch == '=' || ch == '(' || ch == ')' || 
 ch == '[' || ch == ']' || ch == '{' || ch == '}'||ch=='\n'||ch =='"'||ch=='!'||ch=='&'||ch=='|'||ch=='%'|| ch==39) 
 return (true); 
 return (false); 
} 
bool validIdentifier(char* str) 
{ 
 if (str[0] == '0' || str[0] == '1' || str[0] == '2' || 
 str[0] == '3' || str[0] == '4' || str[0] == '5' || 
 str[0] == '6' || str[0] == '7' || str[0] == '8' || 
 str[0] == '9' || isDelimiter(str[0]) == true || isKeyword(str))
 return (false); 
 return (true); 
} 

char * preprocess(char * filename)
{

char * str="q4out.c";
FILE * fa=fopen(filename,"r");
if(fa == NULL){printf("can't open file in preprocessing\n"); return str;}
FILE * fb=fopen(str,"w");
int ca,cb;
ca = getc(fa);
while (ca != EOF)
{
if (ca=='/')
{
cb = getc(fa);
if (cb == '/')
{
while(ca != '\n')
ca = getc(fa);
}
else if (cb == '*')
{
do
{
while(ca != '*')
ca = getc(fa);
ca = getc(fa);
} while (ca != '/');
}
else
{
putc(ca,fb);
putc(cb,fb);
}
}
else putc(ca,fb);
ca = getc(fa);
}
fclose(fa);
fclose(fb);
FILE * fin;
FILE * fout;
char * str2="q4out2.c";
fout=fopen(str2,"w");
fin=fopen("q4out.c","r");
if(fin == NULL)
{
printf("Can't open file\n");
exit(0);
}

ca=getc(fin);
while(ca!= EOF)
{
if(ca==' ')
{
 putc(ca,fout);
 while(ca==' ')
 ca=getc(fin);
 
}
else if(ca==' ')
{
 putc(' ',fout);
 while(ca==' ')
 ca=getc(fin);
}
else
{
putc(ca,fout);
ca=getc(fin);
}

}
fclose(fin);
fclose(fout);

fin=fopen(str2,"r");
if(fin == NULL)
{
printf("Can't open file");
exit(0);
}
char * str3="q4out3.c";
fout=fopen(str3,"w");


ca = getc(fin);
while(ca!=EOF)
{
if(ca == '#')
 {
 while(ca!='\n')
 ca = getc(fin);
 putc(ca,fout);
 }

else
{
 putc(ca,fout);
}
ca=getc(fin);
}

fclose(fin);
fclose(fout);


return str3;
}
char* subString(char* str, int left, int right) 
{ 
 int i; 
 char* subStr = (char*)malloc( 
 sizeof(char) * (right - left + 2)); 
 
 for (i = left; i <= right; i++) 
 subStr[i - left] = str[i]; 
 subStr[right - left + 1] = '\0'; 
 return (subStr); 
}
void * parse(char * str)
{
int left=0; int right=0; int len=strlen(str);
while(right<=len && left<=right)
{
 if(isDelimiter(str[right])==false) right++;
 else if(isDelimiter(str[right])==true && left==right)
 {
 printf("name = %c\n",str[right]);

 strcpy(lexemes[lindex++],subString(str,right,right));
 right++;
 left=right;
 }
 else if (isDelimiter(str[right]) == true && left != right || (right == len && left != right))
 {
 strcpy(lexemes[lindex++],subString(str,left,right-1));
 printf("name = %s \n",subString(str,left,right-1));
 printf("name = %c\n",str[right]);
 strcpy(lexemes[lindex++],subString(str,right,right));
 right++;
 left=right;
 }

}

}
int sizer(char * type)
{
 if(strcmp(type,"int")==0)
 {
 return sizeof(int);
 }
 else if(strcmp(type,"double")==0)
 {
 return sizeof(double);
 }

 else if(strcmp(type,"float")==0)
 {
 return sizeof(float);
 }
 else if(strcmp(type,"void")==0)
 {
 return sizeof(void);
 }

 else if(strcmp(type,"char")==0)
 {
 return sizeof(char);
 }

 else if(strcmp(type,"long")==0)
 {
 return sizeof(long);
 }
 else if(strcmp(type,"char *")==0)
 {
 return sizeof(char *);
 }

 
}
void * parse2()
{ int i=0;

 while(i<lindex)
 {
 if(lexemes[i][0]=='"')
 {
 dou++;
 }
 if(lexemes[i][0]==39)
 {
 a++;
 }
 if(lexemes[i][0]=='{'){globalcount++;}

 if(lexemes[i][0]=='}'){globalcount2++;}
 if(validIdentifier(lexemes[i]) && dou%2==0 && a%2==0)
 {
 if(i<=lindex-2 && lexemes[i+1][0]=='(')
 {printf("%s Function i= %d\n",lexemes[i],i);
 
 
 char * type="void"; 
 int j=i-2; int s=0;
 while(lexemes[j][0]!='\n')
 {printf("%s ",lexemes[j]);
 for(int v=0;v<6;v++){
 
 if(strcmp(lexemes[j],types[v])==0)
 {
 type=lexemes[j];
 s=1;
 break;
 }
 
 }
 if(s) break;
 j--;
 }

 j=i+2;
 int commas=0;
 while(lexemes[j][0]!=')')
 {
 if(lexemes[j][0]==',')
 {
 commas++;
 }
 j++;
 }
 int noa;
 if(commas != 0)
 {
 noa=commas+1;
 }
 if(commas == 0)
 {
 int u = i+1;
 if(lexemes[u][0]=='('&& lexemes[u+1][0]==')') noa=0;
 else noa=1;
 }

 char ch='G';if(globalcount != globalcount2) ch='L';
 char *c;
 if(noa == 0) {c=NULL;}
 
 Insert(CreateSymbol(lexemes[i],"FUNC",sizer(type),'G',noa,c,type));
 }
 else {printf("%s Identifier\n",lexemes[i]);

 char ch='G';if(globalcount != globalcount2) ch='L'; 
 char * type="void"; 
 int j=i-2; int s=0;
 while(lexemes[j][0]!='\n')
 {
 printf("%s->",lexemes[j]);
 for(int v=0;v<6;v++){
 
 if(strcmp(lexemes[j],types[v])==0)
 {
 printf("hu\n");
 type=lexemes[j];
 s=1;
 break;
 }
 
 
 }
 if(s) break;
 j--;
 }
 printf("done\n");
 Insert(CreateSymbol(lexemes[i],type,sizer(type),ch,-1,NULL,NULL));

 }
 }
 i++;
 fflush(stdout);
 }
}
int main()
{
char * filename1="example.c";
char delimiter='@';
char * filename=preprocess(filename1); appendch(filename,delimiter);
 readDocument(filename,delimiter);
printf("%s",buf);
printf("\ndone\n");
parse(buf);
printf("\ndone\n");
printf("lindex=%d\n",lindex);

printf("yo\n");
parse2();
printf("-------------------------------------------------------------------------------\n");
Display();
return 0;
}

