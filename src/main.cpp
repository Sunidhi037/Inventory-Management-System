#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>
#include <iomanip>
using namespace std;

// ====================== Product Class ======================
class Product {
public:
    int id;
    string name;
    int quantity;
    double price;

    Product() : id(0), name(""), quantity(0), price(0.0) {}
    Product(int _id, string _name, int _quantity, double _price) {
        id = _id;
        name = _name;
        quantity = _quantity;
        price = _price;
    }

    string toCSV() const {
        // NOTE: Name must not contain commas.
        ostringstream oss;
        oss << id << ',' << name << ',' << quantity << ',' << fixed << setprecision(2) << price;
        return oss.str();
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

// ====================== CSV Persistence ======================
bool loadFromCSV(const string &filename) {
    inventory.clear();
    ifstream fin(filename);
    if (!fin.is_open()) {
        // File doesn't exist yet — not an error.
        return false;
    }

    string line;
    // Read header (if present). We'll attempt to parse every subsequent line.
    if (!getline(fin, line)) return true;

    // If header looks like data (starts with digit), we will try parsing it too.
    // But typical file has header "id,name,quantity,price" so we skip it.
    bool headerSkipped = false;
    {
        string trimmed = line;
        while (!trimmed.empty() && isspace((unsigned char)trimmed.back())) trimmed.pop_back();
        if (trimmed.find_first_not_of("0123456789") == string::npos || trimmed.rfind("id,", 0) == 0) {
            // likely header or numeric-only line — treat as header and skip
            headerSkipped = true;
        } else {
            // line may actually be data; we'll parse it below together with others
            headerSkipped = false;
            // reset file to beginning
            fin.clear();
            fin.seekg(0);
        }
    }

    while (getline(fin, line)) {
        if (line.empty()) continue;
        vector<string> parts;
        string cur;
        stringstream ss(line);
        while (getline(ss, cur, ',')) parts.push_back(cur);
        if (parts.size() < 4) continue;
        try {
            int id = stoi(parts[0]);
            string name = parts[1];
            int qty = stoi(parts[2]);
            double price = stod(parts[3]);
            inventory.emplace_back(id, name, qty, price);
        } catch (...) {
            // skip malformed line
            continue;
        }
    }

    fin.close();
    return true;
}

bool saveToCSV(const string &filename) {
    ofstream fout(filename);
    if (!fout.is_open()) {
        cerr << "Error: cannot open file " << filename << " for writing.\n";
        return false;
    }
    fout << "id,name,quantity,price\n";
    for (const auto &p : inventory) fout << p.toCSV() << '\n';
    fout.close();
    return true;
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
    const string dataFile = "data/inventory.csv";

    cout << "Loading inventory from " << dataFile << " ...\n";
    if (loadFromCSV(dataFile)) cout << "Loaded inventory (" << inventory.size() << " items).\n";
    else cout << "No existing data found; starting with empty inventory.\n";

    int choice;
    while (true) {
        cout << "\n=== Inventory Management System ===\n";
        cout << "1. Add Product\n";
        cout << "2. View Products\n";
        cout << "3. Save to CSV\n";
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
        else if (choice == 3) {
            if (saveToCSV(dataFile)) cout << "Saved to " << dataFile << '\n';
            else cout << "Save failed.\n";
        }
        else if (choice == 0) {
            cout << "Saving before exit...\n";
            if (saveToCSV(dataFile)) cout << "Saved to " << dataFile << '\n';
            else cout << "Warning: could not save to " << dataFile << '\n';
            cout << "Goodbye!\n";
            break;
        }
        else {
            cout << "Invalid choice! Try again.\n";
        }
    }

    return 0;
}
