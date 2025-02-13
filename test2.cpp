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
    string getPassword() const { return password; }
};

// Book Class
class Book {
public:
    string book_id, title, author;
    int ISBN, year, copiesAvailable, copiesBorrowed;

    Book(string id, string title, string author, int ISBN, int year, int copiesAvailable, int copiesBorrowed)
        : book_id(id), title(title), author(author), ISBN(ISBN), year(year),
          copiesAvailable(copiesAvailable), copiesBorrowed(copiesBorrowed) {}

    void displayDetails() const {
        cout << "| " << setw(4) << book_id
             << " | " << setw(22) << title
             << " | " << setw(18) << author
             << " | " << setw(10) << ISBN
             << " | " << setw(6) << year
             << " | " << setw(6) << copiesAvailable
             << " | " << setw(6) << copiesBorrowed << " |" << endl;
    }
};

// Library Class
class Library {
private:
    vector<Book> books;

public:
    Library() { loadBooksFromCSV(); }

    void loadBooksFromCSV() {
        ifstream file(BOOKS_FILE);
        if (!file.is_open()) return;

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string id, title, author, ISBNStr, yearStr, availableStr, borrowedStr;
            getline(ss, id, ',');
            getline(ss, title, ',');
            getline(ss, author, ',');
            getline(ss, ISBNStr, ',');
            getline(ss, yearStr, ',');
            getline(ss, availableStr, ',');
            getline(ss, borrowedStr, ',');

            try {
                int ISBN = stoi(ISBNStr);
                int year = stoi(yearStr);
                int copiesAvailable = stoi(availableStr);
                int copiesBorrowed = stoi(borrowedStr);
                books.push_back(Book(id, title, author, ISBN, year, copiesAvailable, copiesBorrowed));
            } catch (invalid_argument &e) {
                cout << "Error reading book data: " << line << endl;
            }
        }
        file.close();
    }

    void saveBooksToCSV() {
        ofstream file(BOOKS_FILE);
        for (const auto &book : books) {
            file << book.book_id << "," << book.title << "," << book.author << ","
                 << book.ISBN << "," << book.year << ","
                 << book.copiesAvailable << "," << book.copiesBorrowed << "\n";
        }
        file.close();
    }

    void addBook() {
        string id, title, author;
        int ISBN, year, copiesAvailable;

        cout << "\nEnter Book ID: ";
        cin >> id;
        cin.ignore();
        cout << "Enter Book Title: ";
        getline(cin, title);
        cout << "Enter Author: ";
        getline(cin, author);
        cout << "Enter ISBN: ";
        cin >> ISBN;
        cout << "Enter Year: ";
        cin >> year;
        cout << "Enter Available Copies: ";
        cin >> copiesAvailable;

        books.push_back(Book(id, title, author, ISBN, year, copiesAvailable, 0));
        saveBooksToCSV();
        cout << "Book added successfully!\n";
    }

    void viewBooks() const {
        if (books.empty()) {
            cout << "\nNo books available in the library.\n";
            return;
        }

        cout << "\n---------------------------------------------------------------------------------\n";
        cout << "| ID  | Title                  | Author              | ISBN       | Year  | Avail | Borrowed |\n";
        cout << "---------------------------------------------------------------------------------\n";
        for (const auto &book : books) {
            book.displayDetails();
        }
        cout << "---------------------------------------------------------------------------------\n";
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
            if (book.book_id == book_id && book.copiesAvailable > 0) {
                book.copiesAvailable--;
                book.copiesBorrowed++;
                saveBooksToCSV();
                return true;
            }
        }
        return false;
    }

    bool returnBook(string book_id) {
        for (auto &book : books) {
            if (book.book_id == book_id && book.copiesBorrowed > 0) {
                book.copiesAvailable++;
                book.copiesBorrowed--;
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
    BookKeeper() : Person("admin", "admin123") {}

    void bookKeeperMenu(Library &library) {
        int choice;
        do {
            cout << "\nBookKeeper Menu";
            cout << "\n1. Add Book";
            cout << "\n2. View Books";
            cout << "\n3. Remove Book";
            cout << "\n4. Logout";
            cout << "\nEnter choice: ";
            cin >> choice;

            switch (choice) {
                case 1: library.addBook(); break;
                case 2: library.viewBooks(); break;
                case 3: {
                    string book_id;
                    cout << "Enter Book ID to Remove: ";
                    cin >> book_id;
                    if (library.removeBook(book_id))
                        cout << "Book removed successfully!\n";
                    else
                        cout << "Book not found!\n";
                    break;
                }
                case 4: cout << "Logging out...\n"; break;
                default: cout << "Invalid choice. Try again.\n";
            }
        } while (choice != 4);
    }
};

// Member Class
class Member : public Person {
public:
    string member_id, address;

    Member(string id, string name, string password, string address)
        : Person(name, password), member_id(id), address(address) {}

    void memberMenu(Library &library) {
        int choice;
        do {
            cout << "\nMember Menu";
            cout << "\n1. View Books";
            cout << "\n2. Borrow Book";
            cout << "\n3. Return Book";
            cout << "\n4. Logout";
            cout << "\nEnter choice: ";
            cin >> choice;

            switch (choice) {
                case 1: library.viewBooks(); break;
                case 2: {
                    string book_id;
                    cout << "Enter Book ID to Borrow: ";
                    cin >> book_id;
                    if (library.borrowBook(book_id))
                        cout << "Book borrowed successfully!\n";
                    else
                        cout << "Book not available!\n";
                    break;
                }
                case 3: {
                    string book_id;
                    cout << "Enter Book ID to Return: ";
                    cin >> book_id;
                    if (library.returnBook(book_id))
                        cout << "Book returned successfully!\n";
                    else
                        cout << "You didn't borrow this book!\n";
                    break;
                }
                case 4: cout << "Logging out...\n"; break;
                default: cout << "Invalid choice. Try again.\n";
            }
        } while (choice != 4);
    }
};

// Main Function
int main() {
    Library library;
    BookKeeper keeper;
    vector<Member> members;

    int choice;
    do {
        cout << "\nLibrary System Login";
        cout << "\n1. Login as BookKeeper";
        cout << "\n2. Register as Member";
        cout << "\n3. Login as Member";
        cout << "\n4. Exit";
        cout << "\nEnter choice: ";
        cin >> choice;

        if (choice == 1) {
            string pass;
            cout << "Enter BookKeeper Password: ";
            cin >> pass;
            if (pass == keeper.getPassword()) keeper.bookKeeperMenu(library);
            else cout << "Incorrect Password!\n";
        } else if (choice == 2) {
            string id, name, pass, addr;
            cout << "Enter ID: "; cin >> id;
            cout << "Enter Name: "; cin >> name;
            cout << "Enter Password: "; cin >> pass;
            cout << "Enter Address: "; cin >> addr;
            members.push_back(Member(id, name, pass, addr));
            cout << "Member registered successfully!\n";
        } else if (choice == 3 && !members.empty()) {
            members[0].memberMenu(library);
        }
    } while (choice != 4);

    return 0;
}
