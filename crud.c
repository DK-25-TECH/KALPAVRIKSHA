#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define userFile "users.txt"
#define tempFile "temp.txt"

typedef struct user {
    int id;
    char name[100];
    int age;
} user;

void createUser();
void deleteUser();
void updateUser();
void displayUser();

void createUser() {
    FILE *filePtr = fopen(userFile, "a");
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
    FILE *filePtr = fopen(userFile, "r");
    if (filePtr == NULL) {
        printf("UNABLE TO OPEN THE FILE");
        return;
    }

    user user;
    while (fscanf(filePtr, "%d %s %d", &user.id, user.name, &user.age) == 3) {
        printf("%d %s %d\n", user.id, user.name, user.age);
    }
    fclose(filePtr);
}

void updateUser() {
    FILE *filePtr = fopen(userFile, "r");
    if (filePtr == NULL) {
        printf("UNABLE TO OPEN THE FILE");
        return;
    }

    int id, found = 0;
    printf("ENTER USER ID : ");
    scanf("%d", &id);
    user user;

    FILE *tempPtr = fopen(tempFile, "w");
    if (tempPtr == NULL) {
        printf("UNABLE TO CREATE FILE");
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
    remove(userFile);
    rename(tempFile, userFile);

    if (found) {
        printf("UPDATED SUCCESSFULLY\n");
    } else {
        printf("ID NOT FOUND\n");
    }
}

void deleteUser() {
    FILE *filePtr = fopen(userFile, "r");
    if (filePtr == NULL) {
        printf("UNABLE TO OPEN FILE");
        return;
    }

    int id, found = 0;
    printf("ENTER ID TO DELETE : ");
    scanf("%d", &id);

    user user;
    FILE *tempPtr = fopen(tempFile, "w");
    if (tempPtr == NULL) {
        printf("UNABLE TO CREATE FILE");
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
    remove(userFile);
    rename(tempFile, userFile);

    if (found) {
        printf("DELETED SUCCESSFULLY\n");
    } else {
        printf("USER NOT FOUND\n");
    }
}

int main() {
    int choice;
    FILE *filePtr;

    filePtr = fopen(userFile, "a");
    if (filePtr == NULL) {
        printf("ERROR NOT FOUND");
        return 1;
    }
    fclose(filePtr);

    while (1) {
        printf("USER MANAGEMENT SYSTEM \n");
        printf("1. ADD USER \n");
        printf("2. DISPLAY USERS \n");
        printf("3. UPDATE BY ID \n");
        printf("4. DELETE BY ID \n");
        printf("5. EXIT \n");
        printf("\n");
        printf("ENTER YOUR CHOICE : \n");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createUser(); break;
            case 2: displayUser(); break;
            case 3: updateUser(); break;
            case 4: deleteUser(); break;
            case 5: exit(0);
            default: printf("INVALID CHOICE\n");
        }
    }
    return 0;
}
