#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

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
        cout << "ID: " << book_id << " | " << title << " by " << author
             << " | Copies: " << copies << endl;
    }
};

// Library Class (Manages Books)
class Library {
private:
    vector<Book> books;

public:
    void addBook() {
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
        books.push_back(Book(id, title, author, copies));
        cout << "Book added successfully!\n";
    }

    void viewBooks() const {
        if (books.empty()) {
            cout << "\nNo books available in the library.\n";
            return;
        }
        cout << "\nLibrary Books:\n";
        for (const auto& book : books) {
            book.displayDetails();
        }
    }

    bool borrowBook(string book_id) {
        for (auto& book : books) {
            if (book.book_id == book_id && book.copies > 0) {
                book.copies--;
                return true;
            }
        }
        return false;
    }

    bool returnBook(string book_id) {
        for (auto& book : books) {
            if (book.book_id == book_id) {
                book.copies++;
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
};

// Member Class
class Member : public Person {
private:
    vector<string> booksBorrowed;

public:
    Member(string name, string password) : Person(name, password) {}

    bool borrowBook(string book_id) {
        if (booksBorrowed.size() < 5) {
            booksBorrowed.push_back(book_id);
            return true;
        }
        return false;
    }

    bool returnBook(string book_id) {
        auto it = find(booksBorrowed.begin(), booksBorrowed.end(), book_id);
        if (it != booksBorrowed.end()) {
            booksBorrowed.erase(it);
            return true;
        }
        return false;
    }
};

// Main Function
int main() {
    Library library;
    BookKeeper keeper("Alice", "admin123");
    Member member("Bob", "pass456");

    int choice;
    do {
        cout << "\nLibrary Management System";
        cout << "\n1. Add Book";
        cout << "\n2. View Books";
        cout << "\n3. Borrow Book";
        cout << "\n4. Return Book";
        cout << "\n5. Exit";
        cout << "\nEnter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                library.addBook();
                break;
            case 2:
                library.viewBooks();
                break;
            case 3: {
                string book_id;
                cout << "Enter Book ID to Borrow: ";
                cin >> book_id;
                if (member.borrowBook(book_id) && library.borrowBook(book_id))
                    cout << "Book borrowed successfully!\n";
                else
                    cout << "Failed to borrow book. Either it is unavailable or limit reached.\n";
                break;
            }
            case 4: {
                string book_id;
                cout << "Enter Book ID to Return: ";
                cin >> book_id;
                if (member.returnBook(book_id) && library.returnBook(book_id))
                    cout << "Book returned successfully!\n";
                else
                    cout << "Failed to return book. You may not have borrowed it.\n";
                break;
            }
            case 5:
                cout << "Exiting system...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);

    return 0;
}
