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
        if (!file.is_open()) return;

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

// Function to Load Members from CSV
vector<Member> loadMembersFromCSV() {
    vector<Member> members;
    ifstream file(MEMBERS_FILE);
    if (!file.is_open()) return members;

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
    return members;
}

// Function to Save Members to CSV
void saveMembersToCSV(const vector<Member> &members) {
    ofstream file(MEMBERS_FILE);
    for (const auto &member : members) {
        file << member.member_id << "," << member.getName() << "," << member.getPassword() << "," << member.address << "\n";
    }
    file.close();
}

// Main Function
int main() {
    Library library;
    BookKeeper keeper;
    vector<Member> members = loadMembersFromCSV();

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
            saveMembersToCSV(members);
            cout << "Member registered successfully!\n";
        } else if (choice == 3) {
            string id, pass;
            cout << "Enter Member ID: "; cin >> id;
            cout << "Enter Password: "; cin >> pass;

            bool loggedIn = false;
            for (auto &member : members) {
                if (member.member_id == id && member.getPassword() == pass) {
                    member.memberMenu(library);
                    loggedIn = true;
                    break;
                }
            }
            if (!loggedIn) cout << "Invalid ID or Password!\n";
        }
    } while (choice != 4);

    return 0;
}