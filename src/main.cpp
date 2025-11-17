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

vector<Product> inventory;   // NEW

int main() {
    cout << "=== Inventory Management System ===\n";
    cout << "1. Add Product\n";
    cout << "2. View Products\n";
    cout << "0. Exit\n";

    return 0;
}
