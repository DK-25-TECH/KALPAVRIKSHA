#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int rollNumber;
    char studentName[25];
    int subjectMarks[3];
} Student;

char calculateGrade(const float averageMarks) {
    char grade = 'F';
    if (averageMarks >= 85) {
        grade = 'A';
    } else if (averageMarks >= 70) {
        grade = 'B';
    } else if (averageMarks >= 50) {
        grade = 'C';
    } else if (averageMarks >= 35) {
        grade = 'D';
    }
    return grade;
}

void displayPerformance(const char grade) {
    switch (grade) {
        case 'A': printf("PERFORMANCE : *****\n"); break;
        case 'B': printf("PERFORMANCE : ****\n"); break;
        case 'C': printf("PERFORMANCE : ***\n"); break;
        case 'D': printf("PERFORMANCE : **\n"); break;
        default:  printf("PERFORMANCE : FAIL\n"); break;
    }
}

void displayStudentProgress(const Student studentRecord) {
    int totalMarks = 0;

    for (int subjectIndex = 0; subjectIndex < 3; subjectIndex++) {
        totalMarks += studentRecord.subjectMarks[subjectIndex];
    }

    float averageMarks = totalMarks / 3.0f;
    char grade = calculateGrade(averageMarks);

    printf("\nROLL NUMBER   : %d\n", studentRecord.rollNumber);
    printf("NAME          : %s\n", studentRecord.studentName);
    printf("TOTAL MARKS   : %d\n", totalMarks);
    printf("AVERAGE MARKS : %.2f\n", averageMarks);
    printf("GRADE         : %c\n", grade);

    displayPerformance(grade);
}

void readStudentDetails(Student *studentList, const int totalStudents) {
    for (int studentIndex = 0; studentIndex < totalStudents; studentIndex++) {
        printf("Enter Roll Number, Name, and 3 Subject Marks for Student %d: ", studentIndex + 1);
        scanf("%d %24s %d %d %d",
              &studentList[studentIndex].rollNumber,
              studentList[studentIndex].studentName,
              &studentList[studentIndex].subjectMarks[0],
              &studentList[studentIndex].subjectMarks[1],
              &studentList[studentIndex].subjectMarks[2]);
    }
}

void printAllRollNumbers(const Student studentList[], const int currentStudentIndex, const int totalStudents) {
    if (currentStudentIndex >= totalStudents) {
        return;
    }
    printf("%d ", studentList[currentStudentIndex].rollNumber);
    printAllRollNumbers(studentList, currentStudentIndex + 1, totalStudents);
}

int main() {
    int totalStudents;
    printf("Enter the number of students: ");
    scanf("%d", &totalStudents);

    Student *studentList = malloc(totalStudents * sizeof(Student));
    if (studentList == NULL) {
        printf("Memory Allocation Failed!\n");
        return 1;
    }

    readStudentDetails(studentList, totalStudents);

    printf("\n-- STUDENT ROLL NUMBERS --\n");
    printAllRollNumbers(studentList, 0, totalStudents);
    printf("\n");

    for (int studentIndex = 0; studentIndex < totalStudents; studentIndex++) {
        displayStudentProgress(studentList[studentIndex]);
    }

    free(studentList);
    return 0;
}
