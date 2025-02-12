#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>

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
    string book_id, name, ISBN, author, genre;
    int year, copiesAvailable, copiesBorrowed;
    vector<string> reviews;

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

    void addReview(const string &review) {
        reviews.push_back(review);
    }

    void displayReviews() const {
        if (reviews.empty()) {
            cout << "No reviews available for this book.\n";
        } else {
            cout << "Reviews for " << name << ":\n";
            for (const auto &review : reviews) {
                cout << "- " << review << endl;
            }
        }
    }
};

// Library Class
class Library {
private:
    vector<Book> books;

public:
    void addBook(const Book& book) {
        books.push_back(book);
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

// Function for librarian login
void librarianLogin() {
    string username, password;
    cout << "Enter Librarian Username: ";
    cin >> username;
    cout << "Enter Librarian Password: ";
    cin >> password;

    // Simple check for librarian login (could be expanded with a database)
    if (username == "librarian" && password == "librarianpass") {
        cout << "Welcome, Librarian!" << endl;
    } else {
        cout << "Invalid login credentials. Try again!" << endl;
    }
}

// Function for handling user input and actions
void handleLoginChoice(int choice) {
    switch (choice) {
        case 1:
            memberLogin();
            break;
        case 2:
            librarianLogin();
            break;
        case 3:
            cout << "Exiting the system. Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice! Please enter a valid option." << endl;
            break;
    }
}


int main() {
    Library library;
    BookKeeper keeper("1", "Alice", "admin123");
    vector<Member> members;

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
                cout << "BookKeeper logged in successfully." << endl;
            } else {
                cout << "Invalid credentials." << endl;
            }
        } else if (userType == 2) {
            int subChoice;
            cout << "\n1. Register as Member";
            cout << "\n2. Login as Member";
            cout << "\nEnter choice: ";
            cin >> subChoice;
            cin.ignore();

            if (subChoice == 1) {
                string id, name, password, address;
                cout << "Enter ID: ";
                cin >> id;
                cin.ignore();
                cout << "Enter Name: ";
                getline(cin, name);
                cout << "Enter Password: ";
                cin >> password;
                cout << "Enter Address: ";
                cin.ignore();
                getline(cin, address);
                members.push_back(Member(id, name, password, address));
                cout << "Registration successful!" << endl;
            } else if (subChoice == 2) {
                string id, password;
                cout << "Enter ID: ";
                cin >> id;
                cout << "Enter Password: ";
                cin >> password;
                for (auto &member : members) {
                    if (member.getId() == id && member.validatePassword(password)) {
                        cout << "Member logged in successfully." << endl;
                        break;
                    }
                }
                cout << "Invalid credentials." << endl;
            }
        }
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
}
