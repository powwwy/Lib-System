#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

// Base Class: Person
class Person {
protected:
    string id, name, password;

public:
    Person(string id, string name, string password) : id(id), name(name), password(password) {}
    string getId() const { return id; }
    string getName() const { return name; }
    bool validatePassword(string input) const { return password == input; }
};

// Book Class
class Book {
public:
    string book_id, name, ISBN, author, genre;
    int year, copiesAvailable, copiesBorrowed;

    Book(string id, string name, string isbn, string author, int year, string genre, int copiesAvailable)
        : book_id(id), name(name), ISBN(isbn), author(author), year(year), genre(genre),
          copiesAvailable(copiesAvailable), copiesBorrowed(0) {}

    void displayDetails() const {
        cout << "ID: " << book_id << " | " << name << " by " << author
             << " | Year: " << year << " | Genre: " << genre
             << " | ISBN: " << ISBN
             << " | Available Copies: " << copiesAvailable
             << " | Borrowed Copies: " << copiesBorrowed << endl;
    }
};

// Library Class
class Library {
private:
    vector<Book> books;

public:
    void addBook() {
        string id, name, isbn, author, genre;
        int year, copiesAvailable;

        cout << "Enter Book ID: ";
        cin >> id;
        cin.ignore();

        cout << "Enter Book Name: ";
        getline(cin, name);

        cout << "Enter Author: ";
        getline(cin, author);

        cout << "Enter Genre: ";
        getline(cin, genre);

        cout << "Enter Year: ";
        cin >> year;

        cout << "Enter ISBN: ";
        cin >> isbn;

        cout << "Enter Available Copies: ";
        cin >> copiesAvailable;
        cin.ignore();

        books.push_back(Book(id, name, isbn, author, year, genre, copiesAvailable));
        cout << "Book added successfully!\n";
    }

    void viewBooks() const {
        if (books.empty()) {
            cout << "\nNo books available in the library.\n";
            return;
        }
        cout << "\nLibrary Books:\n";
        for (const auto &book : books) {
            book.displayDetails();
        }
    }

    void viewBooksByGenre(const string &genre) const {
        cout << "\nBooks in " << genre << " genre:\n";
        bool found = false;
        for (const auto &book : books) {
            if (book.genre == genre && book.copiesAvailable > 0) {
                book.displayDetails();
                found = true;
            }
        }
        if (!found) {
            cout << "No books available in this genre right now.\n";
        }
    }

    void borrowBook(const string &bookTitle) {
        for (auto &book : books) {
            if (book.name == bookTitle && book.copiesAvailable > 0) {
                book.copiesAvailable--;
                book.copiesBorrowed++;
                cout << "You have successfully borrowed the book: " << book.name << "\n";
                return;
            }
        }
        cout << "The book is either unavailable or already borrowed.\n";
    }

    void returnBook(const string &bookTitle) {
        for (auto &book : books) {
            if (book.name == bookTitle && book.copiesBorrowed > 0) {
                book.copiesAvailable++;
                book.copiesBorrowed--;
                cout << "You have successfully returned the book: " << book.name << "\n";
                return;
            }
        }
        cout << "The book was not borrowed or does not exist.\n";
    }
};

// BookKeeper Class
class BookKeeper : public Person {
public:
    BookKeeper(string id, string name, string password) : Person(id, name, password) {}
};

// Function for librarian login
void librarianLogin(BookKeeper &keeper, Library &library) {
    int choice;
    do {
        cout << "\nBookKeeper Menu:";
        cout << "\n1. Add Book";
        cout << "\n2. View Books";
        cout << "\n3. Logout";
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
        case 3:
            cout << "Logging out...\n";
            return;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 3);
}

// Function for member login
void memberLogin(Library &library) {
    int choice;
    do {
        cout << "\nMember Menu:";
        cout << "\n1. View Books by Genre";
        cout << "\n2. Borrow Book";
        cout << "\n3. Return Book";
        cout << "\n4. Logout";
        cout << "\nEnter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1: {
            string genre;
            cout << "Enter genre: ";
            getline(cin, genre);
            library.viewBooksByGenre(genre);
            break;
        }
        case 2: {
            string bookTitle;
            cout << "Enter book title to borrow: ";
            getline(cin, bookTitle);
            library.borrowBook(bookTitle);
            break;
        }
        case 3: {
            string bookTitle;
            cout << "Enter book title to return: ";
            getline(cin, bookTitle);
            library.returnBook(bookTitle);
            break;
        }
        case 4:
            cout << "Logging out...\n";
            return;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 4);
}

// Main function
int main() {
    Library library;
    BookKeeper keeper("1001", "Myrk", "iamadmin");
    int choice;

    do {
        cout << "\nLibrary System Menu:";
        cout << "\n1. Login as Member";
        cout << "\n2. Login as Book Keeper";
        cout << "\n3. Exit";
        cout << "\nEnter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            memberLogin(library);
            break;
        case 2:
            librarianLogin(keeper, library);
            break;
        case 3:
            cout << "Exiting the system. Goodbye!\n";
            return 0;
        default:
            cout << "Invalid choice! Try again.\n";
        }
    } while (true);
}
