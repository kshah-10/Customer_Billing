#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>

using namespace std;

#define REGULAR_CUSTOMER_DISCOUNT 0.1  

struct Customer {
    char name[50];
    int id;
    float balance;
    int isRegularCustomer;
};

struct Item {
    char name[50];
    float price;
    int quantity;
};

vector<pair<string, int> > itemfreq;

int findCustomer(struct Customer *customers, int count, int id) {
    for (int i = 0; i < count; i++) {
        if (customers[i].id == id) {
            return i;  
        }
    }
    return -1;  
}

void addCustomer(struct Customer **customers, int *count) {
    int id;
    printf("Enter customer ID: ");
    scanf("%d", &id);

    int index = findCustomer(*customers, *count, id);
    if (index != -1) {
        printf("Customer with ID %d already exists. Updating details.\n", id);
    } else {
        *customers = (struct Customer*)realloc(*customers, (*count + 1) * sizeof(struct Customer));
        index = *count;
        (*count)++;
    }

    printf("Enter customer name: ");
    scanf("%s", (*customers)[index].name);
    (*customers)[index].id = id;
    printf("Enter customer balance: ");
    scanf("%f", &(*customers)[index].balance);
    printf("Is the customer a regular customer? (1 for yes, 0 for no): ");
    scanf("%d", &(*customers)[index].isRegularCustomer);
}

void addItem(struct Item **items, int *count) {
    *items = (struct Item*)realloc(*items, (*count + 1) * sizeof(struct Item));
    printf("Enter item name: ");
    scanf("%s", (*items)[*count].name);
    printf("Enter item price: ");
    scanf("%f", &(*items)[*count].price);
    printf("Enter item quantity: ");
    scanf("%d", &(*items)[*count].quantity);

    int exist = 0;
    for(int i = 0; i < itemfreq.size(); i++) 
    {
        if(itemfreq[i].first == (*items)[*count].name) 
        {
            exist = 1;
            itemfreq[i].second++;
        }
    }

    if(exist == 0) 
    {
        itemfreq.push_back(make_pair((*items)[*count].name, 1));
    }

    (*count)++;
}

float calculateCartValue(struct Item *items, int count) {
    float total = 0.0;
    for (int i = 0; i < count; i++) {
        total += items[i].price * items[i].quantity;
    }
    return total;
}

float applyRegularCustomerDiscount(float cartValue, int isRegularCustomer) {
    if (isRegularCustomer) {
        return cartValue - (cartValue * REGULAR_CUSTOMER_DISCOUNT);
    } else {
        return cartValue;
    }
}

void splitBill(struct Customer *customers, int customerCount, float cartValue) {
    if (customerCount == 0) {
        printf("No customers to split the bill.\n");
        return;
    }

    float share = cartValue / customerCount;
    for (int i = 0; i < customerCount; i++) {
        printf("----------------------------------\n");
        printf("Customer: %s\n", customers[i].name);
        printf("ID: %d\n", customers[i].id);
        printf("----------------------------------\n");
        printf("Item\t\tPrice\tQuantity\n");
        printf("----------------------------------\n");
        for (int j = 0; j < customerCount; j++) {
            printf("%s\t\t%.2f\t%d\n", customers[j].name, customers[j].balance, customers[j].id);
        }
        printf("----------------------------------\n");
        if (customers[i].isRegularCustomer) {
            float discountAmount = share * REGULAR_CUSTOMER_DISCOUNT;
            printf("Regular customer discount applied: %.2f\n", discountAmount);
            printf("Amount to be paid (after discount): %.2f\n\n", share - discountAmount);
        } else {
            printf("Amount to be paid: %.2f\n\n", share);
        }
    }
}

void displayCustomers(struct Customer *customers, int count) {
    if (count == 0) {
        printf("No customers to display.\n");
    } else {
        printf("Customer details:\n");
        for (int i = 0; i < count; i++) {
            printf("Name: %s\n", customers[i].name);
            printf("ID: %d\n", customers[i].id);
            printf("Balance: %.2f\n", customers[i].balance);
            printf("Regular Customer: %s\n\n", customers[i].isRegularCustomer ? "Yes" : "No");
        }
    }
}

void displayItems(struct Item *items, int count) {
    if (count == 0) {
        printf("No items in the cart.\n");
    } else {
        printf("Cart items:\n");
        printf("Item\t\tPrice\tQuantity\n");
        printf("----------------------------------\n");
        for (int i = 0; i < count; i++) {
            printf("%s\t\t%.2f\t%d\n", items[i].name, items[i].price, items[i].quantity);
        }
        printf("----------------------------------\n");
    }
}


void heapify(int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && itemfreq[left].second > itemfreq[largest].second)
        largest = left;

    if (right < n && itemfreq[right].second > itemfreq[largest].second)
        largest = right;

    if (largest != i) {
        pair<string, int> temp = itemfreq[i];
        itemfreq[i] = itemfreq[largest];
        itemfreq[largest] = temp;

        heapify(n, largest);
    }
}

// Build the heap
void buildHeap() {
    int n = itemfreq.size();
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(n, i);
    }
}

void findTop5cousines()
{
    int k;
    cin >> k; 
    if (k <= 0) {
        return;
    }

    buildHeap();
    
    int n = itemfreq.size();
    for (int i = n - 1; i >= n - k; i--) {
        pair<string, int> temp = itemfreq[0];
        itemfreq[0] = itemfreq[i];
        itemfreq[i] = temp;

        heapify(i, 0);
    }

    printf("Our top %d products are: \n", k);
    for (int i = n - 1; i >= n - k; i--) {
        cout << itemfreq[i].first << endl;
    }
    printf("\n");

}

int main() {
    struct Customer *customers = NULL;
    struct Item *items = NULL;
    int customerCount = 0;
    int itemCount = 0;
    int choice;

    while (1) {
        printf("1. Add Customer\n");
        printf("2. Add Item to Cart\n");
        printf("3. Calculate Cart Value\n");
        printf("4. Split Bill\n");
        printf("5. Display Customers\n");
        printf("6. Display Items in Cart\n");
        printf("7. Exit\n");
        printf("8. Top items \n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addCustomer(&customers, &customerCount);
                break;
            case 2:
                addItem(&items, &itemCount);
                break;
            case 3: {
                float cartValue = calculateCartValue(items, itemCount);
                printf("Cart Value: %.2f\n", cartValue);
                break;
            }
            case 4: {
                float cartValue = calculateCartValue(items, itemCount);
                cartValue = applyRegularCustomerDiscount(cartValue, customers[0].isRegularCustomer);
                splitBill(customers, customerCount, cartValue);
                break;
            }
            case 5:
                displayCustomers(customers, customerCount);
                break;
            case 6:
                displayItems(items, itemCount);
                break;
            case 7:
                printf("Exiting program.\n");
                break;
            case 8:
                printf("Best sellers\n");
                findTop5cousines();
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}


