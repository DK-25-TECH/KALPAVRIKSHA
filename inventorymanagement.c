#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MIN_PRODUCTS 1
#define MAX_PRODUCTS 100
#define MIN_ID 1
#define MAX_ID 10000
#define MAX_NAME_LEN 50
#define MIN_PRICE 1
#define MAX_PRICE 100000
#define MIN_QTY 1
#define MAX_QTY 1000000

typedef struct
{
    int id;
    char name[MAX_NAME_LEN];
    float price;
    int quantity;
} Product;

typedef enum
{
    ADD_PRODUCT = 1,
    VIEW_PRODUCTS,
    UPDATE_QUANTITY,
    SEARCH_BY_ID,
    SEARCH_BY_NAME,
    SEARCH_BY_PRICE,
    DELETE_PRODUCT,
    EXIT_PROGRAM
} MenuChoice;

void addProduct(Product **list, int *count, int *capacity)
{
    if (*count == *capacity)
    {
        (*capacity)++;
        *list = realloc(*list, (*capacity) * sizeof(Product));
        if (*list == NULL)
        {
            printf("Memory allocation failed.\n");
            return;
        }
    }

    Product *p = &(*list)[*count];

    printf("\nEnter product ID: ");
    scanf("%d", &p->id);
    if (p->id < MIN_ID || p->id > MAX_ID)
    {
        printf("Invalid ID range.\n");
        return;
    }

    printf("Enter name: ");
    scanf(" %49s", p->name);

    printf("Enter price: ");
    scanf("%f", &p->price);
    if (p->price < MIN_PRICE || p->price > MAX_PRICE)
    {
        printf("Invalid price range.\n");
        return;
    }

    printf("Enter quantity: ");
    scanf("%d", &p->quantity);
    if (p->quantity < MIN_QTY || p->quantity > MAX_QTY)
    {
        printf("Invalid quantity.\n");
        return;
    }

    (*count)++;
    printf("Product added.\n");
}

void viewProducts(Product *list, int count)
{
    if (count == 0)
    {
        printf("No products.\n");
        return;
    }

    for (int i = 0; i < count; i++)
    {
        printf("%d | %s | %.2f | %d\n",
               list[i].id, list[i].name, list[i].price, list[i].quantity);
    }
}

void updateQuantity(Product *list, int count)
{
    int id, qty;
    printf("Enter product ID: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++)
    {
        if (list[i].id == id)
        {
            printf("Enter new quantity: ");
            scanf("%d", &qty);
            list[i].quantity = qty;
            printf("Updated.\n");
            return;
        }
    }

    printf("Product not found.\n");
}

void searchById(Product *list, int count)
{
    int id;
    printf("Enter ID: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++)
    {
        if (list[i].id == id)
        {
            printf("%d | %s | %.2f | %d\n",
                   list[i].id, list[i].name, list[i].price, list[i].quantity);
            return;
        }
    }

    printf("No match.\n");
}

void searchByName(Product *list, int count)
{
    char name[50];
    printf("Enter name: ");
    scanf(" %49s", name);
    bool found = false;

    for (int i = 0; i < count; i++)
    {
        if (strstr(list[i].name, name))
        {
            printf("%d | %s | %.2f | %d\n",
                   list[i].id, list[i].name, list[i].price, list[i].quantity);
            found = true;
        }
    }

    if (!found)
        printf("No match.\n");
}

void searchByPrice(Product *list, int count)
{
    float min, max;
    printf("Enter min price: ");
    scanf("%f", &min);
    printf("Enter max price: ");
    scanf("%f", &max);

    bool found = false;
    for (int i = 0; i < count; i++)
    {
        if (list[i].price >= min && list[i].price <= max)
        {
            printf("%d | %s | %.2f | %d\n",
                   list[i].id, list[i].name, list[i].price, list[i].quantity);
            found = true;
        }
    }

    if (!found)
        printf("No products in range.\n");
}

void deleteProduct(Product **list, int *count)
{
    int id;
    printf("Enter ID to delete: ");
    scanf("%d", &id);

    int index = -1;
    for (int i = 0; i < *count; i++)
    {
        if ((*list)[i].id == id)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        printf("Not found.\n");
        return;
    }

    for (int i = index; i < *count - 1; i++)
    {
        (*list)[i] = (*list)[i + 1];
    }

    (*count)--;
    *list = realloc(*list, (*count) * sizeof(Product));

    printf("Deleted.\n");
}

int main()
{
    int n;

    do
    {
        printf("Enter number of products (1–100): ");
        scanf("%d", &n);
    }
    while (n < MIN_PRODUCTS || n > MAX_PRODUCTS);

    Product *list = calloc(n, sizeof(Product));
    if (!list)
    {
        printf("Memory error.\n");
        return 1;
    }

    for (int i = 0; i < n; i++)
    {
        printf("\nProduct %d\n", i + 1);
        printf("ID: ");
        scanf("%d", &list[i].id);
        printf("Name: ");
        scanf(" %49s", list[i].name);
        printf("Price: ");
        scanf("%f", &list[i].price);
        printf("Quantity: ");
        scanf("%d", &list[i].quantity);
    }

    int count = n, capacity = n;
    bool run = true;

    while (run)
    {
        printf("\n--- INVENTORY MENU ---\n");
        printf("1. Add Product\n");
        printf("2. View Products\n");
        printf("3. Update Quantity\n");
        printf("4. Search by ID\n");
        printf("5. Search by Name\n");
        printf("6. Search by Price\n");
        printf("7. Delete Product\n");
        printf("8. Exit\n");
        printf("Choice: ");

        int ch;
        scanf("%d", &ch);

        switch (ch)
        {
            case ADD_PRODUCT:
                addProduct(&list, &count, &capacity);
                break;
            case VIEW_PRODUCTS:
                viewProducts(list, count);
                break;
            case UPDATE_QUANTITY:
                updateQuantity(list, count);
                break;
            case SEARCH_BY_ID:
                searchById(list, count);
                break;
            case SEARCH_BY_NAME:
                searchByName(list, count);
                break;
            case SEARCH_BY_PRICE:
                searchByPrice(list, count);
                break;
            case DELETE_PRODUCT:
                deleteProduct(&list, &count);
                break;
            case EXIT_PROGRAM:
                run = false;
                free(list);
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}
