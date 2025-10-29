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

void showProgress(Student student, char (*gradePtr)(float), void (*performancePtr)(char)) {
    int totalMarks = student.mark1 + student.mark2 + student.mark3;
    float averageMarks = totalMarks / 3.0f;
    char grade = gradePtr(averageMarks);

    printf("\nROLL NO      : %d\n", student.rollNo);
    printf("NAME         : %s\n", student.name);
    printf("TOTAL MARKS  : %d\n", totalMarks);
    printf("AVERAGE MARKS: %.2f\n", averageMarks);
    printf("GRADE        : %c\n", grade);
    performancePtr(grade);
}

void readStudents(Student *students, int studentCount) {
    for (int studentIndex = 0; studentIndex < studentCount; studentIndex++) {
        printf("Enter roll no, name, mark1, mark2, mark3 for student %d: ",
               studentIndex + 1);
        scanf("%d %24s %d %d %d",
              &students[studentIndex].rollNo,
              students[studentIndex].name,
              &students[studentIndex].mark1,
              &students[studentIndex].mark2,
              &students[studentIndex].mark3);
    }
}

void printRollNumbers(Student students[], int currentIndex, int studentCount) {
    if (currentIndex >= studentCount)
        return;
    printf("%d ", students[currentIndex].rollNo);
    printRollNumbers(students, currentIndex + 1, studentCount);
}

int main() {
    int studentCount;
    int exitCode = 0;

    printf("Enter number of students: ");
    scanf("%d", &studentCount);

    Student *students = malloc(studentCount * sizeof(Student));

    if (students) {
        readStudents(students, studentCount);

        printf("\n-- STUDENT ROLL NUMBERS --\n");
        printRollNumbers(students, 0, studentCount);
        printf("\n");

        char (*gradePtr)(float) = calculateGrade;
        void (*performancePtr)(char) = displayPerformance;

        for (int studentIndex = 0; studentIndex < studentCount; studentIndex++) {
            showProgress(students[studentIndex], gradePtr, performancePtr);
        }

        free(students);
    } else {
        printf("Memory Allocation Failed!\n");
        exitCode = 1;
    }

    return exitCode;
}
