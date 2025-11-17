#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>
#include <iomanip>
using namespace std;

// ====================== Config ======================
const string dataFile = "data/inventory.csv";

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

int findIndexById(int id) {
    for (size_t i = 0; i < inventory.size(); ++i) {
        if (inventory[i].id == id) return (int)i;
    }
    return -1;
}

string toLower(const string &s) {
    string t = s;
    for (char &c : t) c = (char)tolower((unsigned char)c);
    return t;
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
    if (!getline(fin, line)) return true;

    // If header is data, we'll reset and parse from beginning
    {
        string trimmed = line;
        while (!trimmed.empty() && isspace((unsigned char)trimmed.back())) trimmed.pop_back();
        if (!(trimmed.find_first_not_of("0123456789") == string::npos || trimmed.rfind("id,", 0) == 0)) {
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

// ====================== Pretty Table Utilities ======================
void printTableHeader() {
    cout << left << setw(6) << "ID"
         << left << setw(30) << "Name"
         << right << setw(10) << "Quantity"
         << right << setw(12) << "Price" << '\n';
    cout << string(58, '-') << '\n';
}

void printProductRow(const Product &p) {
    cout << left << setw(6) << p.id
         << left << setw(30) << p.name
         << right << setw(10) << p.quantity
         << right << setw(12) << fixed << setprecision(2) << p.price << '\n';
}

// ====================== Add Product Function (auto-save) ======================
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

    // Auto-save
    if (saveToCSV(dataFile)) cout << "Auto-saved to " << dataFile << '\n';
    else cout << "Auto-save failed.\n";
}

// ====================== Update Product Function (auto-save) ======================
void updateProduct() {
    if (inventory.empty()) {
        cout << "Inventory is empty. Nothing to update.\n";
        return;
    }

    cout << "\n--- Update Product ---\n";
    cout << "Enter product ID to update: ";
    int id;
    if (!(cin >> id)) {
        cout << "Invalid ID input.\n";
        clearCin();
        return;
    }

    int idx = findIndexById(id);
    if (idx == -1) {
        cout << "Product with ID " << id << " not found.\n";
        return;
    }

    cout << "Selected:\n";
    printTableHeader();
    printProductRow(inventory[idx]);

    cout << "Do you want to update quantity? (y/n): ";
    char ch;
    cin >> ch;
    bool changed = false;
    if (ch == 'y' || ch == 'Y') {
        cout << "Enter new quantity (integer >= 0): ";
        int newQty;
        if (!(cin >> newQty) || newQty < 0) {
            cout << "Invalid quantity. Aborting update of quantity.\n";
            clearCin();
        } else {
            inventory[idx].quantity = newQty;
            changed = true;
        }
    }

    cout << "Do you want to update price? (y/n): ";
    cin >> ch;
    if (ch == 'y' || ch == 'Y') {
        cout << "Enter new price (>= 0): ";
        double newPrice;
        if (!(cin >> newPrice) || newPrice < 0.0) {
            cout << "Invalid price. Aborting update of price.\n";
            clearCin();
        } else {
            inventory[idx].price = newPrice;
            changed = true;
        }
    }

    if (changed) {
        cout << "Product updated successfully.\n";
        if (saveToCSV(dataFile)) cout << "Auto-saved to " << dataFile << '\n';
        else cout << "Auto-save failed.\n";
    } else {
        cout << "No changes made to the product.\n";
    }
}

// ====================== Delete Product Function (auto-save) ======================
void deleteProduct() {
    if (inventory.empty()) {
        cout << "Inventory is empty. Nothing to delete.\n";
        return;
    }

    cout << "\n--- Delete Product ---\n";
    cout << "Enter product ID to delete: ";
    int id;
    if (!(cin >> id)) {
        cout << "Invalid ID input.\n";
        clearCin();
        return;
    }

    int idx = findIndexById(id);
    if (idx == -1) {
        cout << "Product with ID " << id << " not found.\n";
        return;
    }

    cout << "Selected:\n";
    printTableHeader();
    printProductRow(inventory[idx]);

    cout << "Are you sure you want to delete this product? This action cannot be undone. (y/n): ";
    char ch;
    cin >> ch;
    if (ch == 'y' || ch == 'Y') {
        inventory.erase(inventory.begin() + idx);
        cout << "Product deleted successfully.\n";
        if (saveToCSV(dataFile)) cout << "Auto-saved to " << dataFile << '\n';
        else cout << "Auto-save failed.\n";
    } else {
        cout << "Delete cancelled.\n";
    }
}

// ====================== Search Products Function ======================
void searchProducts() {
    if (inventory.empty()) {
        cout << "Inventory is empty. Nothing to search.\n";
        return;
    }

    cout << "\n--- Search Products ---\n";
    cout << "Enter name or substring to search (case-insensitive): ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string query;
    getline(cin, query);
    if (query.empty()) {
        cout << "Empty query. Aborting search.\n";
        return;
    }

    string q = toLower(query);
    vector<Product> results;
    for (const auto &p : inventory) {
        if (toLower(p.name).find(q) != string::npos) results.push_back(p);
    }

    if (results.empty()) {
        cout << "No matching products found for \"" << query << "\".\n";
        return;
    }

    cout << "\nMatches:\n";
    printTableHeader();
    for (const auto &p : results) printProductRow(p);
}

// ====================== View Products Function ======================
void viewProducts() {
    if (inventory.empty()) {
        cout << "No products available.\n";
        return;
    }

    printTableHeader();
    for (const auto &p : inventory) printProductRow(p);
}

// ====================== Main Menu ======================
int main() {
    cout << "Loading inventory from " << dataFile << " ...\n";
    if (loadFromCSV(dataFile)) cout << "Loaded inventory (" << inventory.size() << " items).\n";
    else cout << "No existing data found; starting with empty inventory.\n";

    int choice;
    while (true) {
        cout << "\n=== Inventory Management System ===\n";
        cout << "1. Add Product\n";
        cout << "2. View Products\n";
        cout << "3. Save to CSV\n";
        cout << "4. Update Product (quantity/price)\n";
        cout << "5. Delete Product\n";
        cout << "6. Search Product\n";
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
        else if (choice == 4) {
            updateProduct();
        }
        else if (choice == 5) {
            deleteProduct();
        }
        else if (choice == 6) {
            searchProducts();
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
