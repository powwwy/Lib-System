#include <iostream>
#include <bits/stdc++.h>
#include <chrono>
using namespace std;

// Global variables to hold data
vector<vector<string>> content; // To store the CSV file content
vector<string> toupdate;        // To store data to be updated

// Function to read data from a CSV file and store it into 'content' variable
void readfile(string fname)
{
    vector<string> row; // To store each row of data
    string line, word;  // Variables to handle each line and word in the file

    fstream file(fname, ios::in); // Open the file in read mode
    if (file.is_open())
    { // Check if file is successfully opened
        while (getline(file, line))
        {                                   // Read each line of the file
            row.clear();                    // Clear previous row data
            stringstream str(line);         // Create stringstream to split the line by commas
            while (getline(str, word, ',')) // Split the line by commas
                row.push_back(word);        // Store words in the row
            content.push_back(row);         // Store the row in content
        }
    }
    else
    {
        cout << "Could not open the file\n"; // Handle file open failure
    }
}
// Function to write the entire content of the vector 'par' to a CSV file
void writefile(vector<vector<string>> par, string fname)
{
    fstream fout(fname, ios::out); // Open the file in write mode
    for (auto x : par)
    { // Iterate through each row in the 'par' vector
        for (auto y : x)
        {              // Iterate through each word in the row
            fout << y; // Write the word to the file
            if (y != x.back())
                fout << ","; 
        }
        fout << "\n"; 
    }
}
// Function to append a new line to a CSV file
void writefileappend(vector<string> par, string fname)
{
    fstream fout(fname, ios::out | ios::app); // Open file in append mode
    for (auto x : par)
    {              // Iterate through each word in the row
        fout << x; // Write the word to the file
        if (x != par.back())
            fout << ","; 
    }
    fout << "\n"; 
}
// Function to print the data stored in a vector
void printdata(vector<vector<string>> par)
{
    int c = 1; // Counter for row numbers
    for (auto x : par)
    {                      // Iterate through each row
        cout << c << ". "; // Print the row number
        for (auto y : x)
        { // Iterate through each word in the row
            cout << y;
            if (y != x.back())
                cout << " | "; // Print words with separator '|' between them
        }
        c++;          // Increment the counter
        cout << "\n"; // Newline after each row
    }
}

// Function to print data without showing passwords
void printdata_notpassword(vector<vector<string>> par)
{
    int c = 1; // Counter for row numbers
    for (auto x : par)
    { // Iterate through each row
        cout << c << ". ";
        // Print only the necessary details (excluding password)
        cout << x[0] << " | " << x[1] << " | " << x[3] << '\n';
        c++; // Increment the counter
    }
}

// User class that contains common user functionalities like login and issuing/returning books
class User{
    private:
        string password; // User's password (private to the class)
    public:
        string name;                 // User's name
        string id;                   // User's ID
        vector<string> isbns_issued; // List of books issued by the user
    
        // List of methods related to user functionalities
        void display_menu();
        void login();
        void see_all_books(string id);
        void see_issued_books(string id);
        void logout();
        void issue_book(string id, string bookname,string type_user);
        void return_book(string id,string isbncode);
        void check_available(string id, string bookname);
        int calc_fine(string id,string type_user);
        void clear_fine_amount(string id,string type_user);
        void User_add(string id);
        void User_update(string id);
        void User_delete(string id);
        void User_search(string id);
};
// Book class to handle book-related tasks
class Book{
    public:
    string title;       // Book title
    string author;      // Book author
    string isbn;        // Book ISBN
    string publication; // Book publication name
    int is_issued;      // Book issue status (1 = issued, 0 = not issued)

    // List of methods related to book functionalities
    void Book_request(string id, string bookname, string type_user);
    void Show_duedate(string isbn_no);
    void Book_add(string id);
    void Book_update(string id);
    void Book_delete(string id);
    void Book_search(string id);
};

// Student class inherits from User, adds student-specific functionalities
class Student : public User
{
public:
    int Fine_amount; // Fine amount for the student
    void display_student_menu(string id);
};

// Professor class inherits from User, adds professor-specific functionalities
class Professor : public User
{
public:
    int Fine_amount; // Fine amount for the professor
    void display_prof_menu(string id);
};


// Librarian class inherits from User, adds librarian-specific functionalities
class Librarian : public User
{
public:
    void display_lib_menu(string id);
    void see_all_users(string id);
    void see_all_books(string id);
    void add_user(string id);
    void update_user(string id);
    void delete_user(string id);
    void add_book(string id);
    void update_book(string id);
    void delete_book(string id);
    void see_issued_to_user(string id, string uid);
    void see_issued_book(string id, string isbn);
};

// Display menu for student users
void Student :: display_student_menu(string id){
    cout<<"---------------------------------------------------------------------\n";
    cout<<"\nYou have been logged in as Student\n";
    cout<<"Press 1 to see all the books\n";
    cout<<"Press 2 to view books issued by you\n";
    cout<<"Press 3 to check if a book is available for issue or not\n";
    cout<<"Press 4 to view the fine\n";
    cout<<"Press 5 to issue a book\n";
    cout<<"Press 6 to return a book\n";
    cout<<"Press 7 to clear your fine\n";
    cout<<"Press 8 to logout\n";
    cout<<"---------------------------------------------------------------------\n";
    char c;
    cin>>c;
    string book_name,isbncode;
    Student s;
    Book b;

// Perform different actions based on user input
switch (c)
{
case '1':
    s.see_all_books(id);        // Show all books
    s.display_student_menu(id); // Re-display the menu
    break;
case '2':
    s.see_issued_books(id);     // Show books issued by the student
    s.display_student_menu(id); // Re-display the menu
    break;
case '3':
    cout << "Enter the name of the book you want to check: ";
    cin.ignore();
    getline(cin, book_name);          // Get the book name from user input
    s.check_available(id, book_name); // Check if the book is available
    s.display_student_menu(id);       // Re-display the menu
    break;
case '4':
    s.calc_fine(id, "1");       // Calculate fine for student
    s.display_student_menu(id); // Re-display the menu
    break;
case '5':
    cout << "Enter the name of the book you want to issue: ";
    cin.ignore();
    getline(cin, book_name);            // Get the book name from user input
    b.Book_request(id, book_name, "1"); // Issue the book
    s.display_student_menu(id);         // Re-display the menu
    break;
case '6':
    cout << "Enter the isbn code of the book you want to return: ";
    cin >> isbncode;             // Get the ISBN code from user input
    s.return_book(id, isbncode); // Return the book
    s.display_student_menu(id);  // Re-display the menu
    break;
case '7':
    s.clear_fine_amount(id, "1"); // Clear fine for student
    s.display_student_menu(id);   // Re-display the menu
    break;
case '8':
    logout(); // Logout the user
    break;
}
}
// Display menu for professor users
void Professor::display_prof_menu(string id)
{
    // Same as Student menu but for professors
    cout << "---------------------------------------------------------------------\n";
    cout << "\nYou have been logged in as Lecturer\n";
    cout << "Press 1 to see all the books\n";
    cout << "Press 2 to view books issued by you\n";
    cout << "Press 3 to check if a book is available for issue or not\n";
    cout << "Press 4 to view the fine\n";
    cout << "Press 5 to issue a book\n";
    cout << "Press 6 to return a book\n";
    cout << "Press 7 to clear your fine\n";
    cout << "Press 8 to logout\n";
    cout << "---------------------------------------------------------------------\n";
    char c;
    cin >> c; // Read user input
    string book_name, isbncode;
    Professor s;

    // Perform different actions based on user input
    switch (c)
    {
    case '1':
        s.see_all_books(id);     // Show all books
        s.display_prof_menu(id); // Re-display the menu
        break;
    case '2':
        s.see_issued_books(id);  // Show books issued by the professor
        s.display_prof_menu(id); // Re-display the menu
        break;
    case '3':
        cout << "Enter the name of the book you want to check: ";
        cin >> book_name;                 // Get the book name from user input
        s.check_available(id, book_name); // Check if the book is available
        s.display_prof_menu(id);          // Re-display the menu
        break;
    case '4':
        s.calc_fine(id, "2");    // Calculate fine for professor
        s.display_prof_menu(id); // Re-display the menu
        break;
    case '5':
        cout << "Enter the name of the book you want to issue: ";
        cin >> book_name;                 // Get the book name from user input
        s.issue_book(id, book_name, "2"); // Issue the book
        s.display_prof_menu(id);          // Re-display the menu
        break;
    case '6':
        cout << "Enter the isbn code of the book you want to return: ";
        cin >> isbncode;             // Get the ISBN code from user input
        s.return_book(id, isbncode); // Return the book
        s.display_prof_menu(id);     // Re-display the menu
        break;
    case '7':
        s.clear_fine_amount(id, "2"); // Clear fine for professor
        s.display_prof_menu(id);      // Re-display the menu
        break;
    case '8':
        logout(); // Logout the user
        break;
    }
}

// Function to display the main menu for the user
void User::display_menu()
{
    char c;
    cout << "---------------------------------------------------------------------\n";
    cout << "\n\nWelcome to the library management system!\n\n";
    cout << "1. Press 1 to log in : \n";
    cout << "2. Press 2 to exit\n";
    cout << "---------------------------------------------------------------------\n";
    cin >> c; // Read user input
    if (c == '1')
    { // If user chooses to log in
        User u;
        u.login(); // Call login function
    }
    else
    {
        cout << "Thanks for using the system!\n"; // If user chooses to exit
        exit(1);                                  // Exit the program
    }
}
void User::login() {
    // Declare variables for user ID and password
    string id, password;
    
    // Prompt the user for login credentials
    cout << "Enter your id: ";
    cin >> id;
    cout << "Enter the password: ";
    cin >> password;

    // Vector to store words extracted from a CSV row
    vector<string> words_in_a_row;
    string line_in_csv, word;

    // Open the user data CSV file in read mode
    fstream file("all_users_data.csv", ios::in);

    int count = 0;  // Counter to track login status

    // Check if the file is open
    if (file.is_open()) {
        // Read each line from the CSV file
        while (getline(file, line_in_csv)) {
            words_in_a_row.clear();  // Clear previous row data

            // Parse the CSV row and extract words separated by commas
            stringstream str(line_in_csv);
            while (getline(str, word, ',')) 
                words_in_a_row.push_back(word);

            // Check if the entered ID matches the stored ID
            if (words_in_a_row[1] == id) {
                count = 1;  // User ID found

                // Check if the password matches
                if (words_in_a_row[2] == password) {
                    count++;  // Successful login
                    break;
                } else {
                    // Incorrect password handling
                    while (password != words_in_a_row[2]) {
                        cout << "You entered the wrong password. Press 1 to re-enter the password and 2 to exit\n";
                        char c;
                        cin >> c;

                        if (c == '1') {
                            cout << "Enter the password: ";
                            cin >> password;
                            if (password == words_in_a_row[2]) 
                                count++;  // Correct password entered
                        } else if (c == '2') {
                            cout << "Exiting...";
                            return;  // Exit login process
                        } else {
                            cout << "Please enter a valid input.\n";
                        }
                    }
                    if (count == 2) break;  // Exit loop if login is successful
                }
            }
            if (count == 2) break;  // Stop checking further once login is successful
        }

        // If user ID was not found in the CSV file
        if (count == 0) {
            cout << "User not found\n";
            User u;
            u.display_menu();  // Return to the main menu
        }

        // If login is successful, check user role and display the appropriate menu
        if (count == 2) {
            Student s;
            Professor p;
            Librarian l;

            // Check user role and call the corresponding menu function
            if (words_in_a_row[3] == "1") {
                s.display_student_menu(words_in_a_row[1]);  // Student menu
            } else if (words_in_a_row[3] == "2") {
                p.display_prof_menu(words_in_a_row[1]);  // Professor menu
            } else {
                l.display_lib_menu(words_in_a_row[1]);  // Librarian menu
            }
        }
    }
}

// Function to display all books
void User::see_all_books(string id) {
    content.clear();
    cout << "In the data below, 0 signifies that the book is not issued and 1 means that the book is issued.\n";
    readfile("all_books_data.csv"); // Read book data from file
    printdata(content); // Display book data
    content.clear();
}

// Function to issue a book to a user
void User::issue_book(string id, string bookname, string type_user) {
    content.clear();
    int fl = 0;

    // If the user is a student (type_user == "1")
    if (type_user == "1") {
        int count = 0;
        content.clear();
        readfile("issued_books_data.csv"); // Read issued books data

        // Count the number of books already issued to the user
        for (int i = 0; i < content.size(); i++) {
            if (content[i][0] == id) {
                count++;
            }
        }
        content.clear();

        // Limit for students: max 5 books
        if (count > 4) {
            cout << "You can't issue more than 5 books!\n\n";
        } else {
            readfile("all_books_data.csv"); // Read all books data

            // Check if the requested book is available
            for (int i = 0; i < content.size(); i++) {
                if (content[i][0] == bookname && content[i][4] == "0") {
                    fl = 1;
                    cout << "Book is available!\n";
                    content[i][4] = "1"; // Mark book as issued

                    // Store issued book details
                    toupdate.clear();
                    toupdate.push_back(id);
                    toupdate.push_back(bookname);
                    toupdate.push_back(content[i][3]); // ISBN code
                    toupdate.push_back(to_string(time(0))); // Timestamp

                    // Append to issued books file
                    writefileappend(toupdate, "issued_books_data.csv");
                    cout << "Book successfully issued\n";
                    break;
                }
            }
            if (fl == 0) cout << "2\n";
            writefile(content, "all_books_data.csv"); // Update book availability
            content.clear();
        }
    }

    // If the user is a professor (type_user == "2"), no issue limit
    if (type_user == "2") {
        readfile("all_books_data.csv");
        for (int i = 0; i < content.size(); i++) {
            if (content[i][0] == bookname && content[i][4] == "0") {
                fl = 1;
                cout << "Book is available!\n";
                content[i][4] = "1"; // Mark book as issued
                toupdate.clear();
                toupdate.push_back(id);
                toupdate.push_back(bookname);
                toupdate.push_back(content[i][3]); // ISBN code
                toupdate.push_back(to_string(time(0))); // Timestamp

                // Append to issued books file
                writefileappend(toupdate, "issued_books_data.csv");
                cout << "Book successfully issued\n";
                break;
            }
        }
        if (fl == 0) cout << "Book not available\n";
        writefile(content, "all_books_data.csv"); // Update book availability
        content.clear();
    }
}

// Function to return an issued book
void User::return_book(string id, string isbncode) {
    content.clear();
    int fl = 0;
    readfile("issued_books_data.csv");

    // Check if the book was issued by the user
    for (int i = 0; i < content.size(); i++) {
        if (content[i][2] == isbncode && content[i][0] == id) {
            fl = 1;
            content.erase(content.begin() + i, content.begin() + i + 1);
            cout << "Book was issued by you and is now being returned\n";

            // Update issued books file
            writefile(content, "issued_books_data.csv");
            content.clear();

            // Mark book as available again
            readfile("all_books_data.csv");
            for (int i = 0; i < content.size(); i++) {
                if (content[i][3] == isbncode) content[i][4] = "0";
            }
            writefile(content, "all_books_data.csv");
            break;
        }
    }
    if (fl == 0) cout << "Invalid details!\n";
    content.clear();
}

// Function to display books issued by a user
void User::see_issued_books(string id) {
    int count = 1;
    content.clear();
    readfile("issued_books_data.csv");

    for (int i = 0; i < content.size(); i++) {
        if (content[i][0] == id) {
            cout << count << ". ";
            count++;

            // Print book details
            for (int k = 0; k < content[i].size() - 1; k++)
                cout << content[i][k] << " | ";

            // Convert and display issue date
            time_t stamp = stoi(content[i][3]);
            tm *dateformat = localtime(&stamp);
            cout << dateformat->tm_mday << "/" << 1 + dateformat->tm_mon << "/" << 1900 + dateformat->tm_year << "\n";
        }
    }
    if (count == 1) cout << "You haven't issued any books as of now\n\n";
    content.clear();
}

// Function to check if a book is available
void User::check_available(string id, string bookname) {
    content.clear();
    int fl = 0;
    readfile("all_books_data.csv");
    int count = 1;

    for (int i = 0; i < content.size(); i++) {
        if (content[i][0] == bookname && content[i][4] == "0") {
            fl = 1;
            cout << count << ". ";
            count++;
            cout << "Book is available!\n";

            // Display book details
            for (auto j : content[i])
                cout << j << " | ";
            cout << "\n";
            break;
        }
    }
    if (fl == 0) cout << "Book not available\n";
    content.clear();
}

// Function to calculate overdue fines
int User::calc_fine(string id, string type_user) {
    int fine = 0;
    content.clear();
    readfile("issued_books_data.csv");

    for (int i = 0; i < content.size(); i++) {
        if (content[i][0] == id) {
            int curtime = time(0);
            int isstime = stoi(content[i][3]);
            int days_overdue = (curtime - isstime) / 86400;

            // Students: fine applies after 30 days
            if (type_user == "1" && days_overdue > 30)
                fine += 2 * (days_overdue - 30);

            // Professors: fine applies after 60 days
            if (type_user == "2" && days_overdue > 60)
                fine += 5 * (days_overdue - 60);
        }
    }
    cout << "Your fine is: " << fine << "\n";
    return fine;
}

// Function to log out
void User::logout() {
    cout << "Logging out...\n";
    User u;
    u.display_menu();
}

// Function to clear outstanding fines
void User::clear_fine_amount(string id, string type_user) {
    int a = calc_fine(id, type_user);
    if (a == 0) cout << "You have no pending dues\n";
    else cout << "Your fine of Rupees " << a << " is being cleared.\n";
}

// Librarian functions to manage users
void User::User_add(string id) {
    Librarian l;
    l.add_user(id);
}
void User::User_delete(string id) {
    Librarian l;
    l.delete_user(id);
}
void User::User_update(string id) {
    Librarian l;
    l.update_user(id);
}

// Function to display the due date of a book
void Book::Show_duedate(string isbn) {
    content.clear();
    string uid;
    time_t stamp;
    readfile("issued_books_data.csv");

    for (int i = 0; i < content.size(); i++) {
        if (content[i][2] == isbn) {
            uid = content[i][0];
            stamp = stoi(content[i][3]);
            break;
        }
    }
    stamp += (uid == "1" ? 30 : 60) * 86400; // Due in 30/60 days
    tm *due_time = localtime(&stamp);
    cout << "Due date of the book is: " << due_time->tm_mday << "/" << 1 + due_time->tm_mon << "/" << 1900 + due_time->tm_year << "\n";
}

void Book :: Book_request(string id,string bookname,string type_user){
    User u;
    u.issue_book(id,bookname,type_user);
}
void Book :: Book_add(string id){
    Librarian l;
    l.add_book(id);
}
void Book :: Book_delete(string id){
    Librarian l;
    l.delete_book(id);
}
void Book :: Book_update(string id){
    Librarian l;
    l.update_book(id);
}
void Book :: Book_search(string id){

}

void Librarian :: display_lib_menu(string id){
    cout<<"---------------------------------------------------------------------\n";
    cout<<"\nWelcome "<<id<<"! You are logged in as librarian.\n\n";
    cout<<"Press 1 to add a user\n";
    cout<<"Press 2 to update a user\n";
    cout<<"Press 3 to delete a user\n";
    cout<<"Press 4 to add a book\n";
    cout<<"Press 5 to update a book\n";
    cout<<"Press 6 to delete a book\n";
    cout<<"Press 7 to see all books issued to a particular user\n";
    cout<<"Press 8 to see which book is issued to which user\n";
    cout<<"Press 9 to view all users\n";
    cout<<"Press 0 to view all books\n";
    cout<<"Press d to show due date of a book\n";
    cout<<"Press l to logout\n";
    cout<<"---------------------------------------------------------------------\n";
    char c;
    cin>>c;
    Librarian l;
    Book b;
    User u;
    string isbn,uid;
    switch(c){
        case '1':
            u.User_add(id);
            l.display_lib_menu(id);
            break;
        case '2':
            u.User_update(id);
            l.display_lib_menu(id);
            break;
        case '3':
            u.User_delete(id);
            l.display_lib_menu(id);
            break;
        case '4':
            b.Book_add(id);
            l.display_lib_menu(id);
            break;
        case '5':
            b.Book_update(id);
            l.display_lib_menu(id);
            break;
        case '6':
            b.Book_delete(id);
            l.display_lib_menu(id);
            break;
        case '7':
            cout<<"Enter the id of the user : ";
            cin>>uid;
            l.see_issued_to_user(id,uid);
            l.display_lib_menu(id);
            break;
        case '8':
            cout<<"Enter the isbn number of the book : ";
            cin>>isbn;
            l.see_issued_book(id,isbn);
            l.display_lib_menu(id);
            break;
        case '9':
            l.see_all_users(id);
            l.display_lib_menu(id);
            break;
        case '0':
            l.see_all_books(id);
            l.display_lib_menu(id);
            break;
        case 'd':
            cout<<"Enter the isbn number of the book : ";
            cin>>isbn;
            b.Show_duedate(isbn);
            l.display_lib_menu(id);
            break;
        case 'l':
            logout();
            l.display_lib_menu(id);
            break;
    }
}

void Librarian :: see_all_users(string id){
    // Clear the content vector to avoid old data being displayed
    content.clear();
    
    // Read user data from the file "all_users_data.csv"
    readfile("all_users_data.csv");
    
    // Display information about the format of user data
    cout<<"Below are all the users. 1 at the end signifies a student, 2 means a professor, and 3 means a librarian \n\n";
    cout<<"The data is given in the form of username, ID, password, type of user.\n\n";
    
    // Print the user data without showing passwords
    printdata_notpassword(content);
    
    // Clear content vector to free memory
    content.clear();
}

void Librarian :: see_all_books(string id){
    content.clear(); // Clear previous content
    readfile("all_books_data.csv"); // Read book data from file
    
    // Display information about the format of book data
    cout<<"Below are all the books.\n";
    cout<<"The data is given in the form of book name, author, publisher, ISBN number, and is_issued (1 if book is issued).\n\n";
    
    printdata(content); // Print the book data
    content.clear(); // Clear content vector
}

void Librarian :: add_user(string id){
    // Prompt the librarian to enter new user details
    cout<<"Please enter the details of the new user : \n";
    string _name,_id,_password,_type;
    
    cout<<"Enter name: ";
    cin.ignore(); // Ignore leftover newline from previous input
    getline(cin, _name); // Read full name including spaces
    
    cout<<"Enter ID : ";
    cin>>_id;
    
    cout<<"Enter password : ";
    cin>>_password;
    
    cout<<"Enter type of the user (1 for student, 2 for professor, 3 for librarian): ";
    cin>>_type;
    
    // Open file in append mode to add new user details
    fstream fout("all_users_data.csv", ios::out | ios::app);
    fout<<_name<<","<<_id<<","<<_password<<","<<_type<<'\n';
    
    cout<<"New user has been added.\n";
}

void Librarian :: add_book(string id){
    // Prompt the librarian to enter book details
    cout<<"Please enter the details of the new book : \n";
    string title, author, isbn, publisher;
    
    cout<<"Enter title: ";
    cin.ignore(); // Ignore newline character
    getline(cin, title);
    
    cout<<"Enter author : ";
    cin>>author;
    
    cout<<"Enter ISBN : ";
    cin>>isbn;
    
    cout<<"Enter name of publisher : ";
    cin>>publisher;
    
    // Open file in append mode to add new book details
    fstream fout("all_books_data.csv", ios::out | ios::app);
    fout<<title<<","<<author<<","<<isbn<<","<<publisher<<",0\n"; // 0 means book is not issued
    
    cout<<"New book has been added.\n";
}

void Librarian :: delete_book(string id){
    string isbnno;
    cout<<"Enter the ISBN number of the book : ";
    cin>>isbnno;
    content.clear();
    int found = 0;
    
    // Read book data into vector
    readfile("all_books_data.csv");
    
    // Search for the book with the given ISBN
    for(int i = 0; i < content.size(); i++){
        if(content[i][3] == isbnno){
            found = 1;
            content.erase(content.begin() + i, content.begin() + i + 1); // Remove book entry
            break;
        }
    }
    
    // Update the books file
    writefile(content, "all_books_data.csv");
    content.clear();
    
    // Remove book from issued books data if issued
    readfile("issued_books_data.csv");
    for(int i = 0; i < content.size(); i++){
        if(content[i][2] == isbnno){
            content.erase(content.begin() + i, content.begin() + i + 1);
            break;
        }
    }
    
    writefile(content, "issued_books_data.csv");
    content.clear();
    
    if(found == 0) cout<<"Book not found.\n";
}

void Librarian :: delete_user(string id){
    string uid;
    cout<<"Enter the user ID of the user : ";
    cin>>uid;
    content.clear();
    int found = 0;
    readfile("all_users_data.csv");
    
    // Search and remove user entry
    for(int i = 0; i < content.size(); i++){
        if(content[i][1] == uid){
            found = 1;
            content.erase(content.begin() + i, content.begin() + i + 1);
            break;
        }
    }
    
    writefile(content, "all_users_data.csv");
    content.clear();
    
    vector<string> isbns;
    readfile("issued_books_data.csv");
    for(int i = 0; i < content.size(); i++){
        if(content[i][0] == uid){
            isbns.push_back(content[i][2]); // Store issued book ISBNs
            content.erase(content.begin() + i, content.begin() + i + 1);
            break;
        }
    }
    
    writefile(content, "issued_books_data.csv");
    content.clear();
    
    // Update issued books status to available (0)
    readfile("all_books_data.csv");
    for(int i = 0; i < content.size(); i++){
        for(int j = 0; j < isbns.size(); j++){
            if(isbns[j] == content[i][3]){
                content[i][4] = "0"; // Mark book as not issued
            }
        }
    }
    
    writefile(content, "all_books_data.csv");
    if(found == 0) cout<<"User not found.\n";
}

void Librarian :: update_user(string id){
    string upid;
    cout<<"Enter the ID of the user you want to update : ";
    cin>>upid;
    
    cout<<"Enter the serial number of the field you want to update : \n";
    cout<<"1. Password\n";
    cout<<"2. Name of the User\n";
    char c;
    cin>>c;
    
    content.clear();
    int fl = 0;
    readfile("all_users_data.csv");
    
    for(int i = 0; i < content.size(); i++){
        if(content[i][1] == upid){
            fl = 1;
            string new_field;
            cout<<"Enter the new value of the field : ";
            cin.ignore();
            getline(cin, new_field);
            
            if(c == '1') content[i][2] = new_field; // Update password
            else if(c == '2') content[i][0] = new_field; // Update name
            else cout<<"Invalid input!\n\n";
            break;
        }
    }
    
    cout<<"User updated\n";
    writefile(content, "all_users_data.csv");
    content.clear();
    if(fl == 0) cout<<"User was not found.\n\n";
}

// Function to update book details
void Librarian :: update_book(string id){
    string isbn_no; // Variable to store the ISBN of the book
    cout<<"Enter the isbn number of the book you want to update : ";
    cin>>isbn_no; // Taking ISBN input from the user

        // Prompting the user to select the field they want to update
    cout<<"Enter the serial number of the field you want to update : \n";
    cout<<"1. Name of the Book\n";
    cout<<"2. Name of the Author\n";
    cout<<"3. Name of the Publisher\n";

    char c;
    cin>>c;

    content.clear(); // Clearing the content vector to avoid leftover data
    int fl=0; // Flag to check if the book was found
    string new_field; // Variable to store the new value for the selected field

    // Reading the list of all books from the file
    readfile("all_books_data.csv");

    // Searching for the book with the provided ISBN
    for(int i=0;i<content.size();i++){
        if(content[i][3]==isbn_no) // Checking if the ISBN matches
        {
            fl=1; // Setting the flag to 1 if the book is found
            cout<<"Enter the new value of the field : ";
            cin.ignore(); // Clearing the buffer
            getline(cin,new_field); // Taking the new field value as input
            if(c=='1'){
                content[i][0]=new_field;
            }
            else if(c=='2'){
                content[i][1]=new_field;
            }
            else if(c=='3'){
                content[i][2]=new_field;
            }
            else{
                cout<<"Invalid input!\n\n";
            }
            break;
        }
    }
    writefile(content,"all_books_data.csv");
    content.clear();
    
    readfile("issued_books_data.csv");
    for(int i=0;i<content.size();i++){
        if(content[i][2]==isbn_no){
            fl=1;
            if(c=='1'){
                content[i][1]=new_field;
            }
            break;
        }
    }
    cout<<"Book updated\n";
    writefile(content,"issued_books_data.csv");

    if(fl==0) cout<<"Book was not found.\n\n";
}
void Librarian :: see_issued_to_user(string id,string uid){
    vector <string> isbns;
    content.clear();
    int count=1;
    readfile("issued_books_data.csv");
    for(int i=0;i<content.size();i++){
        if(content[i][0]==uid){
            isbns.push_back(content[i][2]);
        }
    }
    content.clear();
    if(isbns.size()==0) cout<<"No book is issued to this user.\n\n";
    else{
        readfile("all_books_data.csv");
        for(int d=0;d<isbns.size();d++){
            for(int i=0;i<content.size();i++){
                if(content[i][3]==isbns[d]){
                    cout<<count<<". ";
                    count++;
                    for(auto y:content[i]){
                        if(y!=content[i].back())
                            cout<<y<<" | ";
                        else
                            cout<<y<<"\n";
                    }
                }
            }
        }
        content.clear();
    }
}
void Librarian :: see_issued_book(string id,string isbn){
    content.clear();
    int coun=0;
    readfile("issued_books_data.csv");
    for(int i=0;i<content.size();i++){
        coun=1;
        if(content[i][2]==isbn){
            cout<<"Book with id "<<isbn<<" and name "<<content[i][1]<<" is issued to user with id : "<<content[i][0]<<"\n";
        }
    }
    if(coun==0) cout<<"Book has not been issued to anyone.\n\n";
}

int main(){
    User system;
    system.display_menu();
    return 0;
}