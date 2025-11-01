#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <limits>   
using namespace std;


class Product {
private:
    int id;
    string name;
    int quantity;
    double price;

public:
    Product(int id, string name, int quantity, double price) {
        this->id = id;
        this->name = name;
        this->quantity = quantity;
        this->price = price;
    }

    int getId() const { return id; }
    string getName() const { return name; }
    int getQuantity() const { return quantity; }
    double getPrice() const { return price; }

    void setQuantity(int q) { quantity = q; }
    void setPrice(double p) { price = p; }

    void display() const {
        cout << "ID: " << id
             << " | Name: " << name
             << " | Quantity: " << quantity
             << " | Price: ₹" << price << endl;
    }

    string toCSV() const {
        return to_string(id) + "," + name + "," +
               to_string(quantity) + "," + to_string(price);
    }
};

class Inventory {
private:
    vector<Product> products;

public:
    void addProduct() {
        int id, quantity;
        string name;
        double price;

        cout << "\nEnter Product ID: ";
        cin >> id;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // fix 1

        cout << "Enter Product Name: ";
        getline(cin, name); // now works properly

        cout << "Enter Quantity: ";
        cin >> quantity;
        cout << "Enter Price: ";
        cin >> price;

        products.push_back(Product(id, name, quantity, price));
        cout << "✅ Product added successfully!\n";
    }

    void displayAll() const {
        if (products.empty()) {
            cout << "\n📦 Inventory is empty.\n";
            return;
        }

        cout << "\n----- Inventory -----\n";
        for (const auto& p : products) {
            p.display();
        }
    }

    void saveToFile() {
        ofstream file("inventory.csv");
        for (const auto& p : products) {
            file << p.toCSV() << endl;
        }
        file.close();
        cout << "💾 Data saved to inventory.csv successfully!\n";
    }

    void loadFromFile() {
        ifstream file("inventory.csv");
        if (!file.is_open()) return;

        products.clear();
        string line;
        while (getline(file, line)) {
            int id, quantity;
            double price;
            string name;

            size_t pos1 = line.find(',');
            size_t pos2 = line.find(',', pos1 + 1);
            size_t pos3 = line.find(',', pos2 + 1);

            id = stoi(line.substr(0, pos1));
            name = line.substr(pos1 + 1, pos2 - pos1 - 1);
            quantity = stoi(line.substr(pos2 + 1, pos3 - pos2 - 1));
            price = stod(line.substr(pos3 + 1));

            products.push_back(Product(id, name, quantity, price));
        }
        file.close();
    }
};

int main() {
    Inventory inv;
    inv.loadFromFile();

    int choice;
    do {
        cout << "\n===== Inventory Management System =====\n";
        cout << "1. Add Product\n";
        cout << "2. Display All Products\n";
        cout << "3. Save to File\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail()) { // fix 2: prevent infinite loop
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "❌ Invalid input! Please enter a number.\n";
            continue;
        }

        switch (choice) {
        case 1:
            inv.addProduct();
            break;
        case 2:
            inv.displayAll();
            break;
        case 3:
            inv.saveToFile();
            break;
        case 4:
            inv.saveToFile();
            cout << "👋 Exiting... Data saved.\n";
            break;
        default:
            cout << "❌ Invalid choice! Try again.\n";
        }
    } while (choice != 4);

    return 0;
}
