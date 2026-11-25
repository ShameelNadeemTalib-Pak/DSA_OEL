// Online Shopping and Order Management System
// Data Structures used: Linked List (products), Stack (shopping cart), Queue (orders)
// Algorithms used: Linear Search, Bubble Sort
// File handling: products.txt, inventory.txt, orders.txt, processed_orders.txt

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cstdlib>
using namespace std;

#define MAX_ITEMS 50
const int INVALID_INT = -999999;

// ------------------------------------------------------------
// Helper functions
// ------------------------------------------------------------
string toLower(string s)
{
    for (int i = 0; i < (int)s.length(); i++)
        if (s[i] >= 'A' && s[i] <= 'Z')
            s[i] = s[i] + 32;
    return s;
}

// '|' is used as separator in files, so it is not allowed in text
string cleanText(string s)
{
    for (int i = 0; i < (int)s.length(); i++)
        if (s[i] == '|')
            s[i] = '-';
    return s;
}

string readLine(string prompt)
{
    string s;
    cout << prompt;
    if (!getline(cin, s))
        exit(0);        // input finished
    return s;
}

// returns INVALID_INT if the input is not a proper integer
int readInt(string prompt)
{
    string line = readLine(prompt);
    istringstream ss(line);
    int x;
    char extra;
    if (!(ss >> x))
        return INVALID_INT;
    if (ss >> extra)
        return INVALID_INT;
    return x;
}

// returns -1 if the input is not a proper number
double readDouble(string prompt)
{
    string line = readLine(prompt);
    istringstream ss(line);
    double x;
    char extra;
    if (!(ss >> x))
        return -1;
    if (ss >> extra)
        return -1;
    return x;
}

// ------------------------------------------------------------
// Product (Singly Linked List)
// ------------------------------------------------------------
class Product
{
public:
    int id;
    string name;
    string category;
    double price;
    int stock;
    Product* next;
};

class ProductList
{
public:
    Product* head;

    ProductList() { head = NULL; }

    ~ProductList()
    {
        while (head != NULL)
        {
            Product* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // Returns false if the ID already exists
    bool addProduct(int id, string name, string category, double price, int stock)
    {
        if (searchById(id) != NULL)
            return false;

        /* 1. allocate node */
        Product* newNode = new Product();

        /* 2. put in the data */
        newNode->id = id;
        newNode->name = name;
        newNode->category = category;
        newNode->price = price;
        newNode->stock = stock;
        newNode->next = NULL;

        /* 3. if list is empty, new node becomes head */
        if (head == NULL)
        {
            head = newNode;
            return true;
        }

        /* 4. else traverse till the last node and attach */
        Product* last = head;
        while (last->next != NULL)
            last = last->next;
        last->next = newNode;
        return true;
    }

    // Linear search by ID
    Product* searchById(int id)
    {
        Product* temp = head;
        while (temp != NULL)
        {
            if (temp->id == id)
                return temp;
            temp = temp->next;
        }
        return NULL;
    }

    // Linear search by name (not case sensitive, partial match allowed)
    int searchByName(string name)
    {
        int found = 0;
        string key = toLower(name);
        Product* temp = head;
        while (temp != NULL)
        {
            if (toLower(temp->name).find(key) != string::npos)
            {
                if (found == 0)
                    printHeader();
                printProduct(temp);
                found++;
            }
            temp = temp->next;
        }
        return found;
    }

    // Linear search by category (not case sensitive, partial match allowed)
    int searchByCategory(string category)
    {
        int found = 0;
        string key = toLower(category);
        Product* temp = head;
        while (temp != NULL)
        {
            if (toLower(temp->category).find(key) != string::npos)
            {
                if (found == 0)
                    printHeader();
                printProduct(temp);
                found++;
            }
            temp = temp->next;
        }
        return found;
    }

    // Returns false if product is not found
    bool deleteProduct(int id)
    {
        Product* temp = head;
        Product* prev = NULL;
        while (temp != NULL && temp->id != id)
        {
            prev = temp;
            temp = temp->next;
        }
        if (temp == NULL)
            return false;

        if (prev == NULL)
            head = temp->next;      // deleting the first node
        else
            prev->next = temp->next;
        delete temp;
        return true;
    }

    int count()
    {
        int c = 0;
        Product* temp = head;
        while (temp != NULL)
        {
            c++;
            temp = temp->next;
        }
        return c;
    }

    void printHeader()
    {
        cout << left << setw(8) << "ID" << setw(20) << "Name"
             << setw(15) << "Category" << setw(10) << "Price"
             << setw(8) << "Stock" << endl;
        cout << "------------------------------------------------------------" << endl;
    }

    void printProduct(Product* p)
    {
        cout << left << setw(8) << p->id << setw(20) << p->name
             << setw(15) << p->category << fixed << setprecision(2)
             << setw(10) << p->price << setw(8) << p->stock << endl;
    }

    void display()
    {
        if (head == NULL)
        {
            cout << "No products available." << endl;
            return;
        }
        printHeader();
        Product* temp = head;
        while (temp != NULL)
        {
            printProduct(temp);
            temp = temp->next;
        }
    }

    // choice: 1 = ID, 2 = Name, 3 = Category, 4 = Price (ascending)
    bool shouldSwap(Product* a, Product* b, int choice)
    {
        if (choice == 1)
            return a->id > b->id;
        if (choice == 2)
            return toLower(a->name) > toLower(b->name);
        if (choice == 3)
            return toLower(a->category) > toLower(b->category);
        return a->price > b->price;
    }

    void swapData(Product* a, Product* b)
    {
        int tid = a->id;           a->id = b->id;             b->id = tid;
        string tname = a->name;    a->name = b->name;         b->name = tname;
        string tcat = a->category; a->category = b->category; b->category = tcat;
        double tprice = a->price;  a->price = b->price;       b->price = tprice;
        int tstock = a->stock;     a->stock = b->stock;       b->stock = tstock;
    }

    // Bubble sort on the linked list
    void bubbleSort(int choice)
    {
        if (head == NULL || head->next == NULL)
            return;

        bool swapped;
        Product* last = NULL;
        do
        {
            swapped = false;
            Product* temp = head;
            while (temp->next != last)
            {
                if (shouldSwap(temp, temp->next, choice))
                {
                    swapData(temp, temp->next);
                    swapped = true;
                }
                temp = temp->next;
            }
            last = temp;
        } while (swapped);
    }
};

// ------------------------------------------------------------
// Shopping Cart (Stack using Linked List)
// ------------------------------------------------------------
class CartItem
{
public:
    int productId;
    string name;
    double price;
    int qty;
    CartItem* next;
};

class CartStack
{
public:
    CartItem* top;
    int size;

    CartStack()
    {
        top = NULL;
        size = 0;
    }

    ~CartStack() { clear(); }

    bool isEmpty() { return top == NULL; }

    CartItem* find(int productId)
    {
        CartItem* temp = top;
        while (temp != NULL)
        {
            if (temp->productId == productId)
                return temp;
            temp = temp->next;
        }
        return NULL;
    }

    // push a new item on top of the stack
    void push(int productId, string name, double price, int qty)
    {
        CartItem* newItem = new CartItem();
        newItem->productId = productId;
        newItem->name = name;
        newItem->price = price;
        newItem->qty = qty;
        newItem->next = top;
        top = newItem;
        size++;
    }

    // pop the item on top of the stack
    bool pop(CartItem& removed)
    {
        if (top == NULL)
            return false;
        CartItem* temp = top;
        removed = *temp;
        top = top->next;
        delete temp;
        size--;
        return true;
    }

    void clear()
    {
        CartItem dummy;
        while (pop(dummy)) { }
    }

    double total()
    {
        double sum = 0;
        CartItem* temp = top;
        while (temp != NULL)
        {
            sum = sum + temp->price * temp->qty;
            temp = temp->next;
        }
        return sum;
    }

    void display()
    {
        if (top == NULL)
        {
            cout << "Shopping cart is empty." << endl;
            return;
        }
        cout << left << setw(8) << "ID" << setw(20) << "Name" << setw(10) << "Price"
             << setw(6) << "Qty" << setw(12) << "Amount" << endl;
        cout << "------------------------------------------------------" << endl;
        CartItem* temp = top;
        while (temp != NULL)
        {
            cout << left << setw(8) << temp->productId << setw(20) << temp->name
                 << fixed << setprecision(2) << setw(10) << temp->price
                 << setw(6) << temp->qty << setw(12) << temp->price * temp->qty << endl;
            temp = temp->next;
        }
        cout << "------------------------------------------------------" << endl;
        cout << "Total Amount: " << total() << endl;
    }
};

// ------------------------------------------------------------
// Orders (Queue using Linked List)
// ------------------------------------------------------------
class Order
{
public:
    int orderId;
    string customer;
    int itemCount;
    int ids[MAX_ITEMS];
    string names[MAX_ITEMS];
    double prices[MAX_ITEMS];
    int qtys[MAX_ITEMS];
    double total;
    Order* next;

    Order()
    {
        orderId = 0;
        itemCount = 0;
        total = 0;
        next = NULL;
    }

    void display()
    {
        cout << "Order ID : " << orderId << endl;
        cout << "Customer : " << customer << endl;
        cout << left << setw(8) << "ID" << setw(20) << "Name" << setw(10) << "Price"
             << setw(6) << "Qty" << setw(12) << "Amount" << endl;
        cout << "------------------------------------------------------" << endl;
        for (int i = 0; i < itemCount; i++)
        {
            cout << left << setw(8) << ids[i] << setw(20) << names[i]
                 << fixed << setprecision(2) << setw(10) << prices[i]
                 << setw(6) << qtys[i] << setw(12) << prices[i] * qtys[i] << endl;
        }
        cout << "------------------------------------------------------" << endl;
        cout << "Total Purchase Amount: " << total << endl;
    }
};

class OrderQueue
{
public:
    Order* front;
    Order* rear;

    OrderQueue()
    {
        front = NULL;
        rear = NULL;
    }

    ~OrderQueue()
    {
        while (front != NULL)
        {
            Order* temp = dequeue();
            delete temp;
        }
    }

    bool isEmpty() { return front == NULL; }

    void enqueue(Order* newOrder)
    {
        newOrder->next = NULL;
        if (rear == NULL)
        {
            front = newOrder;
            rear = newOrder;
            return;
        }
        rear->next = newOrder;
        rear = newOrder;
    }

    // returns NULL if the queue is empty, caller must delete the order
    Order* dequeue()
    {
        if (front == NULL)
            return NULL;
        Order* temp = front;
        front = front->next;
        if (front == NULL)
            rear = NULL;
        temp->next = NULL;
        return temp;
    }

    void display()
    {
        if (front == NULL)
        {
            cout << "No pending orders." << endl;
            return;
        }
        Order* temp = front;
        while (temp != NULL)
        {
            cout << "Order #" << temp->orderId << "  Customer: " << temp->customer
                 << "  Items: " << temp->itemCount << "  Total: " << fixed
                 << setprecision(2) << temp->total << endl;
            temp = temp->next;
        }
    }
};

// ------------------------------------------------------------
// File handling
// ------------------------------------------------------------
int splitLine(string line, string parts[], int maxParts)
{
    istringstream ss(line);
    string field;
    int n = 0;
    while (n < maxParts && getline(ss, field, '|'))
    {
        parts[n] = field;
        n++;
    }
    return n;
}

void writeOrder(ofstream& out, Order* o)
{
    out << "ORDER|" << o->orderId << "|" << o->customer << "|" << o->total
        << "|" << o->itemCount << endl;
    for (int i = 0; i < o->itemCount; i++)
        out << "ITEM|" << o->ids[i] << "|" << o->names[i] << "|" << o->prices[i]
            << "|" << o->qtys[i] << endl;
}

void saveAll(ProductList& products, OrderQueue& orders, int nextOrderId)
{
    ofstream pf("products.txt");
    ofstream inv("inventory.txt");
    Product* p = products.head;
    while (p != NULL)
    {
        pf << p->id << "|" << p->name << "|" << p->category << "|" << p->price << endl;
        inv << p->id << "|" << p->stock << endl;
        p = p->next;
    }
    pf.close();
    inv.close();

    ofstream of("orders.txt");
    of << nextOrderId << endl;
    Order* o = orders.front;
    while (o != NULL)
    {
        writeOrder(of, o);
        o = o->next;
    }
    of.close();
}

void loadAll(ProductList& products, OrderQueue& orders, int& nextOrderId)
{
    string line;
    string parts[6];

    // product records
    ifstream pf("products.txt");
    while (getline(pf, line))
    {
        if (splitLine(line, parts, 6) == 4)
            products.addProduct(atoi(parts[0].c_str()), parts[1], parts[2],
                                atof(parts[3].c_str()), 0);
    }
    pf.close();

    // inventory details
    ifstream inv("inventory.txt");
    while (getline(inv, line))
    {
        if (splitLine(line, parts, 6) == 2)
        {
            Product* p = products.searchById(atoi(parts[0].c_str()));
            if (p != NULL)
                p->stock = atoi(parts[1].c_str());
        }
    }
    inv.close();

    // pending orders
    ifstream of("orders.txt");
    if (getline(of, line))
        nextOrderId = atoi(line.c_str());
    Order* current = NULL;
    while (getline(of, line))
    {
        int n = splitLine(line, parts, 6);
        if (parts[0] == "ORDER" && n == 5)
        {
            current = new Order();
            current->orderId = atoi(parts[1].c_str());
            current->customer = parts[2];
            current->total = atof(parts[3].c_str());
            orders.enqueue(current);
        }
        else if (parts[0] == "ITEM" && n == 5 && current != NULL
                 && current->itemCount < MAX_ITEMS)
        {
            int i = current->itemCount;
            current->ids[i] = atoi(parts[1].c_str());
            current->names[i] = parts[2];
            current->prices[i] = atof(parts[3].c_str());
            current->qtys[i] = atoi(parts[4].c_str());
            current->itemCount++;
        }
    }
    of.close();
}

// ------------------------------------------------------------
// Product management
// ------------------------------------------------------------
void addProductUI(ProductList& products)
{
    int id = readInt("Enter product ID: ");
    if (id <= 0)
    {
        cout << "Invalid product ID. It must be a positive number." << endl;
        return;
    }
    if (products.searchById(id) != NULL)
    {
        cout << "Duplicate product ID. Product was not added." << endl;
        return;
    }
    string name = cleanText(readLine("Enter product name: "));
    if (name == "")
    {
        cout << "Product name cannot be empty." << endl;
        return;
    }
    string category = cleanText(readLine("Enter category: "));
    if (category == "")
    {
        cout << "Category cannot be empty." << endl;
        return;
    }
    double price = readDouble("Enter price: ");
    if (price <= 0)
    {
        cout << "Invalid price. It must be greater than 0." << endl;
        return;
    }
    int stock = readInt("Enter stock quantity: ");
    if (stock < 0)
    {
        cout << "Invalid stock quantity." << endl;
        return;
    }
    products.addProduct(id, name, category, price, stock);
    cout << "Product added successfully." << endl;
}

void updateProductUI(ProductList& products)
{
    int id = readInt("Enter ID of the product to update: ");
    Product* p = products.searchById(id);
    if (p == NULL)
    {
        cout << "Product not found." << endl;
        return;
    }
    string name = cleanText(readLine("Enter new name: "));
    if (name == "")
    {
        cout << "Product name cannot be empty." << endl;
        return;
    }
    string category = cleanText(readLine("Enter new category: "));
    if (category == "")
    {
        cout << "Category cannot be empty." << endl;
        return;
    }
    double price = readDouble("Enter new price: ");
    if (price <= 0)
    {
        cout << "Invalid price. Record was not updated." << endl;
        return;
    }
    p->name = name;
    p->category = category;
    p->price = price;
    cout << "Product updated successfully." << endl;
}

void updateStockUI(ProductList& products)
{
    int id = readInt("Enter product ID: ");
    Product* p = products.searchById(id);
    if (p == NULL)
    {
        cout << "Product not found." << endl;
        return;
    }
    int qty = readInt("Enter quantity to add to stock: ");
    if (qty <= 0)
    {
        cout << "Invalid quantity. It must be greater than 0." << endl;
        return;
    }
    p->stock = p->stock + qty;
    cout << "Stock updated. New stock = " << p->stock << endl;
}

void deleteProductUI(ProductList& products)
{
    int id = readInt("Enter ID of the product to delete: ");
    if (products.deleteProduct(id))
        cout << "Product deleted successfully." << endl;
    else
        cout << "Product not found." << endl;
}

void productMenu(ProductList& products, OrderQueue& orders, int& nextOrderId)
{
    int choice = 0;
    while (choice != 6)
    {
        cout << "\n--- PRODUCT MANAGEMENT ---" << endl;
        cout << "1. Add product" << endl;
        cout << "2. Update product" << endl;
        cout << "3. Update stock (restock)" << endl;
        cout << "4. Delete product" << endl;
        cout << "5. Display all products" << endl;
        cout << "6. Back" << endl;
        choice = readInt("Enter your choice: ");

        if (choice == 1)
            addProductUI(products);
        else if (choice == 2)
            updateProductUI(products);
        else if (choice == 3)
            updateStockUI(products);
        else if (choice == 4)
            deleteProductUI(products);
        else if (choice == 5)
            products.display();
        else if (choice != 6)
            cout << "Invalid choice." << endl;

        if (choice >= 1 && choice <= 4)
            saveAll(products, orders, nextOrderId);
    }
}

// ------------------------------------------------------------
// Searching and sorting
// ------------------------------------------------------------
void searchMenu(ProductList& products)
{
    if (products.head == NULL)
    {
        cout << "No products available to search." << endl;
        return;
    }
    cout << "\n--- SEARCH PRODUCTS ---" << endl;
    cout << "1. Search by ID" << endl;
    cout << "2. Search by name" << endl;
    cout << "3. Search by category" << endl;
    int choice = readInt("Enter your choice: ");

    if (choice == 1)
    {
        int id = readInt("Enter product ID: ");
        Product* p = products.searchById(id);
        if (p == NULL)
            cout << "Product not found." << endl;
        else
        {
            products.printHeader();
            products.printProduct(p);
        }
    }
    else if (choice == 2)
    {
        string name = readLine("Enter product name: ");
        if (products.searchByName(name) == 0)
            cout << "No product found with this name." << endl;
    }
    else if (choice == 3)
    {
        string cat = readLine("Enter category: ");
        if (products.searchByCategory(cat) == 0)
            cout << "No product found in this category." << endl;
    }
    else
        cout << "Invalid choice." << endl;
}

void sortMenu(ProductList& products)
{
    if (products.head == NULL)
    {
        cout << "No products available to sort." << endl;
        return;
    }
    cout << "\n--- SORT PRODUCTS ---" << endl;
    cout << "1. Sort by ID" << endl;
    cout << "2. Sort by name" << endl;
    cout << "3. Sort by category" << endl;
    cout << "4. Sort by price" << endl;
    int choice = readInt("Enter your choice: ");

    if (choice >= 1 && choice <= 4)
    {
        products.bubbleSort(choice);
        cout << "Products sorted (ascending order)." << endl;
        products.display();
    }
    else
        cout << "Invalid choice." << endl;
}

// ------------------------------------------------------------
// Shopping cart
// ------------------------------------------------------------
void addToCart(ProductList& products, CartStack& cart)
{
    int id = readInt("Enter product ID: ");
    Product* p = products.searchById(id);
    if (p == NULL)
    {
        cout << "Product is unavailable (not found)." << endl;
        return;
    }
    if (p->stock == 0)
    {
        cout << "Product is unavailable (out of stock)." << endl;
        return;
    }
    int qty = readInt("Enter quantity: ");
    if (qty <= 0)
    {
        cout << "Invalid quantity. It must be greater than 0." << endl;
        return;
    }

    CartItem* existing = cart.find(id);
    int alreadyInCart = 0;
    if (existing != NULL)
        alreadyInCart = existing->qty;

    if (qty + alreadyInCart > p->stock)
    {
        cout << "Insufficient stock. Available stock = " << p->stock << endl;
        return;
    }

    if (existing != NULL)
        existing->qty = existing->qty + qty;
    else
    {
        if (cart.size >= MAX_ITEMS)
        {
            cout << "Cart is full." << endl;
            return;
        }
        cart.push(p->id, p->name, p->price, qty);
    }
    cout << "Product added to cart." << endl;
}

void removeFromCart(CartStack& cart)
{
    CartItem removed;
    if (cart.pop(removed))
        cout << "Removed from cart: " << removed.name << " (Qty " << removed.qty << ")" << endl;
    else
        cout << "Shopping cart is empty. Nothing to remove." << endl;
}

void checkout(ProductList& products, CartStack& cart, OrderQueue& orders, int& nextOrderId)
{
    if (cart.isEmpty())
    {
        cout << "Shopping cart is empty. Cannot place order." << endl;
        return;
    }
    string customer = cleanText(readLine("Enter customer name: "));
    if (customer == "")
    {
        cout << "Customer name cannot be empty." << endl;
        return;
    }

    // check availability of every item before placing the order
    CartItem* c = cart.top;
    while (c != NULL)
    {
        Product* p = products.searchById(c->productId);
        if (p == NULL)
        {
            cout << "Order failed. " << c->name << " is no longer available." << endl;
            return;
        }
        if (p->stock < c->qty)
        {
            cout << "Order failed. Insufficient stock for " << c->name
                 << ". Available stock = " << p->stock << endl;
            return;
        }
        c = c->next;
    }

    // create order, reduce stock
    Order* o = new Order();
    o->orderId = nextOrderId;
    o->customer = customer;
    c = cart.top;
    while (c != NULL)
    {
        int i = o->itemCount;
        o->ids[i] = c->productId;
        o->names[i] = c->name;
        o->prices[i] = c->price;
        o->qtys[i] = c->qty;
        o->itemCount++;

        Product* p = products.searchById(c->productId);
        p->stock = p->stock - c->qty;
        c = c->next;
    }
    o->total = cart.total();

    orders.enqueue(o);
    nextOrderId++;
    cart.clear();
    cout << "Order placed successfully. Order ID = " << o->orderId << endl;
}

void cartMenu(ProductList& products, CartStack& cart, OrderQueue& orders, int& nextOrderId)
{
    int choice = 0;
    while (choice != 5)
    {
        cout << "\n--- SHOPPING CART ---" << endl;
        cout << "1. Add product to cart" << endl;
        cout << "2. Remove last added product (pop)" << endl;
        cout << "3. View cart" << endl;
        cout << "4. Place order (checkout)" << endl;
        cout << "5. Back" << endl;
        choice = readInt("Enter your choice: ");

        if (choice == 1)
            addToCart(products, cart);
        else if (choice == 2)
            removeFromCart(cart);
        else if (choice == 3)
            cart.display();
        else if (choice == 4)
        {
            checkout(products, cart, orders, nextOrderId);
            saveAll(products, orders, nextOrderId);
        }
        else if (choice != 5)
            cout << "Invalid choice." << endl;
    }
}

// ------------------------------------------------------------
// Order management
// ------------------------------------------------------------
void processOrder(ProductList& products, OrderQueue& orders, int nextOrderId)
{
    Order* o = orders.dequeue();
    if (o == NULL)
    {
        cout << "No orders to process. The order queue is empty." << endl;
        return;
    }
    cout << "Processing order..." << endl;
    o->display();

    // keep a permanent record of processed orders
    ofstream log("processed_orders.txt", ios::app);
    writeOrder(log, o);
    log.close();

    delete o;
    saveAll(products, orders, nextOrderId);
    cout << "Order processed successfully." << endl;
}

void orderMenu(ProductList& products, OrderQueue& orders, int& nextOrderId)
{
    int choice = 0;
    while (choice != 3)
    {
        cout << "\n--- ORDER MANAGEMENT ---" << endl;
        cout << "1. Process next order" << endl;
        cout << "2. View pending orders" << endl;
        cout << "3. Back" << endl;
        choice = readInt("Enter your choice: ");

        if (choice == 1)
            processOrder(products, orders, nextOrderId);
        else if (choice == 2)
            orders.display();
        else if (choice != 3)
            cout << "Invalid choice." << endl;
    }
}

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------
int main()
{
    ProductList products;
    CartStack cart;
    OrderQueue orders;
    int nextOrderId = 1;

    loadAll(products, orders, nextOrderId);

    int choice = 0;
    while (choice != 6)
    {
        cout << "\n===== ONLINE SHOPPING AND ORDER MANAGEMENT SYSTEM =====" << endl;
        cout << "1. Product management" << endl;
        cout << "2. Search products" << endl;
        cout << "3. Sort and display products" << endl;
        cout << "4. Shopping cart" << endl;
        cout << "5. Order management" << endl;
        cout << "6. Save and exit" << endl;
        choice = readInt("Enter your choice: ");

        if (choice == 1)
            productMenu(products, orders, nextOrderId);
        else if (choice == 2)
            searchMenu(products);
        else if (choice == 3)
        {
            sortMenu(products);
            saveAll(products, orders, nextOrderId);
        }
        else if (choice == 4)
            cartMenu(products, cart, orders, nextOrderId);
        else if (choice == 5)
            orderMenu(products, orders, nextOrderId);
        else if (choice == 6)
        {
            saveAll(products, orders, nextOrderId);
            cout << "Data saved. Goodbye!" << endl;
        }
        else
            cout << "Invalid choice." << endl;
    }
    return 0;
}
