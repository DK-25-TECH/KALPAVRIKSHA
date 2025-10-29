#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int rollNumber;
    char studentName[25];
    int subjectMarks[3];
} Student;

char calculateGrade(float averageMarks)
{
    char grade = 'F';

    if (averageMarks >= 85)
    {
        grade = 'A';
    }
    else if (averageMarks >= 70)
    {
        grade = 'B';
    }
    else if (averageMarks >= 50)
    {
        grade = 'C';
    }
    else if (averageMarks >= 35)
    {
        grade = 'D';
    }

    return grade;
}

void displayPerformance(char grade)
{
    switch (grade)
    {
        case 'A':
            printf("Performance : Excellent *****\n");
            break;
    
        case 'B':
            printf("Performance : Very Good ****\n");
            break;
        
        case 'C':
            printf("Performance : Good ***\n");
            break;
        
        case 'D':
            printf("Performance : Average **\n");
            break;
        
        default:
            printf("Performance : Fail\n");
            break;
    }
}

void displayStudentProgress(Student student)
{
    int totalMarks = 0;

    for (int subjectIndex = 0; subjectIndex < 3; subjectIndex++)
    {
        totalMarks += student.subjectMarks[subjectIndex];
    }

    float averageMarks = totalMarks / 3.0f;
    char grade = calculateGrade(averageMarks);

    printf("\n---------------------------------------\n");
    printf("Roll Number : %d\n", student.rollNumber);
    printf("Name        : %s\n", student.studentName);
    printf("Total Marks : %d\n", totalMarks);
    printf("Average     : %.2f\n", averageMarks);
    printf("Grade       : %c\n", grade);

    displayPerformance(grade);
}

void readStudentDetails(Student *students, int totalStudents)
{
    for (int studentIndex = 0; studentIndex < totalStudents; studentIndex++)
    {
        printf("\nEnter Roll Number, Name, and 3 Marks for Student %d:\n", studentIndex + 1);
        scanf("%d %24s %d %d %d",
              &students[studentIndex].rollNumber,
              students[studentIndex].studentName,
              &students[studentIndex].subjectMarks[0],
              &students[studentIndex].subjectMarks[1],
              &students[studentIndex].subjectMarks[2]);
    }
}

void printAllRollNumbers(Student students[], int index, int totalStudents)
{
    if (index >= totalStudents)
    {
        return;
    }

    printf("%d ", students[index].rollNumber);
    printAllRollNumbers(students, index + 1, totalStudents);
}

int main()
{
    int totalStudents;

    printf("Enter the number of students: ");
    scanf("%d", &totalStudents);

    Student *students = malloc(totalStudents * sizeof(Student));
    if (students == NULL)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }

    readStudentDetails(students, totalStudents);

    printf("\nList of Roll Numbers: ");
    printAllRollNumbers(students, 0, totalStudents);
    printf("\n");

    for (int studentIndex = 0; studentIndex < totalStudents; studentIndex++)
    {
        displayStudentProgress(students[studentIndex]);
    }

    free(students);
    printf("\nAll student records processed successfully.\n");

    return 0;
}
