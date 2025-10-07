#include<stdio.h>
#include<conio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>

#define n 100

int numst[n];                   // DECLARE NUM STACK
int numtp=-1;

char opst[n];                 //DECLARE OP STACK
int optp=-1;

void numpush(int data)
{
    numst[++numtp]=data;        //PUSH NUM INTO STACK
}

int numpop()
{
    if(numtp<0)
    {
        printf("STACK UNDER-FLOW");       //POP NUM FROM STACK
        exit(1);
    }
    return numst[numtp--];
}

void oppush(char ch)
{
    opst[++optp]=ch;                //PUSH OPERATOR INTO STACK
}

char oppop()
{
    if(optp<0)
    {
        printf("STACK UNDER-FLOW");        //POP OPERATOR INTO STACK
        exit(1);
    }
    return opst[optp--];
}

int priority(char ch)
{
    if(ch=='*' || ch=='/') return 2;
    if(ch=='+' || ch=='-') return 1;         //MAKE PRIORTITY TO THE OPERATORS
    return 0;
}

int calc(int a,int b,char ch,int *er)
{
    if(ch=='*') return a*b;
    if(ch=='+') return a+b;
    if(ch=='-') return a-b;
    if(ch=='/')
    {
        if(b==0)                            //CALCULATE THE OUTPUT
        {
            *er=1;
            printf("INVALID INPUT\n");
            return 0;
        }
        return a/b;
    }
    return 0;
}
int evaluate(char *expression)
{
    int er=0,i=0;
    while(expression[i])
    {
        if(isspace(expression[i]))
        {
            i++;
            continue;                            // PERFORM PRECEDENCE PUSH POP OPERATION
        }
        
        if(isdigit(expression[i]))
        {
            int x=0;
            while(isdigit(expression[i]))
            {
                x=x*10+(expression[i]-'0');
                i++;
            }
            numpush(x);
            continue;
        }
        
        if(strchr("*/+-",expression[i]))
        {
            while(optp!=-1 && priority(opst[optp])>=priority(expression[i]))
            {
                int b=numpop();
                int a=numpop();
                char c=oppop();
                int res=calc(a,b,c,&er);
                if(er)
                {
                    printf("INVALID OPERATION\n");
                    return 0;
                }
                numpush(res);
            }
            oppush(expression[i]);
            i++;
            continue;
        }
        printf("INVALID EXPRESSION\n");
        return 0;
    }
        while(optp!=-1)
        {
            int b=numpop();
            int a=numpop();
            char ch=oppop();
            int res=calc(a,b,ch,&er);
            if(er)
            {
                printf("INVALID OPERATION\n");
                return 0;
            }
            numpush(res);
        }

        return numst[numtp];
}

int main()
{
    char expression[100];
    printf("ENTER YOUR EXPRESSION\n");
    fgets(expression,sizeof(expression),stdin);
     expression[strcspn(expression, "\n")] = '\0';
     int x=evaluate(expression);
     printf("%d ",x);
}