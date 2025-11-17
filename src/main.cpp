#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Product {
public:
    int id;
    string name;
    int quantity;
    double price;

    Product(int _id, string _name, int _quantity, double _price) {
        id = _id;
        name = _name;
        quantity = _quantity;
        price = _price;
    }
};

vector<Product> inventory;

void addProduct() {
    int id, quantity;
    string name;
    double price;

    cout << "Enter ID: ";
    cin >> id;

    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, name);

    cout << "Enter Quantity: ";
    cin >> quantity;

    cout << "Enter Price: ";
    cin >> price;

    Product p(id, name, quantity, price);
    inventory.push_back(p);

    cout << "Product Added Successfully!\n";
}

int main() {
    int choice;

    while (true) {
        cout << "\n=== Inventory Management System ===\n";
        cout << "1. Add Product\n";
        cout << "2. View Products\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1)
            addProduct();
        else if (choice == 0)
            break;
        else
            cout << "Invalid choice!\n";
    }

    return 0;
}
