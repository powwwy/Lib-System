#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

const string FILE_NAME = "books.csv";

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
    Library() { loadBooksFromCSV(); }

    void loadBooksFromCSV() {
        ifstream file(FILE_NAME);
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
        ofstream file(FILE_NAME);
        for (const auto &book : books) {
            file << book.book_id << "," << book.title << "," << book.author << "," << book.copies << "\n";
        }
        file.close();
    }

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
        saveBooksToCSV();
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
};

// BookKeeper Class
class BookKeeper : public Person {
public:
    BookKeeper(string id, string name, string password) : Person(id, name, password) {}
};

// Member Class
class Member : public Person {
public:
    Member(string id, string name, string password) : Person(id, name, password) {}
};

vector<Member> members;

void memberMenu(Member &member, Library &library) {
    int choice;
    do {
        cout << "\nMember Menu:";
        cout << "\n1. View Books";
        cout << "\n2. Logout";
        cout << "\nEnter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                library.viewBooks();
                break;
            case 2:
                cout << "Logging out...\n";
                return;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 2);
}

void bookKeeperMenu(BookKeeper &keeper, Library &library) {
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

int main() {
    Library library;
    BookKeeper keeper("1", "Alice", "admin123");

    int userType;
    do {
        cout << "\nWelcome to Library Management System";
        cout << "\n1. BookKeeper Login";
        cout << "\n2. Member Login or Register";
        cout << "\n3. Exit";
        cout << "\nEnter choice: ";
        cin >> userType;
        cin.ignore();

        if (userType == 1) {
            string id, password;
            cout << "Enter ID: ";
            cin >> id;
            cout << "Enter Password: ";
            cin >> password;
            if (keeper.getId() == id && keeper.validatePassword(password)) {
                bookKeeperMenu(keeper, library);
            } else {
                cout << "Invalid credentials.\n";
            }
        } else if (userType == 2) {
            int subChoice;
            cout << "\n1. Register as Member";
            cout << "\n2. Login as Member";
            cout << "\nEnter choice: ";
            cin >> subChoice;
            cin.ignore();

            if (subChoice == 1) {
                string id, name, password;
                cout << "Enter ID: ";
                cin >> id;
                cin.ignore();
                cout << "Enter Name: ";
                getline(cin, name);
                cout << "Enter Password: ";
                cin >> password;
                members.push_back(Member(id, name, password));
                cout << "Registration successful!\n";
            } else if (subChoice == 2) {
                string id, password;
                cout << "Enter ID: ";
                cin >> id;
                cout << "Enter Password: ";
                cin >> password;
                for (auto &member : members) {
                    if (member.getId() == id && member.validatePassword(password)) {
                        memberMenu(member, library);
                        break;
                    }
                }
                cout << "Invalid credentials.\n";
            }
        }
    } while (userType != 3);
    cout << "Goodbye!\n";
    return 0;
}
