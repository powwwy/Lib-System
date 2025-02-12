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

<<<<<<< HEAD
// Member Class
class Member : public Person {
private:
    string address;
    vector<string> booksBorrowed;
    vector<string> booksOverdue;
    map<string, int> genreCount;
    string commonGenre;
    vector<string> reviews;

    void updateCommonGenre() {
        int maxCount = 0;
        for (const auto& pair : genreCount) {
            if (pair.second > maxCount) {
                maxCount = pair.second;
                commonGenre = pair.first;
            }
        }
    }

public:
    Member(string id, string name, string password, string address)
        : Person(id, name, password), address(address) {}

    void borrowBook(string bookTitle, string genre) {
        booksBorrowed.push_back(bookTitle);
        genreCount[genre]++;
        updateCommonGenre();
    }

    void returnBook(string bookTitle) {
        auto it = find(booksBorrowed.begin(), booksBorrowed.end(), bookTitle);
        if (it != booksBorrowed.end()) {
            booksBorrowed.erase(it);
        }
    }

    void markOverdue(string bookTitle) {
        booksOverdue.push_back(bookTitle);
    }
};

// Borrow Class

class Borrow {
    private:
        static unordered_map<string, int> bookCount; // To track the number of copies borrowed per book
        static int totalBorrowedBooks; // To track the total number of books borrowed by a member
    
    public:
        Book book;
        Member member;
        string dueDate;
    
        Borrow(Book book, Member member, string dueDate)
            : book(book), member(member), dueDate(dueDate) {}
    
        bool borrowBook() {
            // Check if the member already borrowed 5 books
            if (totalBorrowedBooks >= 5) {
                cout << "Error: You have already borrowed the maximum number of books." << endl;
                return false;
            }
    
            // Check if the member has borrowed 3 copies of the same book
            if (bookCount[book.name] >= 3) {
                cout << "Error: You can only borrow a maximum of 3 copies of the same book." << endl;
                return false;
            }
    
            // If both conditions are passed, the book can be borrowed
            bookCount[book.name]++;
            totalBorrowedBooks++;
            cout << "Book: " << book.name << " | Borrowed by: " << member.getName()
                 << " | Due Date: " << dueDate << endl;
            return true;
        }
    };

void displayMenu() {
    cout << "Library System Menu :-)\n";
    cout << "1. Login as Member\n";
    cout << "2. Login as Librarian\n";
    cout << "3. Exit\n";
    cout << "Enter your choice: ";
}

// Function for member login
void memberLogin() {
    string username, password;
    cout << "Enter Member Username: ";
    cin >> username;
    cout << "Enter Member Password: ";
    cin >> password;

    // Simple check for member login (could be expanded with a database)
    if (username == "member" && password == "memberpass") {
        cout << "Welcome, Member!" << endl;
    } else {
        cout << "Invalid login credentials. Try again!" << endl;
    }
}

=======
>>>>>>> 2c8fcfcc264877be28bd65377a48469074479acd
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
<<<<<<< HEAD
    } while (userType != 3);

    Book book1("C++ Programming");
    Book book2("Data Structures");
    Member member1("Ayman");

    Borrow borrow1(book1, member1, "2025-03-01");
    borrow1.borrowBook(); // Successful borrow

    Borrow borrow2(book1, member1, "2025-03-05");
    borrow2.borrowBook(); // Successful borrow

    Borrow borrow3(book1, member1, "2025-03-10");
    borrow3.borrowBook(); // Successful borrow

    Borrow borrow4(book1, member1, "2025-03-15");
    borrow4.borrowBook();
    
    cout << "Goodbye!" << endl;
    return 0;
=======
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
>>>>>>> 2c8fcfcc264877be28bd65377a48469074479acd
}