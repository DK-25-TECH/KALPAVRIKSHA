#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define uf "users.txt"

typedef struct user{
    int id;
    char name[100];
    int age;
}user;

void createuser();
void deleteuser();
void updateuser();
void displayuser();

void createuser()
{
    FILE *fp=fopen(uf,"a");
    if(fp==NULL)
    {
        printf("UNABLE TO OPEN !!\n");
        return;
    }
    
    user u;
    printf("ENTER USER ID : ");
    scanf("%d",&u.id);
    printf("ENTER USER-NAME : ");
    scanf("%s",u.name);
    printf("ENTER USER AGE : ");
    scanf("%d",&u.age);
    
    fprintf(fp,"%d %s %d\n",u.id,u.name,u.age);
    fclose(fp);
    
    printf("USER ADDED TO THE FILE SUCCESSFULLY\n");
}

void displayuser()
{
    FILE *fp=fopen(uf,"r");
    if(fp==NULL)
    {
        printf("UNABLE TO OPEN THE FILE");
        return;
    }
    user u;
    
    while(fscanf(fp,"%d %s %d",&u.id,u.name,&u.age)==3)
    {
        printf("%d %s %d\n",u.id,u.name,u.age);
    }
    fclose(fp);
}

void updateuser()
{
    FILE *fp=fopen(uf,"r");
    if(fp==NULL)
    {
        printf("UNABLE TO OPEN THE FILE");
        return;
    }
    
    int id,x=0;
    printf("ENTER USER ID : ");
    scanf("%d",&id);
    user u;
    
    FILE *tp=fopen("tp.txt","w");
    if(tp==NULL)
    {
        printf("UNABLE TO CREATE FILE");
        fclose(tp);
        return;
    }
    
    while(fscanf(fp,"%d %s %d",&u.id,u.name,&u.age)==3)
    {
        if(u.id==id)
        {
            x=1;
            printf("ENTER NEW USER-NAME : ");
            scanf("%s",u.name);
            printf("ENTER NEW USER AGE : ");
            scanf("%d",&u.age);
        }
        fprintf(tp,"%d %s %d\n",u.id,u.name,u.age);
    }
    fclose(fp);
    fclose(tp);
    remove(uf);
    rename("tp.txt",uf);
    
    if(x)
    {
        printf("UPDATED SUCCESSFILLY");
    }
    else{
        printf("ID NOT FOUND");
    }
}

void deleteuser()
{
    FILE *fp=fopen(uf,"r");
    if(fp==NULL)
    {
        printf("UNABLE TO CREATE FILE");
        return;
    }
    
    int id,x=0;
    printf("ENTER ID TO DELETE : ");
    scanf("%d",&id);
    
    user u;
    FILE *tp=fopen("tp.txt","w");
    if(tp==NULL)
    {
        printf("UNABLE TO CREATE FILE");
        return;
    }
    
    while(fscanf(fp,"%d %s %d",&u.id,u.name,&u.age)==3)
    {
        if(u.id==id)
        {
            x=1;
            continue;
        }
        fprintf(tp,"%d %s %d\n",u.id,u.name,u.age);
    }
    fclose(fp);
    fclose(tp);
    remove(uf);
    rename("tp.txt",uf);
    
    if(x)
    {
        printf("DELETED SUCCESSFULLY");
    }
    else{
        printf("USER NOT FOUND");
    }
}

int main()
{
    int choice;
    FILE *fp;
    
    fp=fopen(uf,"a");
    
    if(fp==NULL)
    {
        printf("ERROR NOT FOUND");
        return 1;
    }
    fclose(fp);
    
    while(1)
    {
        printf("USER MANAGEMENT SYSTEM \n");
        printf("1. ADD USER \n");
        printf("2. DISPLAY USERS \n");
        printf("3. UPDATE BY ID \n");
        printf("4. DELETE BY ID \n");
        printf("5. EXIT \n");
        printf("\n");
        printf("ENTER YOUR CHOICE : \n");
        scanf("%d",&choice);
        
        switch(choice)
        {
            case 1: createuser(); break;
            case 2: displayuser(); break;
            case 3: updateuser(); break;
            case 4: deleteuser(); break;
            case 5: exit(0);
            default: printf("INVALID CHOICE\n");
        }
    }
    return 0;
}