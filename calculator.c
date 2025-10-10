#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define STACK_SIZE 100

int numberStack[STACK_SIZE];
int numberTop = -1;

char operatorStack[STACK_SIZE];
int operatorTop = -1;

void pushNumber(int value) {
    if (numberTop < STACK_SIZE - 1) {
        numberStack[++numberTop] = value;
    } else {
        printf("Number Stack Overflow\n");
    }
}

int popNumber(int *error) {
    int result = 0;
    if (numberTop < 0) {
        printf("Number Stack Underflow\n");
        *error = 1;
    } else {
        result = numberStack[numberTop--];
    }
    return result;
}

void pushOperator(char operatorChar) {
    if (operatorTop < STACK_SIZE - 1) {
        operatorStack[++operatorTop] = operatorChar;
    } else {
        printf("Operator Stack Overflow\n");
    }
}

char popOperator(int *error) {
    char result = '\0';
    if (operatorTop < 0) {
        printf("Operator Stack Underflow\n");
        *error = 1;
    } else {
        result = operatorStack[operatorTop--];
    }
    return result;
}

int priority(char operatorChar) {
    int priorityValue = 0;
    switch (operatorChar) {
        case '*':
        case '/':
            priorityValue = 2;
            break;
        case '+':
        case '-':
            priorityValue = 1;
            break;
        default:
            priorityValue = 0;
            break;
    }
    return priorityValue;
}

int calculate(int firstValue, int secondValue, char operatorChar, int *error) {
    int result = 0;
    switch (operatorChar) {
        case '*':
            result = firstValue * secondValue;
            break;
        case '+':
            result = firstValue + secondValue;
            break;
        case '-':
            result = firstValue - secondValue;
            break;
        case '/':
            if (secondValue == 0) {
                printf("Division by zero not allowed\n");
                *error = 1;
            } else {
                result = firstValue / secondValue;
            }
            break;
        default:
            printf("Invalid Operator\n");
            *error = 1;
            break;
    }
    return result;
}

int evaluateExpression(char *expression) {
    int error = 0;
    int index = 0;
    int finalResult = 0;

    while (expression[index] && !error) {
        if (isspace(expression[index])) {
            index++;
            continue;
        }

        if (isdigit(expression[index])) {
            int currentNumber = 0;
            while (isdigit(expression[index])) {
                currentNumber = currentNumber * 10 + (expression[index] - '0');
                index++;
            }
            pushNumber(currentNumber);
            continue;
        }

        if (strchr("*/+-", expression[index])) {
            while (operatorTop != -1 && priority(operatorStack[operatorTop]) >= priority(expression[index]) && !error) {
                int secondValue = popNumber(&error);
                int firstValue = popNumber(&error);
                char currentOperator = popOperator(&error);
                int tempResult = calculate(firstValue, secondValue, currentOperator, &error);
                pushNumber(tempResult);
            }
            pushOperator(expression[index]);
            index++;
            continue;
        }

        printf("Invalid Expression Character: %c\n", expression[index]);
        error = 1;
    }

    while (operatorTop != -1 && !error) {
        int secondValue = popNumber(&error);
        int firstValue = popNumber(&error);
        char currentOperator = popOperator(&error);
        int tempResult = calculate(firstValue, secondValue, currentOperator, &error);
        pushNumber(tempResult);
    }

    if (!error && numberTop >= 0) {
        finalResult = numberStack[numberTop];
    }

    return error ? 0 : finalResult;
}

int main() {
    char expression[100];
    printf("Enter Expression: ");
    fgets(expression, sizeof(expression), stdin);
    expression[strcspn(expression, "\n")] = '\0';

    int result = evaluateExpression(expression);
    printf("Result: %d\n", result);

    return 0;
}