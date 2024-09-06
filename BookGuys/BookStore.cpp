// User defined Header
#include "bookstore.h"

// Global function to use input validation
template <typename T>
void getInput(T& input, const string& prompt) {
    while (true) {
        cout << prompt;
        if (cin >> input && cin.peek() == '\n') {
            break;
        } else {

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please try again." << endl;
        }
    }
    cin.ignore(); // ignores whitespace
    return;
}

int main() {
    StoreSystem store;

    // Function calls to input Book/User List if any have been stored
    store.inputDataFromFile("Book");
    store.inputDataFromFile("Customer");

    int choice;
    do {
        cout << "\n===== Store Management System =====" << endl;
        cout << "1. Add Book\n2. Add Customer\n3. Buy Book\n4. Display customers\n5. Display Registered Books\n6. Check books bought by Customer in Session\n7. Delete\n8. Check Tree Balance Factor\n9. Exit\n";
        getInput(choice, "Enter your choice: ");
        switch (choice) {
            case 1: { // Add Book
                string title, author;
                int bookID, stock;
                float price;
                bool unique;
                
                do{

                    getInput(bookID, "Enter Book ID: ");
                    
                    if (store.isBookIDUnique(bookID)) {
                        getInput(price, "Enter Price: ");

                        getInput(stock, "Enter Stock: ");

                        cout << "Enter Book Title: ";
                        getline(cin, title);

                        cout << "Enter Author: ";
                        getline(cin, author);

                        Book newBook(title, author, bookID, price, stock);
                        store.addBook(newBook);
                        cout << "Book added successfully!" << endl;
                        unique = true;
                    } else {
                        cout << "Book ID already exists. Please enter a unique Book ID." << endl;
                    }

                } while(unique == false);

                store.outputDataToFile(bookID, "Book"); // Outputs Book to .txt for File I/O
                break;
            }

            case 2: { // Add Customer
                string name;
                int customerID;

                cout << "Enter Customer Name: ";
                getline(cin, name);

                getInput(customerID, "Enter Customer ID: ");

                if (store.iscustomerIDUnique(customerID)) {
                    Customer newCustomer(customerID, name);
                    store.addCustomer(newCustomer);
                    cout << "Customer added successfully!" << endl;
                } else {
                    cout << "Customer ID already exists. Please enter a unique Customer ID." << endl;
                }

                store.outputDataToFile(customerID, "Customer"); // Outputs Book to .txt for File I/O
                break;
            }
            case 3: { // Buy Book
               
                int customerID, bookID;

                getInput(customerID, "Enter Customer ID: ");

                getInput(bookID, "Enter Book ID: ");

                store.buyBook(customerID, bookID);

                break;
            }
            case 4: { // Display customers
               
                cout << "\n===== Registered customers =====" << endl;
                store.displaycustomers(store.getCustomers());
                break;
            }
            case 5: { // Display Registered Books
               
                cout << "\n===== Registered Books =====" << endl;
                store.displayRegisteredBooks(store.getList());
                break;
            }
            case 6: { // Check Books bought by customer
               
                int customerID;
                cout << "\n===== Check books bought by Customer in Session =====" << endl;
                getInput(customerID, "Enter Customer ID: ");

                // Search for the customer in the system
                customerNode* customer = store.searchCustomer(store.getCustomers(), customerID);

                if (customer == nullptr) {
                    cout << "Customer not found." << endl;
                    break;
                }

                // Display books bought by the customer
                store.displayBooksBought(customer);
                break;
            }

            case 7: {
                int delChoice;
                bool fin = false;
                    do{
                        cout << "\nWhat do you want to delete?\n1. Book from Catalogue\n2. User Accout\n3. Back" << endl;
                        getInput(delChoice, "Enter choice: ");
                        switch(delChoice){
                            case 1: {
                                int ID;
                                getInput(ID,"\nEnter Book ID: ");
                                store.deleteBook(ID);
                                fin = true;
                                break;
                            }
                            case 2: {
                                int ID;
                                getInput(ID,"Enter Customer ID: ");
                                store.deleteAccount(ID);
                                fin = true;
                                break;
                            }
                            case 3: {
                                cout << endl << "Going back to Menu..." << endl << endl;
                                break;
                            }
                            default: {
                                cout << "Invalid choice. Please enter a valid option.\n";
                                break;
                            }
                        }
                    }while(delChoice != 3 && !fin);

                break;
            }

            case 8:{
                int balChoice;
                bool fin = false;
                do{
                    cout << "\nWhat Tree would you like to check the balance of\n1. Book Tree\t2. Accounts Tree\n3. Go back to Menu" << endl;
                    getInput(balChoice, "Enter Choice: ");
                    switch(balChoice){
                        case 1: {

                            bookNode* bookBal = store.getList();
                            if(bookBal == nullptr){
                                cout << "Cannot get Balance factor, Tree is empty" << endl;
                                break;
                            }

                            cout << "Book Tree Balance factor ( " << "Root Node: " << bookBal->book.BookID << " ): " << getBalance(store.getList()) << endl;
                            fin = true;
                            break;
                        }
                        case 2: {
                            customerNode* accBal = store.getCustomers();
                            if(accBal == nullptr){
                                cout << "Cannot get Balance factor, Tree is empty" << endl;
                                break;
                            }
                            cout << "Book Tree Balance factor ( " << "Root Node: " << accBal->customer.customerID << " ): " << getBalance(store.getList()) << endl;
                            fin = true;
                            break;
                        }
                        case 3:{
                            cout << endl << "Going back to Menu...\n" << endl;
                            break;
                        }
                        default: {
                            cout << "Invalid choice. Please enter a valid option.\n";
                            break;
                        }
                    }
                }while(balChoice != 3 && !fin);
                break;
            }

            case 9: // exit program
                cout << "Exiting the program. Goodbye!\n";
                break;
                
            default:
                cout << "Invalid choice. Please enter a valid option.\n";
                break;
        }
        system("pause");
        system("cls");
    } while (choice != 9);
    return 0;
}
