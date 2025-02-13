#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <ctime>
#include <algorithm>
#include <iomanip> // For formatting output
using namespace std;

// Abstract Base Class for Library Entities
class LibraryEntity {
public:
    virtual void addEntity() = 0;
    virtual void removeEntity() = 0;
    virtual void displayDetails() const = 0;
    virtual ~LibraryEntity() {}
};

// Base Class: Person
class Person {
protected:
    string name;
    int id;
    string contact;

public:
    Person(string name, int id, string contact)
        : name(name), id(id), contact(contact) {}

    virtual void displayDetails() const {
        cout << "Name: " << name << "\nID: " << id << "\nContact: " << contact << endl;
    }

    virtual ~Person() {}
};

// Derived Class: Member
class Member : public Person, public LibraryEntity {
private:
    vector<int> borrowedBooks;
    map<int, string> bookReviews; // Store book reviews

public:
    Member(string name, int id, string contact)
        : Person(name, id, contact) {}

    void addEntity() override {
        cout << "Member added: " << name << endl;
    }

    void removeEntity() override {
        cout << "Member removed: " << name << endl;
    }

    void displayDetails() const override {
        Person::displayDetails();
        cout << "Borrowed Books: ";
        for (int book : borrowedBooks) cout << book << " ";
        cout << "\nBook Reviews:\n";
        for (const auto& review : bookReviews) {
            cout << "Book " << review.first << ": " << review.second << endl;
        }
    }

    void borrowBook(int bookID) {
        borrowedBooks.push_back(bookID);
    }

    void returnBook(int bookID) {
        auto it = find(borrowedBooks.begin(), borrowedBooks.end(), bookID);
        if (it != borrowedBooks.end()) {
            borrowedBooks.erase(it);
        }
    }

    void addReview(int bookID, const string& review) {
        bookReviews[bookID] = review;
    }

    vector<int> getBorrowedBooks() const {
        return borrowedBooks;
    }

    int getID() const {
        return id;
    }
    string getName()const{
        return name;
    }
    string getContact()const{
        return contact;
    }

    string toCSV() const {
        string borrowedBooksStr;
        for (int book : borrowedBooks) borrowedBooksStr += to_string(book) + ";";

        string reviewsStr;
        for (const auto& review : bookReviews) {
            reviewsStr += to_string(review.first) + ":" + review.second + ";";
        }

        return name + "," + to_string(id) + "," + contact + "," + borrowedBooksStr + "," + reviewsStr;
    }
};

// Book Class
class Book : public LibraryEntity {
private:
    int serialNumber;
    string title;
    string author;
    string ISBN;
    int year;
    int totalCopies;
    int copiesBorrowed;

public:
    Book(int serialNumber, string title, string author, string ISBN, int year, int totalCopies)
        : serialNumber(serialNumber), title(title), author(author), ISBN(ISBN), year(year), totalCopies(totalCopies), copiesBorrowed(0) {}

    void addEntity() override {
        cout << "Book added: " << title << endl;
    }

    void removeEntity() override {
        cout << "Book removed: " << title << endl;
    }

    void displayDetails() const override {
        cout << "Title: " << title << "\nAuthor: " << author << "\nISBN: " << ISBN
             << "\nYear: " << year << "\nTotal Copies: " << totalCopies
             << "\nCopies Available: " << (totalCopies - copiesBorrowed)
             << "\nCopies Borrowed: " << copiesBorrowed << endl;
    }

    bool borrowCopy(int numCopies) {
        if (copiesBorrowed + numCopies <= totalCopies) {
            copiesBorrowed += numCopies;
            return true;
        }
        return false;
    }

    void returnCopy(int numCopies) {
        if (copiesBorrowed >= numCopies) {
            copiesBorrowed -= numCopies;
        }
    }

    int getSerialNumber() const {
        return serialNumber;
    }

    string getTitle() const {
        return title;
    }
    string getAuthor()const{
        return author;
    }

    int getCopiesAvailable() const {
        return totalCopies - copiesBorrowed;
    }

    int getCopiesBorrowed() const {
        return copiesBorrowed;
    }

    string toCSV() const {
        return to_string(serialNumber) + "," + title + "," + author + "," + ISBN + "," + to_string(year) + "," + to_string(totalCopies) + "," + to_string(copiesBorrowed);
    }
};

// Function to Load Books from CSV
vector<Book> loadBooks() {
    vector<Book> books;
    ifstream inFile("books.csv");
    string line;

    while (getline(inFile, line)) {
        stringstream ss(line);
        string serialNumberStr, title, author, ISBN, yearStr, totalCopiesStr, copiesBorrowedStr;
        int serialNumber, year, totalCopies, copiesBorrowed;

        if (getline(ss, serialNumberStr, ',') &&
            getline(ss, title, ',') &&
            getline(ss, author, ',') &&
            getline(ss, ISBN, ',') &&
            getline(ss, yearStr, ',') &&
            getline(ss, totalCopiesStr, ',') &&
            getline(ss, copiesBorrowedStr, ',')) {

            try {
                serialNumber = stoi(serialNumberStr);
                year = stoi(yearStr);
                totalCopies = stoi(totalCopiesStr);
                copiesBorrowed = stoi(copiesBorrowedStr);
                books.emplace_back(serialNumber, title, author, ISBN, year, totalCopies);
                books.back().borrowCopy(copiesBorrowed); // Adjust borrowed copies
            } catch (const invalid_argument& e) {
                cerr << "Error: Invalid data format in books.csv. Skipping line.\n";
            }
        }
    }
    inFile.close();
    return books;
}

// Function to Save Books to CSV
void saveBooks(const vector<Book>& books) {
    ofstream outFile("books.csv");
    for (const auto& book : books) {
        outFile << book.toCSV() << endl;
    }
    outFile.close();
}

// Function to Load Members from CSV
vector<Member> loadMembers() {
    vector<Member> members;
    ifstream inFile("members.csv");
    string line;

    while (getline(inFile, line)) {
        stringstream ss(line);
        string name, idStr, contact, borrowedStr, reviewsStr;
        int id;

        if (getline(ss, name, ',') &&
            getline(ss, idStr, ',') &&
            getline(ss, contact, ',') &&
            getline(ss, borrowedStr, ',') &&
            getline(ss, reviewsStr, ',')) {

            try {
                id = stoi(idStr);
                Member member(name, id, contact);

                stringstream borrowedSS(borrowedStr);
                string token;
                while (getline(borrowedSS, token, ';')) {
                    if (!token.empty()) {
                        member.borrowBook(stoi(token));
                    }
                }

                stringstream reviewSS(reviewsStr);
                while (getline(reviewSS, token, ';')) {
                    size_t pos = token.find(":");
                    if (pos != string::npos) {
                        int bookID = stoi(token.substr(0, pos));
                        string review = token.substr(pos + 1);
                        member.addReview(bookID, review);
                    }
                }

                members.push_back(member);
            } catch (const invalid_argument& e) {
                cerr << "Error: Invalid data format in members.csv. Skipping line.\n";
            }
        }
    }
    inFile.close();
    return members;
}

// Function to Save Members to CSV
void saveMembers(const vector<Member>& members) {
    ofstream outFile("members.csv");
    for (const auto& member : members) {
        outFile << member.toCSV() << endl;
    }
    outFile.close();
}

// Function to Log Transactions
void logTransaction(int memberID, int bookID, const string& action, int numCopies) {
    ofstream outFile("transactions.csv", ios::app);
    time_t now = time(0);
    outFile << memberID << "," << bookID << "," << action << "," << numCopies << "," << ctime(&now);
    outFile.close();
}

// Function to Display Books Horizontally
void displayBooksHorizontally(const vector<Book>& books) {
    cout << "\nAvailable Books:\n";
    cout << left << setw(15) << "Serial No." << setw(30) << "Title" << setw(20) << "Author" << setw(15) << "Available" << setw(15) << "Borrowed" << endl;
    cout << "-----------------------------------------------------------------------------------------\n";
    for (const auto& book : books) {
        cout << left << setw(15) << book.getSerialNumber() << setw(30) << book.getTitle() << setw(20) << book.getAuthor() << setw(15) << book.getCopiesAvailable() << setw(15) << book.getCopiesBorrowed() << endl;
    }
}

// Function to Display Members Horizontally
void displayMembersHorizontally(const vector<Member>& members) {
    cout << "\nMembers:\n";
    cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(20) << "Contact" << setw(20) << "Borrowed Books" << endl;
    cout << "-----------------------------------------------------------------------------------------\n";
    for (const auto& member : members) {
        cout << left << setw(10) << member.getID() << setw(20) << member.getName() << setw(20) << member.getContact() << setw(20);
        for (int book : member.getBorrowedBooks()) {
            cout << book << " ";
        }
        cout << endl;
    }
}

// Function to Check for Duplicate Member ID
bool isDuplicateMemberID(const vector<Member>& members, int id) {
    for (const auto& member : members) {
        if (member.getID() == id) {
            return true;
        }
    }
    return false;
}

// Function to Check for Duplicate Book Serial Number
bool isDuplicateBookSerialNumber(const vector<Book>& books, int serialNumber) {
    for (const auto& book : books) {
        if (book.getSerialNumber() == serialNumber) {
            return true;
        }
    }
    return false;
}

// Librarian Menu
void librarianMenu(vector<Book>& books, vector<Member>& members) {
    int choice;
    do {
        cout << "\nLibrarian Menu:\n1. Manage Books\n2. Manage Members\n3. Exit\nEnter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                // Book Management Menu
                int bookChoice;
                cout << "\nBook Management:\n1. Add Book\n2. View Books\n3. Update Book\n4. Delete Book\nEnter choice: ";
                cin >> bookChoice;

                switch (bookChoice) {
                    case 1: {
                        int serialNumber, year, totalCopies;
                        string title, author, ISBN;

                        while (true) {
                            cout << "Enter Serial Number: ";
                            cin >> serialNumber;

                            if (isDuplicateBookSerialNumber(books, serialNumber)) {
                                cout << "Error: Serial Number already exists. Please enter a unique Serial Number.\n";
                            } else {
                                break;
                            }
                        }

                        cin.ignore();
                        cout << "Enter Title: ";
                        getline(cin, title);
                        cout << "Enter Author: ";
                        getline(cin, author);
                        cout << "Enter ISBN: ";
                        getline(cin, ISBN);
                        cout << "Enter Year: ";
                        cin >> year;
                        cout << "Enter Total Copies: ";
                        cin >> totalCopies;

                        books.emplace_back(serialNumber, title, author, ISBN, year, totalCopies);
                        saveBooks(books);
                        cout << "Book added successfully!\n";
                        break;
                    }
                    case 2: {
                        displayBooksHorizontally(books);
                        break;
                    }
                    case 3: {
                        int serialNumber;
                        cout << "Enter Serial Number of Book to Update: ";
                        cin >> serialNumber;

                        for (auto& book : books) {
                            if (book.getSerialNumber() == serialNumber) {
                                string title, author, ISBN;
                                int year, totalCopies;
                                cin.ignore();
                                cout << "Enter New Title: ";
                                getline(cin, title);
                                cout << "Enter New Author: ";
                                getline(cin, author);
                                cout << "Enter New ISBN: ";
                                getline(cin, ISBN);
                                cout << "Enter New Year: ";
                                cin >> year;
                                cout << "Enter New Total Copies: ";
                                cin >> totalCopies;

                                book = Book(serialNumber, title, author, ISBN, year, totalCopies);
                                saveBooks(books);
                                cout << "Book updated successfully!\n";
                                return;
                            }
                        }
                        cout << "Book not found!\n";
                        break;
                    }
                    case 4: {
                        int serialNumber;
                        cout << "Enter Serial Number of Book to Delete: ";
                        cin >> serialNumber;

                        auto it = remove_if(books.begin(), books.end(), [serialNumber](const Book& b) {
                            return b.getSerialNumber() == serialNumber;
                        });

                        if (it != books.end()) {
                            books.erase(it, books.end());
                            saveBooks(books);
                            cout << "Book deleted successfully!\n";
                        } else {
                            cout << "Book not found!\n";
                        }
                        break;
                    }
                    default:
                        cout << "Invalid choice!\n";
                }
                break;
            }
            case 2: {
                // Member Management Menu
                int memberChoice;
                cout << "\nMember Management:\n1. Add Member\n2. View Members\n3. Update Member\n4. Delete Member\nEnter choice: ";
                cin >> memberChoice;

                switch (memberChoice) {
                    case 1: {
                        string name, contact;
                        int id;

                        while (true) {
                            cout << "Enter ID: ";
                            cin >> id;

                            if (isDuplicateMemberID(members, id)) {
                                cout << "Error: Member ID already exists. Please enter a unique ID.\n";
                            } else {
                                break;
                            }
                        }

                        cin.ignore();
                        cout << "Enter Name: ";
                        getline(cin, name);
                        cout << "Enter Contact: ";
                        getline(cin, contact);

                        members.emplace_back(name, id, contact);
                        saveMembers(members);
                        cout << "Member added successfully!\n";
                        break;
                    }
                    case 2: {
                        displayMembersHorizontally(members);
                        break;
                    }
                    case 3: {
                        int id;
                        cout << "Enter ID of Member to Update: ";
                        cin >> id;

                        for (auto& member : members) {
                            if (member.getID() == id) {
                                string name, contact;
                                cin.ignore();
                                cout << "Enter New Name: ";
                                getline(cin, name);
                                cout << "Enter New Contact: ";
                                getline(cin, contact);

                                member = Member(name, id, contact);
                                saveMembers(members);
                                cout << "Member updated successfully!\n";
                                return;
                            }
                        }
                        cout << "Member not found!\n";
                        break;
                    }
                    case 4: {
                        int id;
                        cout << "Enter ID of Member to Delete: ";
                        cin >> id;

                        auto it = remove_if(members.begin(), members.end(), [id](const Member& m) {
                            return m.getID() == id;
                        });

                        if (it != members.end()) {
                            members.erase(it, members.end());
                            saveMembers(members);
                            cout << "Member deleted successfully!\n";
                        } else {
                            cout << "Member not found!\n";
                        }
                        break;
                    }
                    default:
                        cout << "Invalid choice!\n";
                }
                break;
            }
            case 3:
                cout << "Exiting Librarian Menu...\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while (choice != 3);
}

// Member Menu
void memberMenu(vector<Book>& books, vector<Member>& members) {
    int memberID;
    cout << "Enter Member ID: ";
    cin >> memberID;

    auto memberIt = find_if(members.begin(), members.end(), [memberID](const Member& m) {
        return m.getID() == memberID;
    });

    if (memberIt == members.end()) {
        cout << "Member not found!\n";
        return;
    }

    int choice;
    do {
        cout << "\nMember Menu:\n1. Borrow Book\n2. Return Book\n3. Leave Review\n4. View Borrowed Books\n5. Exit\nEnter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                displayBooksHorizontally(books);

                int bookID, numCopies;
                cout << "Enter Book Serial Number: ";
                cin >> bookID;
                cout << "Enter Number of Copies to Borrow (max 3): ";
                cin >> numCopies;

                if (numCopies < 1 || numCopies > 3) {
                    cout << "Invalid number of copies! You can borrow 1 to 3 copies at a time.\n";
                    break;
                }

                auto bookIt = find_if(books.begin(), books.end(), [bookID](const Book& b) {
                    return b.getSerialNumber() == bookID;
                });

                if (bookIt != books.end()) {
                    // Check if member has already borrowed 5 books
                    if (memberIt->getBorrowedBooks().size() >= 5) {
                        cout << "You cannot borrow more than 5 books!\n";
                        break;
                    }

                    // Check if member has already borrowed 3 copies of the same book
                    int count = count_if(memberIt->getBorrowedBooks().begin(), memberIt->getBorrowedBooks().end(), [bookID](int id) {
                        return id == bookID;
                    });

                    if (count + numCopies > 3) {
                        cout << "You cannot borrow more than 3 copies of the same book!\n";
                        break;
                    }

                    if (bookIt->borrowCopy(numCopies)) {
                        for (int i = 0; i < numCopies; i++) {
                            memberIt->borrowBook(bookID);
                        }
                        saveBooks(books);
                        saveMembers(members);
                        logTransaction(memberID, bookID, "Borrow", numCopies);
                        cout << "Book borrowed successfully!\n";
                    } else {
                        cout << "Not enough copies available for this book!\n";
                    }
                } else {
                    cout << "Book not found!\n";
                }
                break;
            }
            case 2: {
                int bookID, numCopies;
                cout << "Enter Book Serial Number: ";
                cin >> bookID;
                cout << "Enter Number of Copies to Return: ";
                cin >> numCopies;

                auto bookIt = find_if(books.begin(), books.end(), [bookID](const Book& b) {
                    return b.getSerialNumber() == bookID;
                });

                if (bookIt != books.end()) {
                    bookIt->returnCopy(numCopies);
                    for (int i = 0; i < numCopies; i++) {
                        memberIt->returnBook(bookID);
                    }
                    saveBooks(books);
                    saveMembers(members);
                    logTransaction(memberID, bookID, "Return", numCopies);
                    cout << "Book returned successfully!\n";
                } else {
                    cout << "Book not found!\n";
                }
                break;
            }
            case 3: {
                int bookID;
                string review;
                cout << "Enter Book Serial Number: ";
                cin >> bookID;
                cin.ignore();
                cout << "Enter Review: ";
                getline(cin, review);

                memberIt->addReview(bookID, review);
                saveMembers(members);
                cout << "Review added successfully!\n";
                break;
            }
            case 4: {
                cout << "Borrowed Books:\n";
                for (int bookID : memberIt->getBorrowedBooks()) {
                    auto bookIt = find_if(books.begin(), books.end(), [bookID](const Book& b) {
                        return b.getSerialNumber() == bookID;
                    });

                    if (bookIt != books.end()) {
                        cout << "Title: " << bookIt->getTitle() << ", Copies Borrowed: " << count(memberIt->getBorrowedBooks().begin(), memberIt->getBorrowedBooks().end(), bookID) << endl;
                    }
                }
                break;
            }
            case 5:
                cout << "Exiting Member Menu...\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while (choice != 5);
}

// Main Menu
void mainMenu(vector<Book>& books, vector<Member>& members) {
    int choice;
    do {
        cout << "\nMain Menu:\n1. Librarian Menu\n2. Member Menu\n3. Exit\nEnter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                librarianMenu(books, members);
                break;
            case 2:
                memberMenu(books, members);
                break;
            case 3:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while (choice != 3);
}

int main() {
    vector<Book> books = loadBooks();
    vector<Member> members = loadMembers();
    mainMenu(books, members);
    return 0;
}