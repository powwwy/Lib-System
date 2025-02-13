#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <map>

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
        
        cout << "\n----------------------------------------------" << endl;
        cout << "| ID  | Title                  | Author              | Copies |" << endl;
        cout << "----------------------------------------------" << endl;
        for (const auto &book : books) {
            book.displayDetails();
        }
        cout << "----------------------------------------------" << endl;
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

// Member Class
class Member : public Person {
private:
    string member_id, address, commonGenre;
    vector<string> booksBorrowed;
    int booksOverdue;

public:
    Member(string id, string name, string password, string address)
        : Person(name, password), member_id(id), address(address), booksOverdue(0) {}

    void displayMemberInfo() const {
        cout << "\nMember ID: " << member_id
             << "\nName: " << name
             << "\nAddress: " << address
             << "\nBooks Overdue: " << booksOverdue
             << "\nCommon Genre: " << (commonGenre.empty() ? "N/A" : commonGenre)
             << "\nBooks Borrowed: ";
        if (booksBorrowed.empty()) cout << "None\n";
        else {
            for (const auto &book : booksBorrowed) cout << book << " ";
            cout << endl;
        }
    }

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
    vector<Member> members;

    // Load members from CSV
    ifstream file(MEMBERS_FILE);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string id, name, password, address;
            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, password, ',');
            getline(ss, address, ',');
            members.push_back(Member(id, name, password, address));
        }
        file.close();
    }

    int choice;
    do {
        cout << "\nLibrary Management System";
        cout << "\n1. Register Member";
        cout << "\n2. View Books";
        cout << "\n3. Borrow Book";
        cout << "\n4. Return Book";
        cout << "\n5. View Member Info";
        cout << "\n6. Delete Account";
        cout << "\n7. Exit";
        cout << "\nEnter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: {
                string id, name, password, address;
                cout << "Enter Member ID: ";
                cin >> id;
                cin.ignore();
                cout << "Enter Name: ";
                getline(cin, name);
                cout << "Enter Password: ";
                getline(cin, password);
                cout << "Enter Address: ";
                getline(cin, address);
                members.push_back(Member(id, name, password, address));

                // Save member to CSV
                ofstream file(MEMBERS_FILE, ios::app);
                file << id << "," << name << "," << password << "," << address << "\n";
                file.close();
                cout << "Member registered successfully!\n";
                break;
            }
            case 2:
                library.viewBooks();
                break;
            case 3: {
                string book_id;
                cout << "Enter Book ID to Borrow: ";
                cin >> book_id;
                if (members[0].borrow(library, book_id))  // Using first member for now
                    cout << "Book borrowed successfully!\n";
                else
                    cout << "Failed to borrow book. Either it is unavailable or limit reached.\n";
                break;
            }
            case 4: {
                string book_id;
                cout << "Enter Book ID to Return: ";
                cin >> book_id;
                if (members[0].returnBook(library, book_id))
                    cout << "Book returned successfully!\n";
                else
                    cout << "Failed to return book. You may not have borrowed it.\n";
                break;
            }
            case 5:
                members[0].displayMemberInfo();
                break;
            case 6:
                members[0].deleteAccount();
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



