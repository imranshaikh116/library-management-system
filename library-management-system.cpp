#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <vector>
#include <algorithm>

class Book {
private:
    char bookId[10];
    char title[100];
    char author[50];
    int quantity;
    int yearPublished;
    bool isAvailable; 

public:
    Book() : quantity(0), yearPublished(0), isAvailable(false) {
        bookId[0] = '\0';
        title[0] = '\0';
        author[0] = '\0';
    }

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

    const char* getBookId() const { return bookId; }
    const char* getTitle() const { return title; }
    const char* getAuthor() const { return author; }
    int getQuantity() const { return quantity; }

    void setQuantity(int newQuantity) {
        quantity = newQuantity;
        isAvailable = (quantity > 0);
    }
};


class LibraryManager {
private:
    std::vector<Book> bookList; 
    const char* fileName = "books.dat";

    std::string getBookIdFromUser() {
        std::string id;
        std::cout << "Enter Book ID: ";
        std::cin.ignore();
        std::getline(std::cin, id);
        return id;
    }

public:
    void loadData() {
        std::ifstream inFile(fileName, std::ios::binary);
        if (inFile.is_open()) {
            Book temp;
            while (inFile.read(reinterpret_cast<char*>(&temp), sizeof(temp))) {
                bookList.push_back(temp);
            }
            std::cout << "Data loaded: " << bookList.size() << " records found." << std::endl;
            inFile.close();
        } else {
            std::cout << "No existing data file found. Starting fresh." << std::endl;
        }
    }

    void saveData() {
        std::ofstream outFile(fileName, std::ios::binary);
        if (outFile.is_open()) {
            for (const auto& book : bookList) {
                outFile.write(reinterpret_cast<const char*>(&book), sizeof(book)); 
            }
            std::cout << "All data saved: " << bookList.size() << " records." << std::endl;
            outFile.close();
        } else {
            std::cerr << "ERROR: Could not open file for saving!" << std::endl;
        }
    }


    void addBookRecord() {
        Book newBook;
        newBook.createBook();
        bookList.push_back(newBook);
        std::cout << "\nBook added to memory successfully." << std::endl;
    }

    void displayAllBooks() {
        if (bookList.empty()) {
            std::cout << "\nNo books in the library." << std::endl;
            return;
        }
        std::cout << "\n======== ALL BOOK RECORDS (In-Memory) ========" << std::endl;
        for (const auto& book : bookList) {
            book.showBook();
        }
        std::cout << "================ END OF RECORDS ================" << std::endl;
    }

    void searchBook() {
        std::string id = getBookIdFromUser();
        bool found = false;
        
        for (const auto& book : bookList) {
            if (id == book.getBookId()) {
                book.showBook();
                found = true;
                break;
            }
        }
        if (!found) {
            std::cout << "\nBook with ID " << id << " not found!" << std::endl;
        }
    }

    void modifyBookRecord() {
        std::string id = getBookIdFromUser();
        bool found = false;

        for (auto it = bookList.begin(); it != bookList.end(); ++it) {
            if (id == it->getBookId()) {
                std::cout << "Current details:" << std::endl;
                it->showBook();
                std::cout << "\n--- Enter NEW details ---" << std::endl;
                it->createBook();
                std::cout << "\nBook record updated in memory successfully." << std::endl;
                found = true;
                break;
            }
        }
        if (!found) {
            std::cout << "\nRecord not found!" << std::endl;
        }
    }

    void deleteBookRecord() {
        std::string id = getBookIdFromUser();
        bool found = false;

        auto new_end = std::remove_if(bookList.begin(), bookList.end(), 
            [&id](const Book& book) {
                return id == book.getBookId(); 
            });

        if (new_end != bookList.end()) {
            bookList.erase(new_end, bookList.end());
            std::cout << "\nRecord deleted from memory successfully." << std::endl;
            found = true;
        }

        if (!found) {
            std::cout << "\nRecord not found!" << std::endl;
        }
    }
    

    void sortBooksByTitle() {
        std::sort(bookList.begin(), bookList.end(), 
            [](const Book& a, const Book& b) {
                return std::strcmp(a.getTitle(), b.getTitle()) < 0; 
            });
        std::cout << "\nLibrary sorted alphabetically by Title." << std::endl;
    }
    
};


void displayMenu() {
    std::cout << "\n\n======== LIBRARY MANAGEMENT SYSTEM (V3) ========" << std::endl;
    std::cout << "1. Add New Book" << std::endl;
    std::cout << "2. Display All Books" << std::endl;
    std::cout << "3. Search Book by ID" << std::endl;
    std::cout << "4. Modify Book Details" << std::endl;
    std::cout << "5. Delete Book Record" << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    std::cout << "6. Sort Books by Title (NEW!)" << std::endl;
    std::cout << "8. EXIT & SAVE Data" << std::endl;
    std::cout << "=========================================" << std::endl;
    std::cout << "Enter your choice: ";
}

int main() {
    LibraryManager manager;
    manager.loadData();

    int choice;
    do {
        displayMenu();
        if (!(std::cin >> choice)) {
            std::cout << "\nInvalid input! Please enter a number." << std::endl;
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        switch(choice) {
            case 1: manager.addBookRecord(); break;
            case 2: manager.displayAllBooks(); break;
            case 3: manager.searchBook(); break;
            case 4: manager.modifyBookRecord(); break;
            case 5: manager.deleteBookRecord(); break;
            case 6: manager.sortBooksByTitle(); break;
            case 8: manager.saveData(); std::cout << "\nExiting Library System. Goodbye!" << std::endl; break;
            default: std::cout << "\nInvalid choice. Please try again." << std::endl;
        }
    } while (choice != 8);

    return 0;
}