#include <stdio.h>
#include <stdlib.h>

typedef struct Student {
    int rollNo;
    char name[25];
    int mark1, mark2, mark3;
} Student;

char calculateGrade(float average) {
    char grade = 'F';
    if (average >= 85) grade = 'A';
    else if (average >= 70) grade = 'B';
    else if (average >= 50) grade = 'C';
    else if (average >= 35) grade = 'D';
    return grade;
}

void displayPerformance(char grade) {
    switch (grade) {
        case 'A': printf("PERFORMANCE : *****\n"); break;
        case 'B': printf("PERFORMANCE : ****\n"); break;
        case 'C': printf("PERFORMANCE : ***\n"); break;
        case 'D': printf("PERFORMANCE : **\n"); break;
        default:  printf("PERFORMANCE : FAIL\n"); break;
    }
}

void showProgress(Student s, char (*gradePtr)(float), void (*starPtr)(char)) {
    int total = s.mark1 + s.mark2 + s.mark3;
    float average = total / 3.0f;
    char grade = gradePtr(average);

    printf("\nROLL NO: %d\nNAME: %s\nTOTAL MARKS: %d\nAVERAGE MARKS: %.2f\nGRADE: %c\n",
           s.rollNo, s.name, total, average, grade);
    starPtr(grade);
}

void readStudents(Student *students, int n) {
    for (int i = 0; i < n; i++) {
        printf("Enter roll no, name, marks1, marks2, marks3 for student %d: ", i + 1);
        scanf("%d %24s %d %d %d",
              &students[i].rollNo,
              students[i].name,
              &students[i].mark1,
              &students[i].mark2,
              &students[i].mark3);
    }
}

void printRollNumbers(Student students[], int index, int n) {
    if (index >= n) return;
    printf("%d ", students[index].rollNo);
    printRollNumbers(students, index + 1, n);
}

int main() {
    int n;
    int exitCode = 0;
    printf("Enter number of students: ");
    scanf("%d", &n);

    Student *students = malloc(n * sizeof(Student));

    if (students) {
        readStudents(students, n);

        printf("\n-- STUDENT ROLL NUMBERS --\n");
        printRollNumbers(students, 0, n);
        printf("\n");

        char (*gradePtr)(float) = calculateGrade;
        void (*starPtr)(char) = displayPerformance;

        for (int i = 0; i < n; i++) {
            showProgress(students[i], gradePtr, starPtr);
        }

        free(students);
    } else {
        printf("Memory Allocation Failed!\n");
        exitCode = 1;
    }

    return exitCode;
}
