#include <bits/stdc++.h>
#include <chrono>
#include <iostream>
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
                fout << ","; // Add a comma between words
        }
        fout << "\n"; // Add a newline after each row
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
            fout << ","; // Add a comma between words
    }
    fout << "\n"; // Add a newline at the end of the row
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
class User
{
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
    void issue_book(string id, string bookname, string type_user);
    void return_book(string id, string isbncode);
    void check_available(string id, string bookname);
    int calc_fine(string id, string type_user);
    void clear_fine_amount(string id, string type_user);
    void User_add(string id);
    void User_update(string id);
    void User_delete(string id);
    void User_search(string id);
};

// Book class to handle book-related tasks
class Book
{
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
void Student::display_student_menu(string id)
{
    cout << "---------------------------------------------------------------------\n";
    cout << "\nYou have been logged in as Student\n";
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

// Function to log in a user (checks ID and password)
void User::login()
{
    string id, password;
    cout << "Enter your id: ";
    cin >> id; // Get user ID
    cout << "Enter the password: ";
    cin >> password;               // Get user password
    vector<string> words_in_a_row; // To store words from the CSV file
    string line_in_csv, word;
    fstream file("all_users_data.csv", ios::in); // Open the CSV file with user data
    int count = 0;
    if (file.is_open())
    { // If file is successfully opened
        while (getline(file, line_in_csv))
        {                                  // Read each line
            words_in_a_row.clear();        // Clear previous row data
            stringstream str(line_in_csv); // Create stringstream to split the line
            while (getline(str, word, ','))
                words_in_a_row.push_back(word); // Split by commas
            if (words_in_a_row[1] == id)
            { // Check if ID matches
                count = 1;
                if (words_in_a_row[2] == password)
                {            // Check if password matches
                    count++; // User is logged in
                    break;
                }
                else
                { // Password mismatch
                    while (password != words_in_a_row[2])
                    {
                        cout << "You entered the wrong password. Press 1 to reenter the password and 2 to exit\n";
                        char c;
                        cin >> c; // Read input
                        if (c == '1')
                        {
                            cout << "Enter the password: ";
                            cin >> password; // Get the new password
                            if (password == words_in_a_row[2])
                                count++; // Check if password is correct
                        }
                        else if (c == '2')
                        {
                            cout << "Exiting..."; // Exit the login process
                            return;
                        }
                        else
                        {
                            cout << "Please enter a valid input.\n"; // Invalid input
                        }
                    }
                }
            }
        }
    }
}

void User ::display_menu()
{
    char c;
    cout << "---------------------------------------------------------------------\n";
    cout << "\n\nWelcome to the library management system!\n\n";
    cout << "1. Press 1 to log in : \n";
    cout << "2. Press 2 to exit\n";
    cout << "---------------------------------------------------------------------\n";
    cin >> c;
    if (c == '1')
    { // logs in a user
        User u;
        u.login();
    }
    else
    {
        cout << "Thanks for using the system !";
        exit(1);
    }
}
void User ::login()
{

    string id, password;
    cout << "Enter your id : ";
    cin >> id;
    cout << "Enter the password : ";
    cin >> password;
    vector<string> words_in_a_row;
    string line_in_csv, word;
    fstream file("all_users_data.csv", ios::in);
    int count = 0;
    if (file.is_open())
    {
        while (getline(file, line_in_csv))
        {
            words_in_a_row.clear();
            stringstream str(line_in_csv);
            while (getline(str, word, ','))
                words_in_a_row.push_back(word);
            if (words_in_a_row[1] == id)
            {
                count = 1;
                if (words_in_a_row[2] == password)
                {
                    count++;
                    break;
                }
                else
                {
                    while (password != words_in_a_row[2])
                    {
                        cout << "You entered the wrong password. Press 1 to reenter the password and 2 to exit\n";
                        char c;
                        cin >> c;
                        if (c == '1')
                        {
                            cout << "Enter the password : ";
                            cin >> password;
                            if (password == words_in_a_row[2])
                                count++;
                        }
                        else if (c == '2')
                        {
                            cout << "Exiting...";
                            return;
                        }
                        else
                        {
                            cout << "Please enter a valid input.\n";
                        }
                    }
<<<<<<< HEAD
=======

                    switch (c)
                    {
                    case '1':
                        s.see_all_books(id);     // Call function to display all books in the library
                        s.display_prof_menu(id); // Return to the professor menu after displaying books
                        break;
                    case '2':
                        s.see_issued_books(id);  // Call function to display books issued by the professor
                        s.display_prof_menu(id); // Return to the professor menu
                        break;
                    case '3':
                        cout << "Enter the name of the book you want to check : ";
                        cin >> book_name;                 // Get the name of the book to check availability
                        s.check_available(id, book_name); // Call function to check if the book is available
                        s.display_prof_menu(id);          // Return to the professor menu
                        break;
                    case '4':
                        s.calc_fine(id, "2");    // Call function to calculate and display the fine amount
                        s.display_prof_menu(id); // Return to the professor menu
                        break;
                    case '5':
                        cout << "Enter the name of the book you want to issue : ";
                        cin >> book_name;                 // Get the name of the book to issue
                        s.issue_book(id, book_name, "2"); // Call function to issue the book
                        s.display_prof_menu(id);          // Return to the professor menu
                        break;
                    case '6':
                        cout << "Enter the isbn code of the book you want to return : ";
                        cin >> isbncode;             // Get the ISBN code of the book to return
                        s.return_book(id, isbncode); // Call function to return the book
                        s.display_prof_menu(id);     // Return to the professor menu
                        break;
                    case '7':
                        s.clear_fine_amount(id, "2"); // Call function to clear the fine amount
                        s.display_prof_menu(id);      // Return to the professor menu
                        break;
                    case '8':
                        logout(); // Call function to log out the professor
                        break;
                    }

                    // Function to display the main menu for the library management system
                    void User::display_menu()
                    {
                        char c;
                        cout << "---------------------------------------------------------------------\n";
                        cout << "\n\nWelcome to the library management system!\n\n";
                        cout << "1. Press 1 to log in : \n";
                        cout << "2. Press 2 to exit\n";
                        cout << "---------------------------------------------------------------------\n";
                        cin >> c; // Get user input for menu selection

                        if (c == '1')
                        {              // If the user chooses to log in
                            User u;    // Create a User object
                            u.login(); // Call the login function
                        }
                        else
                        { // If the user chooses to exit
                            cout << "Thanks for using the system !";
                            exit(1); // Exit the program
                        }
                    }

                    // Function to handle user login
                    void User::login()
                    {
                        string id, password;
                        cout << "Enter your id : ";
                        cin >> id; // Get the user's ID
                        cout << "Enter the password : ";
                        cin >> password; // Get the user's password

                        vector<string> words_in_a_row;               // Vector to store data from a row in the CSV file
                        string line_in_csv, word;                    // Variables to read lines and words from the CSV file
                        fstream file("all_users_data.csv", ios::in); // Open the CSV file for reading
                        int count = 0;                               // Counter to track if the ID and password are found

                        if (file.is_open())
                        { // Check if the file was successfully opened
                            while (getline(file, line_in_csv))
                            {                                  // Read each line from the file
                                words_in_a_row.clear();        // Clear the vector for the new line
                                stringstream str(line_in_csv); // Convert the line into a string stream
                                while (getline(str, word, ','))
                                {                                   // Split the line by commas
                                    words_in_a_row.push_back(word); // Add each word to the vector
                                }

                                if (words_in_a_row[1] == id)
                                {              // Check if the ID matches
                                    count = 1; // Set count to 1 if the ID is found
                                    if (words_in_a_row[2] == password)
                                    {            // Check if the password matches
                                        count++; // Set count to 2 if the password is correct
                                        break;   // Exit the loop if login is successful
                                    }
                                    else
                                    { // If the password is incorrect
                                        while (password != words_in_a_row[2])
                                        { // Loop until the correct password is entered
                                            cout << "You entered the wrong password. Press 1 to reenter the password and 2 to exit\n";
                                            char c;
                                            cin >> c; // Get user input for retrying or exiting
                                            if (c == '1')
                                            { // If the user chooses to retry
                                                cout << "Enter the password : ";
                                                cin >> password; // Get the password again
                                                if (password == words_in_a_row[2])
                                                {            // Check if the password is correct
                                                    count++; // Set count to 2 if the password is correct
                                                    break;   // Exit the loop
                                                }
                                            }
                                            else if (c == '2')
                                            { // If the user chooses to exit
                                                cout << "Exiting...";
                                                return; // Exit the function
                                            }
                                            else
                                            { // If the user enters an invalid input
                                                cout << "Please enter a valid input.\n";
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

                    writefile(content, "all_books_data.csv");
                    content.clear();
                    readfile("issued_books_data.csv");
                    for (int i = 0; i < content.size(); i++)
                    {
                        if (content[i][2] == isbnno)
                        {
                            content.erase(content.begin() + i, content.begin() + i + 1);
                            break;
                        }
                    }
                    writefile(content, "issued_books_data.csv");
                    content.clear();
                    if (found == 0)
                        cout << "Book not found.\n";
                }
                void Librarian ::delete_user(string id)
                {
                    string uid;
                    cout << "Enter the user id of the user : ";
                    cin >> uid;
                    content.clear();
                    int found = 0;
                    readfile("all_users_data.csv");
                    for (int i = 0; i < content.size(); i++)
                    {
                        if (content[i][1] == uid)
                        {
                            found = 1;
                            content.erase(content.begin() + i, content.begin() + i + 1);
                            break;
                        }
                    }
                    writefile(content, "all_users_data.csv");
                    content.clear();
                    vector<string> isbns;
                    readfile("issued_books_data.csv");
                    for (int i = 0; i < content.size(); i++)
                    {
                        if (content[i][0] == uid)
                        {
                            isbns.push_back(content[i][2]);
                            content.erase(content.begin() + i, content.begin() + i + 1);
                            break;
                        }
                    }
                    writefile(content, "issued_books_data.csv");
                    content.clear();

                    readfile("all_books_data.csv");
                    for (int i = 0; i < content.size(); i++)
                    {
                        for (int j = 0; j < isbns.size(); j++)
                        {
                            if (isbns[j] == content[i][3])
                            {
                                content[i][4] = "0";
                            }
                        }
                    }
                    writefile(content, "all_books_data.csv");
                    if (found == 0)
                        cout << "User not found.\n";
                }
                void Librarian ::update_user(string id)
                {
                    string upid;
                    cout << "Enter the id of the user you want to update : ";
                    cin >> upid;
                    cout << "Enter the serial number of the field you want to update : \n";
                    cout << "1. Password\n";
                    cout << "2. Name of the User\n";
                    char c;
                    cin >> c;
                    content.clear();
                    int fl = 0;
                    readfile("all_users_data.csv");
                    for (int i = 0; i < content.size(); i++)
                    {
                        if (content[i][1] == upid)
                        {
                            fl = 1;
                            string new_field;
                            cout << "Enter the new value of the field : ";
                            cin.ignore();
                            getline(cin, new_field);
                            if (c == '1')
                            {
                                content[i][2] = new_field;
                            }
                            else if (c == '2')
                            {
                                content[i][0] = new_field;
                            }
                            else
                            {
                                cout << "Invalid input!\n\n";
                            }
                            break;
                        }
                    }
                    cout << "User updated\n";
                    writefile(content, "all_users_data.csv");
                    content.clear();
                    if (fl == 0)
                        cout << "User was not found.\n\n";
                }




// Function to update book details
void Librarian::update_book(string id)
{
    string isbn_no;  // Variable to store the ISBN of the book
    cout << "Enter the isbn number of the book you want to update : ";
    cin >> isbn_no;  // Taking ISBN input from the user
    
    // Prompting the user to select the field they want to update
    cout << "Enter the serial number of the field you want to update : \n";
    cout << "1. Name of the Book\n";
    cout << "2. Name of the Author\n";
    cout << "3. Name of the Publisher\n";
    
    char c;  // Variable to store user choice
    cin >> c;  // Taking choice input from the user
        content.clear();  // Clearing the content vector to avoid leftover data

    int fl = 0;  // Flag to check if the book was found
    string new_field;  // Variable to store the new value for the selected field
    
    // Reading the list of all books from the file
    readfile("all_books_data.csv");

    // Searching for the book with the provided ISBN
    for (int i = 0; i < content.size(); i++)
    {
        if (content[i][3] == isbn_no)  // Checking if the ISBN matches
        {
            fl = 1;  // Setting the flag to 1 if the book is found
            cout << "Enter the new value of the field : ";
            cin.ignore();  // Clearing the buffer
            getline(cin, new_field);  // Taking the new field value as input
            
            // Updating the chosen field based on the user's choice
            if (c == '1')
            {
                content[i][0] = new_field;  // Updating the name of the book
            }
            else if (c == '2')
            {
                content[i][1] = new_field;  // Updating the author's name
            }
            else if (c == '3')
            {
                content[i][2] = new_field;  // Updating the publisher's name
            }
            else
            {
                cout << "Invalid input!\n\n";  // Handling invalid input
            }
            break;  // Exit the loop after updating the book
        }
    }
    
    // Writing the updated content back to the all_books_data.csv file
    writefile(content, "all_books_data.csv");
    content.clear();  // Clearing the content vector for the next operation
    
    // Reading the list of issued books from the file
    readfile("issued_books_data.csv");

    // Searching for the issued book with the given ISBN
    for (int i = 0; i < content.size(); i++)
    {
        if (content[i][2] == isbn_no)  // Checking if the ISBN matches the issued books
        {
            fl = 1;  // Setting the flag if the book is found in issued books
            // If the user updated the book name, we update the issued book's name
            if (c == '1')
            {
                content[i][1] = new_field;  // Updating the book name in issued books
            }
            break;  // Exit the loop after updating the issued book
        }
    }
    
    // Writing the updated issued books list back to the file
    cout << "Book updated\n";  // Confirmation message
    writefile(content, "issued_books_data.csv");

    // If the book was not found in either of the files, display an error
    if (fl == 0)
        cout << "Book was not found.\n\n";
}

// Function to see the books issued to a specific user
void Librarian::see_issued_to_user(string id, string uid)
{
    vector<string> isbns;  // Vector to store ISBNs of books issued to the user
    content.clear();  // Clearing the content vector
    int count = 1;  // Variable to count the issued books
    
    // Reading the list of issued books from the file
    readfile("issued_books_data.csv");

    // Searching for the user's issued books
    for (int i = 0; i < content.size(); i++)
    {
        if (content[i][0] == uid)  // Checking if the user ID matches
        {
            isbns.push_back(content[i][2]);  // Adding the ISBN of issued books to the list
        }
    }
    
    content.clear();  // Clearing content after gathering issued books
    
    // If no books are found, display a message
    if (isbns.size() == 0)
        cout << "No book is issued to this user.\n\n";
    else
    {
        // Reading all books data to fetch book details
        readfile("all_books_data.csv");
        
        // For each ISBN of the issued books, display the book details
        for (int d = 0; d < isbns.size(); d++)
        {
            for (int i = 0; i < content.size(); i++)
            {
                if (content[i][3] == isbns[d])  // If the ISBN matches
                {
                    cout << count << ". ";
                    count++;
                    // Display the book's details
                    for (auto y : content[i])
                    {
                        if (y != content[i].back())
                            cout << y << " | ";  // Print the fields separated by '|'
                        else
                            cout << y << "\n";  // Print the last field without '|'
                    }
                }
            }
        }
        content.clear();  // Clearing the content vector after displaying details
    }
}

// Function to check if a specific book is issued to anyone
void Librarian::see_issued_book(string id, string isbn)
{
    content.clear();  // Clearing the content vector
    int coun = 0;  // Variable to track if the book is found
    readfile("issued_books_data.csv");  // Reading the issued books data file

    // Searching for the book with the given ISBN in the issued books list
    for (int i = 0; i < content.size(); i++)
    {
        coun = 1;  // Set flag to 1 when book is found
        if (content[i][2] == isbn)  // If ISBN matches
        {
            cout << "Book with id " << isbn << " and name " << content[i][1]
                 << " is issued to user with id : " << content[i][0] << "\n";  // Display the issued book info
        }
    }
    
    // If the book was not found, display a message
    if (coun == 0)
        cout << "Book has not been issued to anyone.\n\n";
}

// Main function
int main()
{
    User temp;  // Creating an instance of the User class
    temp.display_menu();  // Calling the display_menu method of the User class
    return 0;  // Returning 0 to indicate successful execution
}















                void Librarian ::update_book(string id)
                {
                    string isbn_no;
                    cout << "Enter the isbn number of the book you want to update : ";
                    cin >> isbn_no;
                    cout << "Enter the serial number of the field you want to update : \n";
                    cout << "1. Name of the Book\n";
                    cout << "2. Name of the Author\n";
                    cout << "3. Name of the Publisher\n";
                    char c;
                    cin >> c;
                    content.clear();
                    int fl = 0;
                    string new_field;
                    readfile("all_books_data.csv");
                    for (int i = 0; i < content.size(); i++)
                    {
                        if (content[i][3] == isbn_no)
                        {
                            fl = 1;
                            cout << "Enter the new value of the field : ";
                            cin.ignore();
                            getline(cin, new_field);
                            if (c == '1')
                            {
                                content[i][0] = new_field;
                            }
                            else if (c == '2')
                            {
                                content[i][1] = new_field;
                            }
                            else if (c == '3')
                            {
                                content[i][2] = new_field;
                            }
                            else
                            {
                                cout << "Invalid input!\n\n";
                            }
                            break;
                        }
                    }
                    writefile(content, "all_books_data.csv");
                    content.clear();

                    readfile("issued_books_data.csv");
                    for (int i = 0; i < content.size(); i++)
                    {
                        if (content[i][2] == isbn_no)
                        {
                            fl = 1;
                            if (c == '1')
                            {
                                content[i][1] = new_field;
                            }
                            break;
                        }
                    }
                    cout << "Book updated\n";
                    writefile(content, "issued_books_data.csv");

                    if (fl == 0)
                        cout << "Book was not found.\n\n";
                }
                void Librarian ::see_issued_to_user(string id, string uid)
                {
                    vector<string> isbns;
                    content.clear();
                    int count = 1;
                    readfile("issued_books_data.csv");
                    for (int i = 0; i < content.size(); i++)
                    {
                        if (content[i][0] == uid)
                        {
                            isbns.push_back(content[i][2]);
                        }
                    }
                    content.clear();
                    if (isbns.size() == 0)
                        cout << "No book is issued to this user.\n\n";
                    else
                    {
                        readfile("all_books_data.csv");
                        for (int d = 0; d < isbns.size(); d++)
                        {
                            for (int i = 0; i < content.size(); i++)
                            {
                                if (content[i][3] == isbns[d])
                                {
                                    cout << count << ". ";
                                    count++;
                                    for (auto y : content[i])
                                    {
                                        if (y != content[i].back())
                                            cout << y << " | ";
                                        else
                                            cout << y << "\n";
                                    }
                                }
                            }
                        }
                        content.clear();
                    }
                }
                void Librarian ::see_issued_book(string id, string isbn)
                {
                    content.clear();
                    int coun = 0;
                    readfile("issued_books_data.csv");
                    for (int i = 0; i < content.size(); i++)
                    {
                        coun = 1;
                        if (content[i][2] == isbn)
                        {
                            cout << "Book with id " << isbn << " and name " << content[i][1] << " is issued to user with id : " << content[i][0] << "\n";
                        }
                    }
                    if (coun == 0)
                        cout << "Book has not been issued to anyone.\n\n";
                }

                int main()
                {
                    User temp;
                    temp.display_menu();
                    return 0;
>>>>>>> c7d87ffcfe46f5803614086331da4ce658cba46c
                }