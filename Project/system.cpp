#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

const string BOOKS_FILE = "books.csv";
const string MEMBERS_FILE = "members.csv";

// Base Class: Person
class Person {
protected:
    string name, password;

public:
    Person(string name, string password) : name(name), password(password) {}
    string getName() const { return name; }
};

// Book Class
class Book {
public:
    string book_id, title, author;
    int copies;

    Book(string id, string title, string author, int copies)
        : book_id(id), title(title), author(author), copies(copies) {}

    void displayDetails() const {
        cout << "| " << setw(4) << book_id
             << " | " << setw(22) << title
             << " | " << setw(18) << author
             << " | " << setw(6) << copies << " |" << endl;
    }
};

// Library Class (Manages Books)
class Library {
private:
    vector<Book> books;

public:
    Library() { loadBooksFromCSV(); }

    void loadBooksFromCSV() {
        ifstream file(BOOKS_FILE);
        if (!file.is_open()) {
            cout << "No existing book data found.\n";
            return;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string id, title, author, copiesStr;
            getline(ss, id, ',');
            getline(ss, title, ',');
            getline(ss, author, ',');
            getline(ss, copiesStr, ',');
            int copies = stoi(copiesStr);
            books.push_back(Book(id, title, author, copies));
        }
        file.close();
    }

    void saveBooksToCSV() {
        ofstream file(BOOKS_FILE);
        for (const auto &book : books) {
            file << book.book_id << "," << book.title << "," << book.author << "," << book.copies << "\n";
        }
        file.close();
    }

    void addBook(string id, string title, string author, int copies) {
        books.push_back(Book(id, title, author, copies));
        saveBooksToCSV();
        cout << "Book added successfully!\n";
    }

    void viewBooks() const {
        if (books.empty()) {
            cout << "\nNo books available in the library.\n";
            return;
        }
        
        cout << "\n----------------------------------------------" << endl;
        cout << "| ID  | Title                  | Author              | Copies |" << endl;
        cout << "----------------------------------------------" << endl;
        for (const auto &book : books) {
            book.displayDetails();
        }
        cout << "----------------------------------------------" << endl;
    }

    bool removeBook(string book_id) {
        auto it = remove_if(books.begin(), books.end(), [&](Book &b) { return b.book_id == book_id; });
        if (it != books.end()) {
            books.erase(it);
            saveBooksToCSV();
            return true;
        }
        return false;
    }

    bool borrowBook(string book_id) {
        for (auto &book : books) {
            if (book.book_id == book_id && book.copies > 0) {
                book.copies--;
                saveBooksToCSV();
                return true;
            }
        }
        return false;
    }

    bool returnBook(string book_id) {
        for (auto &book : books) {
            if (book.book_id == book_id) {
                book.copies++;
                saveBooksToCSV();
                return true;
            }
        }
        return false;
    }
};

// BookKeeper Class
class BookKeeper : public Person {
public:
    BookKeeper(string name, string password) : Person(name, password) {}

    void addBooks(Library &library) {
        string id, title, author;
        int copies;
        cout << "\nEnter Book ID: ";
        cin >> id;
        cin.ignore();
        cout << "Enter Book Title: ";
        getline(cin, title);
        cout << "Enter Author: ";
        getline(cin, author);
        cout << "Enter Number of Copies: ";
        cin >> copies;
        library.addBook(id, title, author, copies);
    }

    void viewSystem(Library &library) {
        library.viewBooks();
    }

    void removeBook(Library &library) {
        string book_id;
        cout << "Enter Book ID to Remove: ";
        cin >> book_id;
        if (library.removeBook(book_id))
            cout << "Book removed successfully!\n";
        else
            cout << "Book not found!\n";
    }
};

// Member Class
class Member : public Person {
private:
    string member_id, address, commonGenre, review;
    vector<string> booksBorrowed;
    int booksOverdue;

public:
    Member(string id, string name, string password, string address)
        : Person(name, password), member_id(id), address(address), booksOverdue(0) {}

    bool borrow(Library &library, string book_id) {
        if (booksBorrowed.size() < 5 && library.borrowBook(book_id)) {
            booksBorrowed.push_back(book_id);
            return true;
        }
        return false;
    }

    bool returnBook(Library &library, string book_id) {
        if (library.returnBook(book_id)) {
            auto it = find(booksBorrowed.begin(), booksBorrowed.end(), book_id);
            if (it != booksBorrowed.end()) booksBorrowed.erase(it);
            return true;
        }
        return false;
    }

    void deleteAccount() {
        cout << "Member " << member_id << " has been deleted.\n";
        booksBorrowed.clear();
    }
};

// Main Function
int main() {
    Library library;
    BookKeeper keeper("Alice", "admin123");
    Member member("1001", "Bob", "pass456", "123 Library St");

    int choice;
    do {
        cout << "\nLibrary Management System";
        cout << "\n1. Add Book";
        cout << "\n2. View Books";
        cout << "\n3. Remove Book";
        cout << "\n4. Borrow Book";
        cout << "\n5. Return Book";
        cout << "\n6. Delete Member Account";
        cout << "\n7. Exit";
        cout << "\nEnter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                keeper.addBooks(library);
                break;
            case 2:
                keeper.viewSystem(library);
                break;
            case 3:
                keeper.removeBook(library);
                break;
            case 4: {
                string book_id;
                cout << "Enter Book ID to Borrow: ";
                cin >> book_id;
                if (member.borrow(library, book_id))
                    cout << "Book borrowed successfully!\n";
                else
                    cout << "Failed to borrow book. Either it is unavailable or limit reached.\n";
                break;
            }
            case 5: {
                string book_id;
                cout << "Enter Book ID to Return: ";
                cin >> book_id;
                if (member.returnBook(library, book_id))
                    cout << "Book returned successfully!\n";
                else
                    cout << "Failed to return book. You may not have borrowed it.\n";
                break;
            }
            case 6:
                member.deleteAccount();
                break;
            case 7:
                cout << "Exiting system...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 7);

    return 0;
}


