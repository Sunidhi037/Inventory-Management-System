#include <iostream>
#include <string>
#include <vector>
#include <limits>
using namespace std;

// ====================== Product Class ======================
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

// ====================== Inventory Storage ======================
vector<Product> inventory;   // global vector

// ====================== Helpers ======================
int nextId() {
    int mx = 0;
    for (const auto &p : inventory) if (p.id > mx) mx = p.id;
    return mx + 1;
}

void clearCin() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ====================== Add Product Function ======================
void addProduct() {
    string name;
    int quantity = 0;
    double price = 0.0;

    cout << "\n--- Add New Product ---\n";
    cout << "Enter product name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear leftover newline
    getline(cin, name);
    if (name.empty()) {
        cout << "Product name cannot be empty. Aborting add.\n";
        return;
    }

    cout << "Enter quantity (integer >= 0): ";
    if (!(cin >> quantity) || quantity < 0) {
        cout << "Invalid quantity. Aborting add.\n";
        clearCin();
        return;
    }

    cout << "Enter price (e.g. 99.99, >= 0): ";
    if (!(cin >> price) || price < 0.0) {
        cout << "Invalid price. Aborting add.\n";
        clearCin();
        return;
    }

    int id = nextId();
    inventory.emplace_back(id, name, quantity, price);
    cout << "Product added successfully with ID " << id << ".\n";
}

// ====================== View Products Function ======================
void viewProducts() {
    if (inventory.empty()) {
        cout << "No products available.\n";
        return;
    }

    cout << "\nID\tName\tQuantity\tPrice\n";
    for (const auto &p : inventory) {
        cout << p.id << "\t" << p.name << "\t" << p.quantity << "\t" << p.price << "\n";
    }
}

// ====================== Main Menu ======================
int main() {
    int choice;

    while (true) {
        cout << "\n=== Inventory Management System ===\n";
        cout << "1. Add Product\n";
        cout << "2. View Products\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            clearCin();
            continue;
        }

        if (choice == 1) {
            addProduct();
        }
        else if (choice == 2) {
            viewProducts();
        }
        else if (choice == 0) {
            cout << "Exiting...\n";
            break;
        }
        else {
            cout << "Invalid choice! Try again.\n";
        }
    }

    return 0;
}
