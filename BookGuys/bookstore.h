#ifndef bookstore_h
#define bookstore_h

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

// User Defined Headers
#include "avltree.h" // includes header with avltree functions

struct Book{
    string title;
    string author;
    int BookID;
    bool available;
    float price;
    int stock;
                        // constructor for Book                                                                     Member Initialization
    Book(const string& titleName, const string& authorName, int ID, const float& priceVal, int stockVal) : title(titleName), author(authorName), BookID(ID), 
                                                                                                            available(true), price(priceVal), stock(stockVal) {}
};

struct booksBought{
    int ID;
    booksBought* next;

    booksBought(const string& titleval, int IDno) : ID(IDno) {}
};

struct Customer{
    int customerID;
    booksBought* booksbought = NULL;
    string name;
             // constructor for Customer          Member Initialization
    Customer(int ID, const string &customername) : customerID(ID), name(customername) {}
};

struct customerNode{
    Customer customer;
    customerNode* left;
    customerNode* right;
    int height = 1;

    customerNode(const Customer& customerval) : customer(customerval), left(nullptr), right(nullptr) {}
};

struct bookNode{
    Book book;
    bookNode* left;
    bookNode* right;
    int height = 1;

    bookNode(const Book& bookval) : book(bookval), left(nullptr), right(nullptr) {}
};

class StoreSystem{
    customerNode* customerAccounts = NULL; // root for the Tree for customer accounts
    bookNode* bookList = NULL; //  root for the Tree for books

public:
    
    // constructor for StoreSystem     Member Initialization
    StoreSystem() : bookList(nullptr), customerAccounts(nullptr){}

    // Function to insert a book into the tree
    bookNode* insertBook(bookNode* root, const Book& book) {
        if (root == nullptr) {
            return new bookNode(book);
        }

        if (book.BookID < root->book.BookID) {
            root->left = insertBook(root->left, book);
        } else if (book.BookID > root->book.BookID) {
            root->right = insertBook(root->right, book);
        } else {
            // Duplicate customerID, not allowed
            return root;
        }

        // Update height
        root->height = 1 + max(getHeight(root->left), getHeight(root->right));

        // Balance the tree
        return balanceTreeNodes(root);
    }

    // Add Book information to Book Tree
    void addBook(const Book& book){  
        bookList = insertBook(bookList, book);
    }

    // inserts a customer to Customer Tree
    customerNode* insertCustomer(customerNode* root, const Customer& customer) {
        if (root == nullptr) {
            return new customerNode(customer);
        }

        if (customer.customerID < root->customer.customerID) {
            root->left = insertCustomer(root->left, customer);
        } else if (customer.customerID > root->customer.customerID) {
            root->right = insertCustomer(root->right, customer);
        } else {
            // Duplicate customerID, not allowed
            return root;
        }

        // Update height
        root->height = 1 + max(getHeight(root->left), getHeight(root->right));

        // Balance the tree
        return balanceTreeNodes(root);
    }

    // Add a customer account
    void addCustomer(const Customer& customer) {
        customerAccounts = insertCustomer(customerAccounts, customer);
    }

    // Searches for customer via ID in the Tree
    customerNode* searchCustomer(customerNode* current, int customerID){
        if (current == nullptr || current->customer.customerID == customerID) {
            return current;
        }

        if (customerID < current->customer.customerID) { // if the customerID given is lower than the current customerID, it will be searched in the left node.
            return searchCustomer(current->left, customerID);
        } else { 
            return searchCustomer(current->right, customerID); // else it will be searched in the right node
        }
    }

    // boolean function to chck if the customer ID is unique
    bool iscustomerIDUnique(int customerID) {
        return searchCustomer(customerAccounts, customerID) == nullptr;
    }

    // Search for a book in the list
    bookNode* searchBook(bookNode* current, int bookNo) {
        if (current == nullptr || current->book.BookID == bookNo) {
            return current;
        }

        if (bookNo < current->book.BookID) { // if the customerID given is lower than the current customerID, it will be searched in the left node.
            return searchBook(current->left, bookNo);
        } else { // else it will be searched in the right node
            return searchBook(current->right, bookNo);
        }
    }

    // checks if the book id is unique or not
    bool isBookIDUnique(int BookID) {
        return searchBook(bookList, BookID) == nullptr;
    }

    // Function to buy Books from the List
    void buyBook(int customerID, int BookID){
        
        customerNode* customer = searchCustomer(customerAccounts, customerID);

        if(customer == nullptr) {
            cout << "Customer not Found" << endl;
            return;
        }

        //Search for book in the list
        bookNode* book = searchBook(bookList, BookID);
        if(book == nullptr || book->book.stock == 0) {
            cout << "Book not available or out of stock" << endl;
            return;
        }

        book->book.stock--; // decrements book stock

        // Add book to booksBought list by the customer
        addBookBought(customer->customer.booksbought, book->book.title, book->book.BookID);

        cout << "Book " << book->book.title << " has been bought by " << customer->customer.name << endl;

        // code to update stock in file
        bookNode* current = searchBook(bookList, BookID);
        updateStock(BookID, current);

        printReceipt(customerID, BookID);
        cout << "Receipt has been Printed (File Output)" << endl;
        return;

    }

    // updates Stock in bookList.txt file
    void updateStock(int ID, bookNode* current) {
        string filename = "BookList.txt";
        
        ifstream inputFile(filename);
        if (!inputFile.is_open()) {
            cout << "Error opening file: " << filename << endl;
            return;
        }

        ostringstream updatedStock;
        string line;

        string searchID = to_string(ID); // converts the value of 'ID' to string and stores it to searchID

        while (getline(inputFile, line)) {
            istringstream iss(line);
            string firstWord;
            iss >> firstWord;

            if (firstWord == searchID) {
                updatedStock << current->book.BookID << "\t" << current->book.title << "\t" << current->book.author << "\t" << current->book.price << "\t" << current->book.stock << endl;
            } else {
                updatedStock << line << '\n'; // Keep other lines intact
            }
        }

        inputFile.close();

        ofstream outputFile(filename);
        if (!outputFile.is_open()) {
            cout << "Error opening file for writing: " << filename << endl;
            return;
        }

        outputFile << updatedStock.str();

        outputFile.close();
    }

    /*     Seperate codes for booksBought Linked List     */

    // add books bought in current session into booksBought structure
    void addBookBought(booksBought*& head, const string& title, int ID) { // renamed insertAtStart()
        booksBought* newBook = new booksBought(title, ID);
        newBook->next = head; 
        head = newBook; 
    }

    /*      DELETE FUNCTIONS       */

    void deleteLinefromFile(int ID, const string &filename) {
        
        ifstream inputFile(filename);
        if (!inputFile.is_open()) {
            cout << "Error opening file: " << filename << endl;
            return;
        }

        ostringstream updateLine;
        string line;

        string searchID = to_string(ID); // converts the value of 'ID' to string and stores it to searchID

        while (getline(inputFile, line)) {
            istringstream iss(line);
            string firstWord;
            iss >> firstWord;

            if (firstWord != searchID) {
                updateLine << line << '\n'; // Keep lines that do not match the search ID
            }
        }

        inputFile.close();

        ofstream outputFile(filename);
        if (!outputFile.is_open()) {
            cout << "Error opening file for writing: " << filename << endl;
            return;
        }

        outputFile << updateLine.str(); // outputs the string variant of updateline

        outputFile.close();
    }

    // Deletes book from Book Tree
    void deleteBook(int bookID) {

        bookNode* find = searchBook(bookList, bookID);
        if (find == nullptr) {
            cout << "Book with ID: " << bookID << " is not a  registered account" << endl << endl;
            return;
        }

        bookList = deleteBookHelper(bookList, bookID);
        cout << "Book with ID " << bookID << " deleted successfully!" << endl;

        // Delete book from file
        deleteLinefromFile(bookID, "BookList.txt");
    }

    // Helper function to actually delete book from tree
    bookNode* deleteBookHelper(bookNode* root, int bookID) {
        if (root == nullptr) {
            return root;
        }

        if (bookID < root->book.BookID) {
            root->left = deleteBookHelper(root->left, bookID);
        } else if (bookID > root->book.BookID) {
            root->right = deleteBookHelper(root->right, bookID);
        } else {
            // Node with only one child or no child
            if (root->left == nullptr) {
                bookNode* temp = root->right;
                delete root;
                return temp;
            } else if (root->right == nullptr) {
                bookNode* temp = root->left;
                delete root;
                return temp;
            }

            // Node with two children, get the inorder successor (smallest in the right subtree)
            bookNode* temp = findSuccessor(root->right);

            // Copy the inorder successor's content to this node
            root->book = temp->book;

            // Delete the inorder successor
            root->right = deleteBookHelper(root->right, temp->book.BookID);
        }

        // Update height and balance the tree
        root->height = 1 + max(getHeight(root->left), getHeight(root->right));
        return balanceTreeNodes(root);
    }

    void deleteAccount(int accountID) {

        customerNode* find = searchCustomer(customerAccounts, accountID);
        if (find == nullptr) {
            cout << "Book with ID: " << accountID << " is not a  registered account" << endl << endl;
            return;
        }

        customerAccounts = deleteAccountHelper(customerAccounts, accountID);
        cout << "Customer with ID " << accountID << " deleted successfully!" << endl;

        // Delete customer from file
        deleteLinefromFile(accountID, "customerList.txt");
    }


    customerNode* deleteAccountHelper(customerNode* root, int accountID) {
        if (root == nullptr) {
            return root;
        }

        if (accountID < root->customer.customerID) {
            root->left = deleteAccountHelper(root->left, accountID);
        } else if (accountID > root->customer.customerID) {
            root->right = deleteAccountHelper(root->right, accountID);
        } else {
            // Node with only one child or no child
            if (root->left == nullptr) {
                customerNode* temp = root->right;
                delete root;
                return temp;
            } else if (root->right == nullptr) {
                customerNode* temp = root->left;
                delete root;
                return temp;
            }

            // Node with two children, get the inorder successor (smallest in the right subtree)
            customerNode* temp = findSuccessor(root->right);

            // Copy the inorder successor's content to this node
            root->customer = temp->customer;

            // Delete the inorder successor
            root->right = deleteAccountHelper(root->right, temp->customer.customerID);
        }

        // Update height and balance the tree
        root->height = 1 + max(getHeight(root->left), getHeight(root->right));
        return balanceTreeNodes(root);
    }

    template <typename T>
    T* findSuccessor(T* node) {
        // Find the leftmost node in the right subtree (successor)
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    /*      Display Functions       */

    // Function to display registered Customers
    void displaycustomers(customerNode* root) {
        if (root == nullptr) {
            cout << "No customers registered." << endl;
        } else {
            displayCustomerHelper(root);
        }
    }

    void displayCustomerHelper(customerNode* root) {
        if (root != nullptr) {
            displayCustomerHelper(root->left); // Traverse left subtree
            displayCustomers(root->customer); // Display customer information
            displayCustomerHelper(root->right); // Traverse right subtree
        }
    }


    void displayCustomers(const Customer& customer) {
        cout << "customer ID: " << customer.customerID << ", Name: " << customer.name << endl;
    }

    // Function to display bought books for a customer
    void displayBooksBought(customerNode* root) {
        cout << "Bought Books: " << endl;

        if(root->customer.booksbought == nullptr) {
            cout << "No books have been bought by this user this session." << endl;
        }

        while(root->customer.booksbought != nullptr){
            bookNode* bookBought = searchBook(bookList, root->customer.booksbought->ID);
            cout << "ID: " << bookBought->book.BookID << "\nTitle: " << bookBought->book.title << "\nAuthor: " << bookBought->book.author << endl << endl;
            root->customer.booksbought = root->customer.booksbought->next;
        }
        
    }

    // Function to display registered books
    void displayRegisteredBooks(bookNode* root) {
        inputDataFromFile("Book");
        if (root == nullptr) {
            cout << "The book list is empty." << endl;
        } else {
            displayBooksHelper(root);
        }
    }

    void displayBooksHelper(bookNode* root) {
        if (root != nullptr) {
            displayBooksHelper(root->left); // Traverse left subtree
            displayBook(root->book);         // Display book information
            displayBooksHelper(root->right); // Traverse right subtree
        }
    }

    // Function to display individual book information
    void displayBook(const Book& book) {
        cout << "Book ID: " << book.BookID << ", Title: " << book.title << ", Author: " << book.author << endl;
        cout << "Price: " << book.price << ", Stock: " << book.stock << endl;
        cout << endl;
    }

    /* FILE OUTPUT FUNCTIONS */

    void outputDataToFile(int ID, const string& BookOrCustomer) {
        
        string filename;
        // Checks if the data being output is a book or a customer
        if(BookOrCustomer == "Book"){
            filename = "BookList.txt";
        } else {
            filename = "customerList.txt";
        }

        bool success = true;
        ofstream outFile(filename, ios::app);

        if (!outFile.is_open()) {
            cout << "Error opening the file." << endl;
            return;
        }

        if(BookOrCustomer == "Book"){
            outputBookDataHelper(bookList, outFile, ID, success);
        } else {
            outputCustomerHelper(customerAccounts, outFile, ID, success);
        }

        if(success) {
            cout << "Data has been added to File \"" << filename << "\"" << endl;
        }

        outFile.close();
    }

    void outputCustomerHelper(customerNode* root, ofstream& outFile, int customerID, bool &s) {

        if(iscustomerIDUnique(customerID)) { // Checks if the book does exist in the Tree
            cout << "Customer Account with ID " << customerID << " not found. Cannot output to List File" << endl;
            s = false;
            return;
        }

        customerNode* current = searchCustomer(root, customerID);
        outFile << current->customer.customerID << "\t" << current->customer.name << "\t" << endl;
        return;
    
    }

    void outputBookDataHelper(bookNode* root, ofstream& outFile, int bookID, bool &s) {

        if(isBookIDUnique(bookID)) { // Checks if the book does exist in the Tree
            cout << "Book with ID " << bookID << " not found. Cannot output to List File" << endl;
            s = false;
            return;
        }

        bookNode* current = searchBook(root, bookID);
        outFile << current->book.BookID << "\t" << current->book.title << "\t" << current->book.author << "\t" << current->book.price << "\t" << current->book.stock << endl;
        return;

    }

    /* FILE INPUT FUNCTIONS */

    void inputDataFromFile(const string& BookOrCustomer) {
        string filename;
        // Checks if the data being input is a book or a customer
        if (BookOrCustomer == "Book") {
            filename = "BookList.txt";
        } else {
            filename = "customerList.txt";
        }
        
        ifstream inFile(filename);

        if (!inFile.is_open()) {
            cout << "Error opening " << filename << endl;
            return;
        }

        //cout << filename << " has been opened." << endl;

        if (BookOrCustomer == "Book") {
            inputBookDataHelper(bookList, inFile);
        } else {
            inputCustomerHelper(customerAccounts, inFile);
        }

        inFile.close();
    }

    void inputCustomerHelper(customerNode* root, ifstream& inFile) {
        int customerID;
        string name;
        string line;

        while (getline(inFile, line)) {
            istringstream iss(line);

            if (iss >> customerID) { // sets customerID variable value
                getline(iss >> ws, name, '\t'); // sets name variable value
                    Customer newCustomer(customerID, name); 
                    addCustomer(newCustomer); // adds customer to tree
            } else {
                cout << "Error: Incorrect data format in line: " << line << endl;
            }
        }
    }

    void inputBookDataHelper(bookNode* root, ifstream& inFile) {
        int bookID;
        string title, author;
        int stock;
        double price;
        string line;

        while (getline(inFile, line)) {
            istringstream iss(line);

            if (iss >> bookID) { // sets bookID variable value
                getline(iss >> ws, title, '\t'); // sets title variable value
                getline(iss >> ws, author, '\t'); // sets author variable value
                if (iss >> price >> stock) { // sets price and stock variable value
                    Book newBook(title, author, bookID, price, stock); 
                    addBook(newBook); // adds book to tree
                } else {
                    cout << "Error: Incorrect data format in line: " << line << endl;
                }
            } else {
                cout << "Error: Incorrect data format in line: " << line << endl;
            }
        }
    }

    /* Function for Hypothetical Receipt */

    void printReceipt(int customerID, int BookID){

        ofstream outFile("Receipt.txt", ios::app);
        
        bookNode* soldBook = searchBook(bookList, BookID);
        customerNode* buyer = searchCustomer(customerAccounts, customerID);

        outFile << "Book ID: " << soldBook->book.BookID << "\nTitle: " << soldBook->book.title << "\nBuyer ID: " << buyer->customer.customerID << "\nBuyer Name: " << buyer->customer.name; 

        outFile.close();
    }

    /*   Get Codes to obtain tree Roots for Main Function algorithms */
    bookNode* getList() {
        return bookList;
    }

    customerNode* getCustomers() {
        return customerAccounts;
    }
};


#endif
