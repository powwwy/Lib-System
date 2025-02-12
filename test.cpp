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
    vector<Person> users;
    vector<string> logs;

public:
    void addUser(const Person &user) {
        users.push_back(user);
    }

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
        logs.push_back("Book added: " + name);
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

    void viewUsers() const {
        if (users.empty()) {
            cout << "No users registered.\n";
            return;
        }
        cout << "\nRegistered Users:\n";
        for (const auto &user : users) {
            cout << "ID: " << user.getId() << " | Name: " << user.getName() << endl;
        }
    }

    void viewLogs() const {
        if (logs.empty()) {
            cout << "No system logs available.\n";
            return;
        }
        cout << "\nSystem Logs:\n";
        for (const auto &log : logs) {
            cout << log << endl;
        }
    }
};

// BookKeeper Class
class BookKeeper : public Person {
public:
    BookKeeper(string id, string name, string password) : Person(id, name, password) {}
};

// Function for librarian login
template <typename T>
T* login(vector<T> &users, string role) {
    string id, password;
    cout << "Enter " << role << " ID: ";
    cin >> id;
    cout << "Enter Password: ";
    cin >> password;

    for (auto &user : users) {
        if (user.getId() == id && user.validatePassword(password)) {
            cout << "Login successful!\n";
            return &user;
        }
    }
    cout << "Invalid credentials!\n";
    return nullptr;
}

void librarianMenu(BookKeeper &keeper, Library &library) {
    int choice;
    do {
        cout << "\nBookKeeper Menu:";
        cout << "\n1. Add Book";
        cout << "\n2. View Books";
        cout << "\n3. View System Logs";
        cout << "\n4. View Users";
        cout << "\n5. Logout";
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
            library.viewLogs();
            break;
        case 4:
            library.viewUsers();
            break;
        case 5:
            cout << "Logging out...\n";
            return;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    } while (true);
}

int main() {
    Library library;
    vector<BookKeeper> bookkeepers = {BookKeeper("1001", "Myrk", "iamadmin")};
    vector<Person> members = {Person("2001", "Alice", "mypassword"), Person("2002", "Bob", "1234")};

    for (auto &member : members) {
        library.addUser(member);
    }

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
        case 1: {
            Person *member = login(members, "Member");
            if (member) {
                cout << "Welcome, " << member->getName() << "!\n";
            }
            break;
        }
        case 2: {
            BookKeeper *keeper = login(bookkeepers, "Book Keeper");
            if (keeper) {
                librarianMenu(*keeper, library);
            }
            break;
        }
        case 3:
            cout << "Exiting the system. Goodbye!\n";
            return 0;
        default:
            cout << "Invalid choice! Try again.\n";
        }
    } while (true);
}
