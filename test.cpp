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
public:
    Book book;
    Member member;
    string dueDate;

    Borrow(Book book, Member member, string dueDate)
        : book(book), member(member), dueDate(dueDate) {}

    void displayBorrowDetails() const {
        cout << "Book: " << book.name << " | Borrowed by: " << member.getName()
             << " | Due Date: " << dueDate << endl;
    }
};

void displayMenu() {
    cout << "Library System Menu :-)\n";
    cout << "1. Login as Member\n";
    cout << "2. Login as Book Keeper\n";
    cout << "3. Exit\n";
    cout << "Enter your choice: ";
}

// Function for member login
void memberLogin() {
    string id, password;
    cout << "Enter Member ID: ";
    cin >> id;
    cout << "Enter Member Password: ";
    cin >> password;

    // Simple check for member login (could be expanded with a database)
    if (id == "member" && password == "memberpass") {
        cout << "Welcome, Member!" << endl;
    } else {
        cout << "Invalid login credentials. Try again!" << endl;
    }
}
// Function to display the menu for member options
void memberMenu() {
    cout << "\nMember Menu:\n";
    cout << "1. View Books by Genre\n";
    cout << "2. Borrow Book\n";
    cout << "3. Return Book\n";
    cout << "4. Exit Member Menu\n";
    cout << "Enter your choice: ";
}

// Function to display genres available
void displayGenres() {
    cout << "\nAvailable Genres:\n";
    cout << "1. Fiction\n";
    cout << "2. Science\n";
    cout << "3. History\n";
    cout << "4. Horror\n";
    cout << "5. Action\n";
    cout << "6. Thriller\n";
    cout << "7. Romance\n";
    cout << "8. Exit\n";
}

// Function to display books by genre
void viewBooksByGenre(const vector<Book>& books, const string& genre) {
    cout << "\nBooks in " << genre << " genre:\n";
    bool found = false;
    for (const auto& book : books) {
        if (book.genre == genre && !book.isBorrowed) {
            cout << "Title: " << book.title << "\n";
            found = true;
        }
    }
    if (!found) {
        cout << "No books available in this genre right now.\n";
    }
}

// Function to borrow a book
void borrowBook(vector<Book>& books) {
    string bookTitle;
    cout << "Enter the title of the book you want to borrow: ";
    cin.ignore(); // to clear the newline character from the buffer
    getline(cin, bookTitle);

    for (auto& book : books) {
        if (book.title == bookTitle && !book.isBorrowed) {
            book.isBorrowed = true;
            cout << "You have successfully borrowed the book: " << book.title << "\n";
            return;
        }
    }
    cout << "The book is either unavailable or already borrowed.\n";
}

// Function to return a borrowed book
void returnBook(vector<Book>& books) {
    string bookTitle;
    cout << "Enter the title of the book you want to return: ";
    cin.ignore(); // to clear the newline character from the buffer
    getline(cin, bookTitle);

    for (auto& book : books) {
        if (book.title == bookTitle && book.isBorrowed) {
            book.isBorrowed = false;
            cout << "You have successfully returned the book: " << book.title << "\n";
            return;
        }
    }
    cout << "The book was not borrowed or does not exist.\n";
}

// Function to handle member login and actions
void memberLogin(vector<Book>& books) {
    int memberChoice;
    while (true) {
        memberMenu();
        cin >> memberChoice;

        switch (memberChoice) {
            case 1: {
                int genreChoice;
                displayGenres();
                cin >> genreChoice;

                switch (genreChoice) {
                    case 1:
                        viewBooksByGenre(books, "Fiction");
                        break;
                    case 2:
                        viewBooksByGenre(books, "Science");
                        break;
                    case 3:
                        viewBooksByGenre(books, "History");
                        break;
                    case 4:
                        viewBooksByGenre(books, "Horror");
                        break;
                    case 5:
                        viewBooksByGenre(books, "Action");
                        break;
                    case 6:
                        viewBooksByGenre(books, "Thriller");
                        break;
                    case 7:
                        viewBooksByGenre(books, "Romance");
                        break;
                    case 8:
                        cout << "Exiting...\n";
                        break;
                    default:
                        cout << "Invalid choice! Please try again.\n";
                }
                break;
            }
            case 2:
                borrowBook(books);
                break;
            case 3:
                returnBook(books);
                break;
            case 4:
                cout << "Exiting Member Menu.\n";
                return;  // Exit member menu and go back to the main menu
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    }
}


// Function for Book Keeper login
void booKeeperLogin() {
    string id, password;
    cout << "Enter Book Keeper ID: ";
    cin >> id;
    cout << "Enter Book Keeper Password: ";
    cin >> password;

    // Simple check for Book Keeper login (could be expanded with a database)
    if (id == "librarian" && password == "librarianpass") {
        cout << "Welcome, Book Keeper!" << endl;
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
        booKeeperLogin();
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
    
    cout << "Goodbye!" << endl;
    return 0;
}
