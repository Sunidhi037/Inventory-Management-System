#include <iostream>
#include <string>
#include <vector>
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

// ====================== View Products Function ======================
void viewProducts() {
    if (inventory.empty()) {
        cout << "No products available.\n";
        return;
    }

    cout << "\nID\tName\tQuantity\tPrice\n";
    for (auto &p : inventory) {
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
        cin >> choice;

        if (choice == 1) {
            cout << "Add product feature coming soon!\n"; // temporary
        }
        else if (choice == 2) {
            viewProducts();   // working feature
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
