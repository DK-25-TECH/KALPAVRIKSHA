#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MIN_PRODUCTS 1
#define MAX_PRODUCTS 100
#define MIN_ID 1
#define MAX_ID 10000
#define MAX_NAME_LENGTH 50
#define MIN_PRICE 1
#define MAX_PRICE 100000
#define MIN_QUANTITY 1
#define MAX_QUANTITY 1000000

typedef struct
{
    int id;
    char name[MAX_NAME_LENGTH];
    float price;
    int quality;
} Product;

typedef enum
{
    ADD = 1, VIEW, UPDATE, FIND_BY_ID, FIND_BY_NAME, FIND_BY_PRICE, REMOVE, EXIT
} MenuOption;

void addProduct(Product **productList, int *productCount, int *listCapacity)
{
    if (*productCount == *listCapacity)
    {
        *listCapacity += 1;
        *productList = realloc(*productList, (*listCapacity) * sizeof(Product));
        if (!*productList)
        {
            printf("Memory allocation failed.\n");
            return;
        }
    }

    Product *newProduct = &(*productList)[*productCount];

    printf("Enter ID: ");
    scanf("%d", &newProduct->id);
    printf("Enter Name: ");
    scanf(" %49s", newProduct->name);
    printf("Enter Price: ");
    scanf("%f", &newProduct->price);
    printf("Enter Quantity: ");
    scanf("%d", &newProduct->quality);

    (*productCount)++;
    printf("Product added.\n");
}

void viewProducts(Product *productList, int productCount)
{
    if (productCount == 0)
    {
        printf("No products available.\n");
        return;
    }

    for (int index = 0; index < productCount; index++)
    {
        printf("ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
               productList[index].id, productList[index].name,
               productList[index].price, productList[index].quality);
    }
}

void updateQuantity(Product *productList, int productCount)
{
    int targetId;
    bool found = false;

    printf("Enter ID to update: ");
    scanf("%d", &targetId);

    for (int index = 0; index < productCount; index++)
    {
        if (productList[index].id == targetId)
        {
            printf("Enter new Quantity: ");
            scanf("%d", &productList[index].quality);
            printf("Quantity updated.\n");
            found = true;
            break;
        }
    }

    if (!found)
    {
        printf("Product not found.\n");
    }
}

void searchById(Product *productList, int productCount)
{
    int targetId;
    printf("Enter ID: ");
    scanf("%d", &targetId);

    for (int index = 0; index < productCount; index++)
    {
        if (productList[index].id == targetId)
        {
            printf("ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   productList[index].id, productList[index].name,
                   productList[index].price, productList[index].quality);
            return;
        }
    }

    printf("Product not found.\n");
}

void searchByName(Product *productList, int productCount)
{
    char searchName[MAX_NAME_LENGTH];
    bool found = false;

    printf("Enter Name: ");
    scanf(" %[^\n]", searchName);

    for (int index = 0; index < productCount; index++)
    {
        if (strstr(productList[index].name, searchName))
        {
            printf("ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   productList[index].id, productList[index].name,
                   productList[index].price, productList[index].quality);
            found = true;
        }
    }

    if (!found)
    {
        printf("No matching products.\n");
    }
}

void searchByPrice(Product *productList, int productCount)
{
    float minPrice, maxPrice;
    bool found = false;

    printf("Enter Min Price: ");
    scanf("%f", &minPrice);
    printf("Enter Max Price: ");
    scanf("%f", &maxPrice);

    for (int index = 0; index < productCount; index++)
    {
        if (productList[index].price >= minPrice && productList[index].price <= maxPrice)
        {
            printf("ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   productList[index].id, productList[index].name,
                   productList[index].price, productList[index].quality);
            found = true;
        }
    }

    if (!found)
    {
        printf("No products in this price range.\n");
    }
}

void deleteProduct(Product **productList, int *productCount)
{
    int targetId, deleteIndex = -1;
    printf("Enter ID to delete: ");
    scanf("%d", &targetId);

    for (int index = 0; index < *productCount; index++)
    {
        if ((*productList)[index].id == targetId)
        {
            deleteIndex = index;
            break;
        }
    }

    if (deleteIndex == -1)
    {
        printf("Product not found.\n");
        return;
    }

    for (int index = deleteIndex; index < *productCount - 1; index++)
    {
        (*productList)[index] = (*productList)[index + 1];
    }

    (*productCount)--;
    *productList = realloc(*productList, (*productCount) * sizeof(Product));
    printf("Product deleted.\n");
}

void showMenu()
{
    printf("\n--- INVENTORY MENU ---\n");
    printf("1. Add Product\n");
    printf("2. View Products\n");
    printf("3. Update Quantity\n");
    printf("4. Search by ID\n");
    printf("5. Search by Name\n");
    printf("6. Search by Price Range\n");
    printf("7. Delete Product\n");
    printf("8. Exit\n");
}

void initializeInventory(Product **productList, int *productCount, int *listCapacity)
{
    int totalProducts;

    do
    {
        printf("Enter number of products (1–100): ");
        scanf("%d", &totalProducts);
    }
    while (totalProducts < MIN_PRODUCTS || totalProducts > MAX_PRODUCTS);

    *productList = calloc(totalProducts, sizeof(Product));
    *productCount = totalProducts;
    *listCapacity = totalProducts;

    for (int index = 0; index < totalProducts; index++)
    {
        printf("Product %d details:\n", index + 1);
        printf("ID: ");
        scanf("%d", &(*productList)[index].id);
        printf("Name: ");
        scanf(" %49s", (*productList)[index].name);
        printf("Price: ");
        scanf("%f", &(*productList)[index].price);
        printf("Quantity: ");
        scanf("%d", &(*productList)[index].quality);
    }
}

void handleUserChoice(Product **productList, int *productCount, int *listCapacity)
{
    bool continueRunning = true;

    while (continueRunning)
    {
        showMenu();
        int userChoice;

        printf("Enter your choice: ");
        scanf("%d", &userChoice);

        switch ((MenuOption)userChoice)
        {
            case ADD:
                addProduct(productList, productCount, listCapacity);
                break;
            case VIEW:
                viewProducts(*productList, *productCount);
                break;
            case UPDATE:
                updateQuantity(*productList, *productCount);
                break;
            case FIND_BY_ID:
                searchById(*productList, *productCount);
                break;
            case FIND_BY_NAME:
                searchByName(*productList, *productCount);
                break;
            case FIND_BY_PRICE:
                searchByPrice(*productList, *productCount);
                break;
            case REMOVE:
                deleteProduct(productList, productCount);
                break;
            case EXIT:
                continueRunning = false;
                free(*productList);
                printf("Exiting inventory system.\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}

int main()
{
    Product *productList = NULL;
    int productCount = 0;
    int listCapacity = 0;

    initializeInventory(&productList, &productCount, &listCapacity);
    handleUserChoice(&productList, &productCount, &listCapacity);

    return 0;
}
