#include<stdio.h>

struct student
{ 
    int rollNo;
    char name[25];
    int mark1,mark2,mark3;  
};
char gradeCalc(float avg)
{ 
    int range = ((int)avg/10);
    
    switch(range) 
    { 
        case 10:
        case 9:
        case 8:
        return 'A';
        
        case 7:
        return 'B';
        
        case 6:
        case 5:
        return 'C';
        
        case 4:
        return 'D';
        
        default:
        return 'F';
        
        } 
    
} 

void printStars(char grade) 
{
    switch(grade)
    { 
        case 'A':
        printf("PERFORMANCE : *****\n");
        break; 
        
        case 'B':
        printf("PERFORMANCE : ****\n");
        break; 
        
        case 'C':
        printf("PERFORMANCE : ***\n"); 
        break;
        
        case 'D': 
        printf("PERFORMANCE : **\n");
        break;
        
        default:
        printf("PERFORMANCE : FAIL\n");
        break;
        
        }
        
}
        
void progressCalculator(struct student s,char (*gradeFptr)(float),void (*starFptr)(char)) 
{ 
    int total = s.mark1 + s.mark2 + s.mark3;
    
    float avg = total / 3.0;
    
    char grade = gradeFptr(avg);
    
    printf("\nROLL_NO : %d \nNAME : %s\nTOTAL-MARKS : %d \nAVERAGE-MARKS : %.2f\nGRADE : %c\n", s.rollNo,s.name,total,avg,grade);
    
    starFptr(grade);
    
}
void displayRollno(struct student s[],int index,int n)
{
    if(index==n)
    {
        return;
    }
    printf("%d ",s[index].rollNo);
    displayRollno(s,index+1,n);
}

int main() 
{ 
    int n;
    
    scanf("%d",&n);
    
    struct student *s = (struct student*) malloc(n * sizeof(struct student));
    if (s == NULL) {
        printf("Memory Allocation Failed!\n");
        return 1;
    }
    
    for(int index=0; index < n; index++)
    { 
        scanf("%d %s %d %d %d",&s[index].rollNo,s[index].name,&s[index].mark1,&s[index].mark2,&s[index].mark3);
    } 
    
    printf("\n-- STUDENT ROLL NUMBERS --\n");
    
    void(*rollnoFptr)(struct student[],int,int)=displayRollno;
    
    rollnoFptr(s,0,n);
    
    char (*gradeFptr)(float) = gradeCalc;
    
    void (*starFptr)(char) = printStars;
    
    for(int index = 0; index < n; index++) 
    {
        progressCalculator(s[index], gradeFptr, starFptr);
    } 
    
}





