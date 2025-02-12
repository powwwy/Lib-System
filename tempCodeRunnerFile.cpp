#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

const string BOOKS_FILE = "books.csv";
const string MEMBERS_FILE = "members.csv";
const string BORROWED_FILE = "borrowed_books.csv";

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
    string book_id, title, ISBN, author, genre;
    int availableCopies, borrowedCopies, year;
    double score;

    Book(string id, string title, string ISBN, string author, string genre, int available, int borrowed, int year, double score)
        : book_id(id), title(title), ISBN(ISBN), author(author), genre(genre), availableCopies(available), borrowedCopies(borrowed), year(year), score(score) {}

    void displayDetails() const {
        cout << "| " << setw(4) << book_id
             << " | " << setw(22) << title
             << " | " << setw(13) << author
             << " | " << setw(6) << availableCopies
             << " | " << setw(6) << borrowedCopies
             << " | " << setw(4) << year
             << " | " << setw(4) << score << " |" << endl;
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
            string id, title, ISBN, author, genre, availableStr, borrowedStr, yearStr, scoreStr;
            getline(ss, id, ',');
            getline(ss, title, ',');
            getline(ss, ISBN, ',');
            getline(ss, author, ',');
            getline(ss, genre, ',');
            getline(ss, availableStr, ',');
            getline(ss, borrowedStr, ',');
            getline(ss, yearStr, ',');
            getline(ss, scoreStr, ',');

            int available = stoi(availableStr);
            int borrowed = stoi(borrowedStr);
            int year = stoi(yearStr);
            double score = stod(scoreStr);

            books.push_back(Book(id, title, ISBN, author, genre, available, borrowed, year, score));
        }
        file.close();
    }

    void saveBooksToCSV() {
        ofstream file(BOOKS_FILE);
        for (const auto &book : books) {
            file << book.book_id << "," << book.title << "," << book.ISBN << "," << book.author << ","
                 << book.genre << "," << book.availableCopies << "," << book.borrowedCopies << ","
                 << book.year << "," << book.score << "\n";
        }
        file.close();
    }

    void viewBooks() const {
        if (books.empty()) {
            cout << "\nNo books available in the library.\n";
            return;
        }

        cout << "\n-------------------------------------------------------------------" << endl;
        cout << "| ID  | Title                  | Author       | Avail | Brwd | Year | Rate |" << endl;
        cout << "-------------------------------------------------------------------" << endl;
        for (const auto &book : books) {
            book.displayDetails();
        }
        cout << "-------------------------------------------------------------------" << endl;
    }

    void addBook(Book book) {
        books.push_back(book);
        saveBooksToCSV();
        cout << "Book added successfully!\n";
    }

    void removeBook(string book_id) {
        books.erase(remove_if(books.begin(), books.end(), [&](Book &b) { return b.book_id == book_id; }), books.end());
        saveBooksToCSV();
        cout << "Book removed successfully!\n";
    }

    bool borrowBook(string book_id) {
        for (auto &book : books) {
            if (book.book_id == book_id && book.availableCopies > 0) {
                book.availableCopies--;
                book.borrowedCopies++;
                saveBooksToCSV();
                return true;
            }
        }
        return false;
    }

    bool returnBook(string book_id) {
        for (auto &book : books) {
            if (book.book_id == book_id) {
                book.availableCopies++;
                book.borrowedCopies--;
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

    void addBook(Library &library) {
        string id, title, ISBN, author, genre;
        int available, borrowed, year;
        double score;
        cout << "\nEnter Book ID: ";
        cin >> id;
        cin.ignore();
        cout << "Enter Title: ";
        getline(cin, title);
        cout << "Enter ISBN: ";
        cin >> ISBN;
        cin.ignore();
        cout << "Enter Author: ";
        getline(cin, author);
        cout << "Enter Genre: ";
        getline(cin, genre);
        cout << "Enter Available Copies: ";
        cin >> available;
        cout << "Enter Borrowed Copies: ";
        cin >> borrowed;
        cout << "Enter Year: ";
        cin >> year;
        cout << "Enter Rating (Score): ";
        cin >> score;

        library.addBook(Book(id, title, ISBN, author, genre, available, borrowed, year, score));
    }

    void removeBook(Library &library) {
        string book_id;
        cout << "Enter Book ID to Remove: ";
        cin >> book_id;
        library.removeBook(book_id);
    }

    void viewSystem(Library &library) {
        library.viewBooks();
    }
};

// Member Class
class Member : public Person {
private:
    vector<string> booksBorrowed;

public:
    Member(string name, string password) : Person(name, password) {}

    bool borrowBook(Library &library, string book_id) {
        if (booksBorrowed.size() < 5 && library.borrowBook(book_id)) {
            booksBorrowed.push_back(book_id);
            return true;
        }
        return false;
    }

    bool returnBook(Library &library, string book_id) {
        if (library.returnBook(book_id)) {
            booksBorrowed.erase(remove(booksBorrowed.begin(), booksBorrowed.end(), book_id), booksBorrowed.end());
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
        cout << "\n1. Add Book\n2. Remove Book\n3. View Books\n4. Borrow Book\n5. Return Book\n6. Exit\nEnter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: keeper.addBook(library); break;
            case 2: keeper.removeBook(library); break;
            case 3: library.viewBooks(); break;
            case 4: member.borrowBook(library, "1"); break;
            case 5: member.returnBook(library, "1"); break;
            case 6: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 6);

    return 0;
}
