#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define USER_FILE "users.txt"
#define TEMP_FILE "temp.txt"

typedef struct user {
    int id;
    char name[100];
    int age;
} user;

typedef enum {
    CREATE = 1,
    DISPLAY,
    UPDATE,
    DELETE,
    EXIT_PROGRAM
} Operation;

void createUser();
void deleteUser();
void updateUser();
void displayUser();

void createUser() {
    FILE *filePtr = fopen(USER_FILE, "a");
    if (filePtr == NULL) {
        printf("UNABLE TO OPEN !!\n");
        return;
    }

    user user;
    printf("ENTER USER ID : ");
    scanf("%d", &user.id);
    printf("ENTER USER-NAME : ");
    scanf("%s", user.name);
    printf("ENTER USER AGE : ");
    scanf("%d", &user.age);

    fprintf(filePtr, "%d %s %d\n", user.id, user.name, user.age);
    fclose(filePtr);

    printf("USER ADDED TO THE FILE SUCCESSFULLY\n");
}

void displayUser() {
    FILE *filePtr = fopen(USER_FILE, "r");
    if (filePtr == NULL) {
        printf("UNABLE TO OPEN THE FILE\n");
        return;
    }

    user user;
    while (fscanf(filePtr, "%d %s %d", &user.id, user.name, &user.age) == 3) {
        printf("%d %s %d\n", user.id, user.name, user.age);
    }
    fclose(filePtr);
}

void updateUser() {
    FILE *filePtr = fopen(USER_FILE, "r");
    if (filePtr == NULL) {
        printf("UNABLE TO OPEN THE FILE\n");
        return;
    }

    int id, found = 0;
    printf("ENTER USER ID : ");
    scanf("%d", &id);
    user user;

    FILE *tempPtr = fopen(TEMP_FILE, "w");
    if (tempPtr == NULL) {
        printf("UNABLE TO CREATE FILE\n");
        fclose(filePtr);
        return;
    }

    while (fscanf(filePtr, "%d %s %d", &user.id, user.name, &user.age) == 3) {
        if (user.id == id) {
            found = 1;
            printf("ENTER NEW USER-NAME : ");
            scanf("%s", user.name);
            printf("ENTER NEW USER AGE : ");
            scanf("%d", &user.age);
        }
        fprintf(tempPtr, "%d %s %d\n", user.id, user.name, user.age);
    }
    fclose(filePtr);
    fclose(tempPtr);
    remove(USER_FILE);
    rename(TEMP_FILE, USER_FILE);

    if (found) {
        printf("UPDATED SUCCESSFULLY\n");
    } else {
        printf("ID NOT FOUND\n");
    }
}

void deleteUser() {
    FILE *filePtr = fopen(USER_FILE, "r");
    if (filePtr == NULL) {
        printf("UNABLE TO OPEN FILE\n");
        return;
    }

    int id, found = 0;
    printf("ENTER ID TO DELETE : ");
    scanf("%d", &id);

    user user;
    FILE *tempPtr = fopen(TEMP_FILE, "w");
    if (tempPtr == NULL) {
        printf("UNABLE TO CREATE FILE\n");
        fclose(filePtr);
        return;
    }

    while (fscanf(filePtr, "%d %s %d", &user.id, user.name, &user.age) == 3) {
        if (user.id == id) {
            found = 1;
            continue;
        }
        fprintf(tempPtr, "%d %s %d\n", user.id, user.name, user.age);
    }
    fclose(filePtr);
    fclose(tempPtr);
    remove(USER_FILE);
    rename(TEMP_FILE, USER_FILE);

    if (found) {
        printf("DELETED SUCCESSFULLY\n");
    } else {
        printf("USER NOT FOUND\n");
    }
}

int main() {
    Operation choice;

    FILE *filePtr = fopen(USER_FILE, "a");
    if (filePtr == NULL) {
        printf("ERROR NOT FOUND\n");
        return 1;
    }
    fclose(filePtr);

    while (1) {
        printf("USER MANAGEMENT SYSTEM \n");
        printf("%d. ADD USER \n", CREATE);
        printf("%d. DISPLAY USERS \n", DISPLAY);
        printf("%d. UPDATE BY ID \n", UPDATE);
        printf("%d. DELETE BY ID \n", DELETE);
        printf("%d. EXIT \n", EXIT_PROGRAM);
        printf("\nENTER YOUR CHOICE : \n");
        scanf("%d", &choice);

        switch (choice) {
            case CREATE: createUser(); break;
            case DISPLAY: displayUser(); break;
            case UPDATE: updateUser(); break;
            case DELETE: deleteUser(); break;
            case EXIT_PROGRAM: exit(0);
            default: printf("INVALID CHOICE\n");
        }
    }
    return 0;
}
