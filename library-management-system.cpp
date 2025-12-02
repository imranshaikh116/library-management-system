#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>

class Book {
private:
    char bookId[10];
    char title[100];
    char author[50];
    int quantity;
    int yearPublished;
    bool isAvailable;

public:
    void createBook() {
        std::cout << "Enter Book ID (max 9 chars): ";
        std::cin.ignore(); 
        std::cin.getline(bookId, 10);
        std::cout << "Enter Title (max 99 chars): ";
        std::cin.getline(title, 100);
        std::cout << "Enter Author (max 49 chars): ";
        std::cin.getline(author, 50);
        std::cout << "Enter Quantity: ";
        std::cin >> quantity;
        std::cout << "Enter Year Published: ";
        std::cin >> yearPublished;
        isAvailable = (quantity > 0);
        std::cin.ignore();
    }

    void showBook() const {
        std::cout << "\n--- Book Details ---" << std::endl;
        std::cout << "Book ID: " << bookId << std::endl;
        std::cout << "Title: " << title << std::endl;
        std::cout << "Author: " << author << std::endl;
        std::cout << "Quantity in Stock: " << quantity << std::endl;
        std::cout << "Year Published: " << yearPublished << std::endl;
        std::cout << "Availability: " << (isAvailable ? "Available" : "Not Available") << std::endl;
        std::cout << "--------------------" << std::endl;
    }

    const char* getBookId() const {
        return bookId;
    }

    void addBookRecord() {
        std::ofstream outFile("books.dat", std::ios::binary | std::ios::app);
        outFile.write(reinterpret_cast<char*>(this), sizeof(*this));
        outFile.close();
        std::cout << "\nBook record added successfully." << std::endl;
    }

    void displayAllBooks() {
        std::ifstream inFile("books.dat", std::ios::binary);
        if(!inFile) {
            std::cout << "\n--- ERROR: 'books.dat' file not found or empty. ---" << std::endl;
            return;
        }
        Book temp;
        std::cout << "\n================ ALL BOOK RECORDS ================" << std::endl;
        while(inFile.read(reinterpret_cast<char*>(&temp), sizeof(temp))) {
            temp.showBook();
        }
        std::cout << "================ END OF RECORDS ================" << std::endl;
        inFile.close();
    }

    void searchBook(const char* id) {
        std::ifstream inFile("books.dat", std::ios::binary);
        if(!inFile) {
            std::cout << "\n--- ERROR: 'books.dat' file not found. ---" << std::endl;
            return;
        }
        Book temp;
        bool found = false;
        while(inFile.read(reinterpret_cast<char*>(&temp), sizeof(temp))) {
            if(std::strcmp(temp.getBookId(), id) == 0) {
                std::cout << "\n--- BOOK FOUND ---" << std::endl;
                temp.showBook();
                found = true;
                break;
            }
        }
        if(!found) {
            std::cout << "\n--- Book with ID " << id << " not found! ---" << std::endl;
        }
        inFile.close();
    }

    void updateBookRecord(const char* id) {
        std::fstream file("books.dat", std::ios::binary | std::ios::in | std::ios::out);
        if(!file) {
            std::cout << "\n--- ERROR: 'books.dat' file not found. ---" << std::endl;
            return;
        }
        Book temp;
        bool found = false;
        while(file.read(reinterpret_cast<char*>(&temp), sizeof(temp))) {
            if(std::strcmp(temp.getBookId(), id) == 0) {
                std::cout << "Current details:" << std::endl;
                temp.showBook();
                std::cout << "\n--- Enter NEW details (ID " << id << ") ---" << std::endl;
                temp.createBook();
                file.seekp(-static_cast<int>(sizeof(temp)), std::ios::cur);
                file.write(reinterpret_cast<char*>(&temp), sizeof(temp));
                std::cout << "\nBook record updated successfully." << std::endl;
                found = true;
                break;
            }
        }
        if(!found) {
            std::cout << "\n--- Record not found! ---" << std::endl;
        }
        file.close();
    }

    void deleteBookRecord(const char* id) {
        std::ifstream inFile("books.dat", std::ios::binary);
        if(!inFile) {
            std::cout << "\n--- ERROR: 'books.dat' file not found. ---" << std::endl;
            return;
        }
        std::ofstream outFile("temp.dat", std::ios::binary);
        Book temp;
        bool found = false;

        while(inFile.read(reinterpret_cast<char*>(&temp), sizeof(temp))) {
            if(std::strcmp(temp.getBookId(), id) != 0) {
                outFile.write(reinterpret_cast<char*>(&temp), sizeof(temp));
            } else {
                found = true;
            }
        }
        inFile.close();
        outFile.close();
        
        if(found) {
            std::remove("books.dat");
            std::rename("temp.dat", "books.dat");
            std::cout << "\nRecord deleted successfully." << std::endl;
        } else {
            std::remove("temp.dat");
            std::cout << "\nRecord not found!" << std::endl;
        }
    }

    void issueBook() {
        char id[10];
        std::cout << "\nEnter Book ID to issue: ";
        std::cin.ignore();
        std::cin.getline(id, 10);

        std::fstream file("books.dat", std::ios::binary | std::ios::in | std::ios::out);

        Book temp;
        bool found = false;
        while(file.read(reinterpret_cast<char*>(&temp), sizeof(temp))) {
            if(std::strcmp(temp.getBookId(), id) == 0) {
                if(temp.quantity > 0) {
                    temp.quantity--;
                    temp.isAvailable = (temp.quantity > 0);
                    
                    file.seekp(-static_cast<int>(sizeof(temp)), std::ios::cur);
                    file.write(reinterpret_cast<char*>(&temp), sizeof(temp));
                    std::cout << "\nBook issued successfully. New quantity: " << temp.quantity << std::endl;
                } else {
                    std::cout << "\nBook is out of stock (Quantity: 0)." << std::endl;
                }
                found = true;
                break;
            }
        }
        if(!found) {
            std::cout << "\nBook with ID " << id << " not found!" << std::endl;
        }
        file.close();
    }

    void returnBook() {
        char id[10];
        std::cout << "\nEnter Book ID to return: ";
        std::cin.ignore();
        std::cin.getline(id, 10);
        
        std::fstream file("books.dat", std::ios::binary | std::ios::in | std::ios::out);

        Book temp;
        bool found = false;
        while(file.read(reinterpret_cast<char*>(&temp), sizeof(temp))) {
            if(std::strcmp(temp.getBookId(), id) == 0) {
                temp.quantity++;
                temp.isAvailable = true;
                
                file.seekp(-static_cast<int>(sizeof(temp)), std::ios::cur);
                file.write(reinterpret_cast<char*>(&temp), sizeof(temp));
                std::cout << "\nBook returned successfully. New quantity: " << temp.quantity << std::endl;
                found = true;
                break;
            }
        }
        if(!found) {
            std::cout << "\nBook with ID " << id << " not found!" << std::endl;
        }
        file.close();
    }
};


class User {
private:
    char userId[10];
    char name[50];
    
public:
    void createNewUser() {
        std::cout << "\nEnter User ID: ";
        std::cin.ignore(); 
        std::cin.getline(userId, 10);
        std::cout << "Enter User Name: ";
        std::cin.getline(name, 50);
    }
    const char* getUserId() const { return userId; }
};



void displayMenu() {
    std::cout << "\n\n======== LIBRARY MANAGEMENT SYSTEM ========" << std::endl;
    std::cout << "1. Add New Book" << std::endl;
    std::cout << "2. Display All Books" << std::endl;
    std::cout << "3. Search Book by ID" << std::endl;
    std::cout << "4. Modify Book Details" << std::endl;
    std::cout << "5. Delete Book Record" << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    std::cout << "6. Issue a Book" << std::endl;
    std::cout << "7. Return a Book" << std::endl;
    std::cout << "8. Exit" << std::endl;
    std::cout << "=========================================" << std::endl;
    std::cout << "Enter your choice: ";
}

int main() {
    int choice;
    Book b;
    char searchID[10];

    do {
        displayMenu();
        if (!(std::cin >> choice)) {
            std::cout << "\nInvalid input! Please enter a number." << std::endl;
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        switch(choice) {
            case 1:
                b.createBook();
                b.addBookRecord();
                break;
            case 2:
                b.displayAllBooks();
                break;
            case 3:
                std::cout << "\nEnter Book ID to search: ";
                std::cin.ignore();
                std::cin.getline(searchID, 10);
                b.searchBook(searchID);
                break;
            case 4:
                std::cout << "\nEnter Book ID to modify: ";
                std::cin.ignore();
                std::cin.getline(searchID, 10);
                b.updateBookRecord(searchID);
                break;
            case 5:
                std::cout << "\nEnter Book ID to delete: ";
                std::cin.ignore();
                std::cin.getline(searchID, 10);
                b.deleteBookRecord(searchID);
                break;
            case 6:
                b.issueBook();
                break;
            case 7:
                b.returnBook();
                break;
            case 8:
                std::cout << "\nExiting Library System. Goodbye!" << std::endl;
                break;
            default:
                std::cout << "\nInvalid choice. Please enter a number between 1 and 8." << std::endl;
        }
    } while (choice != 8);

    return 0;
}