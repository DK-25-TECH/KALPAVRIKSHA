#include <stdio.h>
#include <stdlib.h>

struct student
{ 
    int rollNo;
    char name[25];
    int mark1, mark2, mark3;  
};

char gradeCalc(float avg)
{ 
    char grade;

    if (avg >= 85)
        grade = 'A';
    else if (avg >= 70)
        grade = 'B';
    else if (avg >= 50)
        grade = 'C';
    else if (avg >= 35)
        grade = 'D';
    else
        grade = 'F';
    
    return grade;
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

void progressCalculator(struct student s, char (*gradeFptr)(float), void (*starFptr)(char)) 
{ 
    int total = s.mark1 + s.mark2 + s.mark3;
    float avg = total / 3.0;
    char grade = gradeFptr(avg);
    
    printf("\nROLL_NO : %d \nNAME : %s\nTOTAL-MARKS : %d \nAVERAGE-MARKS : %.2f\nGRADE : %c\n",
           s.rollNo, s.name, total, avg, grade);
    
    starFptr(grade);
}

void inputStudents(struct student *s, int n)
{
    for (int index = 0; index < n; index++)
    { 
        scanf("%d %s %d %d %d",
              &s[index].rollNo,
              s[index].name,
              &s[index].mark1,
              &s[index].mark2,
              &s[index].mark3);
    }
}

void printRollNumbers(struct student s[], int index, int n)
{
    if (index == n)
        return;

    printf("%d ", s[index].rollNo);
    printRollNumbers(s, index + 1, n);
}

int main() 
{ 
    int n;
    scanf("%d", &n);
    
    struct student *s = (struct student *) malloc(n * sizeof(struct student));
    if (s == NULL) {
        printf("Memory Allocation Failed!\n");
        return 1;
    }
    
    inputStudents(s, n);
    
    printf("\n-- STUDENT ROLL NUMBERS --\n");
    printRollNumbers(s, 0, n);
    printf("\n");
    
    char (*gradeFptr)(float) = gradeCalc;
    void (*starFptr)(char) = printStars;
    
    for (int index = 0; index < n; index++) 
    {
        progressCalculator(s[index], gradeFptr, starFptr);
    }
    
    free(s);
    return 0;
}
