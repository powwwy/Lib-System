#include <bits/stdc++.h>
#include <chrono>
#include <iostream>
using namespace std;

// Global variables to hold data
vector<vector<string>> content; // To store the CSV file content
vector<string> toupdate; // To store data to be updated

// Function to read data from a CSV file and store it into 'content' variable
void readfile(string fname) { 
    vector<string> row; // To store each row of data
    string line, word; // Variables to handle each line and word in the file

    fstream file (fname, ios::in); // Open the file in read mode
    if(file.is_open()) { // Check if file is successfully opened
        while(getline(file, line)) { // Read each line of the file
            row.clear(); // Clear previous row data
            stringstream str(line); // Create stringstream to split the line by commas
            while(getline(str, word, ',')) // Split the line by commas
                row.push_back(word); // Store words in the row
            content.push_back(row); // Store the row in content
        }
    } else {
        cout << "Could not open the file\n"; // Handle file open failure
    }
}

// Function to write the entire content of the vector 'par' to a CSV file
void writefile(vector<vector<string>> par, string fname) {  
    fstream fout(fname, ios::out); // Open the file in write mode
    for(auto x: par) { // Iterate through each row in the 'par' vector
        for(auto y: x) { // Iterate through each word in the row
            fout << y; // Write the word to the file
            if(y != x.back()) fout << ","; // Add a comma between words
        }
        fout << "\n"; // Add a newline after each row
    }
}

// Function to append a new line to a CSV file
void writefileappend(vector<string> par, string fname) {  
    fstream fout(fname, ios::out | ios::app); // Open file in append mode
    for(auto x: par) { // Iterate through each word in the row
        fout << x; // Write the word to the file
        if(x != par.back()) fout << ","; // Add a comma between words
    }
    fout << "\n"; // Add a newline at the end of the row
}

// Function to print the data stored in a vector
void printdata(vector<vector<string>> par) {
    int c = 1; // Counter for row numbers
    for(auto x: par) { // Iterate through each row
        cout << c << ". "; // Print the row number
        for(auto y: x) { // Iterate through each word in the row
            cout << y;
            if(y != x.back()) cout << " | "; // Print words with separator '|' between them
        }
        c++; // Increment the counter
        cout << "\n"; // Newline after each row
    }
}

// Function to print data without showing passwords
void printdata_notpassword(vector<vector<string>> par) {
    int c = 1; // Counter for row numbers
    for(auto x: par) { // Iterate through each row
        cout << c << ". ";
        // Print only the necessary details (excluding password)
        cout << x[0] << " | " << x[1] << " | " << x[3] << '\n';
        c++; // Increment the counter
    }
}

// User class that contains common user functionalities like login and issuing/returning books
class User {
private:
    string password; // User's password (private to the class)
public:
    string name; // User's name
    string id; // User's ID
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
class Book {
public:
    string title; // Book title
    string author; // Book author
    string isbn; // Book ISBN
    string publication; // Book publication name
    int is_issued; // Book issue status (1 = issued, 0 = not issued)

    // List of methods related to book functionalities
    void Book_request(string id, string bookname, string type_user);
    void Show_duedate(string isbn_no);
    void Book_add(string id);
    void Book_update(string id);
    void Book_delete(string id);
    void Book_search(string id);
};

// Student class inherits from User, adds student-specific functionalities
class Student : public User {
public:
    int Fine_amount; // Fine amount for the student
    void display_student_menu(string id);
};

// Professor class inherits from User, adds professor-specific functionalities
class Professor : public User {
public:
    int Fine_amount; // Fine amount for the professor
    void display_prof_menu(string id);
};

// Librarian class inherits from User, adds librarian-specific functionalities
class Librarian : public User {
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
void Student::display_student_menu(string id) {
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
    switch(c) {
        case '1':
            s.see_all_books(id); // Show all books
            s.display_student_menu(id); // Re-display the menu
            break;
        case '2':
            s.see_issued_books(id); // Show books issued by the student
            s.display_student_menu(id); // Re-display the menu
            break;
        case '3':
            cout << "Enter the name of the book you want to check: ";
            cin.ignore();
            getline(cin, book_name); // Get the book name from user input
            s.check_available(id, book_name); // Check if the book is available
            s.display_student_menu(id); // Re-display the menu
            break;
        case '4':
            s.calc_fine(id, "1"); // Calculate fine for student
            s.display_student_menu(id); // Re-display the menu
            break;
        case '5':
            cout << "Enter the name of the book you want to issue: ";
            cin.ignore();
            getline(cin, book_name); // Get the book name from user input
            b.Book_request(id, book_name, "1"); // Issue the book
            s.display_student_menu(id); // Re-display the menu
            break;
        case '6':
            cout << "Enter the isbn code of the book you want to return: ";
            cin >> isbncode; // Get the ISBN code from user input
            s.return_book(id, isbncode); // Return the book
            s.display_student_menu(id); // Re-display the menu
            break;
        case '7':
            s.clear_fine_amount(id, "1"); // Clear fine for student
            s.display_student_menu(id); // Re-display the menu
            break;
        case '8':
            logout(); // Logout the user
            break;
    }
}

// Display menu for professor users
void Professor::display_prof_menu(string id) {
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
    switch(c) {
        case '1':
            s.see_all_books(id); // Show all books
            s.display_prof_menu(id); // Re-display the menu
            break;
        case '2':
            s.see_issued_books(id); // Show books issued by the professor
            s.display_prof_menu(id); // Re-display the menu
            break;
        case '3':
            cout << "Enter the name of the book you want to check: ";
            cin >> book_name; // Get the book name from user input
            s.check_available(id, book_name); // Check if the book is available
            s.display_prof_menu(id); // Re-display the menu
            break;
        case '4':
            s.calc_fine(id, "2"); // Calculate fine for professor
            s.display_prof_menu(id); // Re-display the menu
            break;
        case '5':
            cout << "Enter the name of the book you want to issue: ";
            cin >> book_name; // Get the book name from user input
            s.issue_book(id, book_name, "2"); // Issue the book
            s.display_prof_menu(id); // Re-display the menu
            break;
        case '6':
            cout << "Enter the isbn code of the book you want to return: ";
            cin >> isbncode; // Get the ISBN code from user input
            s.return_book(id, isbncode); // Return the book
            s.display_prof_menu(id); // Re-display the menu
            break;
        case '7':
            s.clear_fine_amount(id, "2"); // Clear fine for professor
            s.display_prof_menu(id); // Re-display the menu
            break;
        case '8':
            logout(); // Logout the user
            break;
    }
}

// Function to display the main menu for the user
void User::display_menu() {
    char c;
    cout << "---------------------------------------------------------------------\n";
    cout << "\n\nWelcome to the library management system!\n\n";
    cout << "1. Press 1 to log in : \n";
    cout << "2. Press 2 to exit\n";
    cout << "---------------------------------------------------------------------\n";
    cin >> c; // Read user input
    if(c == '1') { // If user chooses to log in
        User u;
        u.login(); // Call login function
    } else {
        cout << "Thanks for using the system!\n"; // If user chooses to exit
        exit(1); // Exit the program
    }
}

// Function to log in a user (checks ID and password)
void User::login() {
    string id, password;
    cout << "Enter your id: ";
    cin >> id; // Get user ID
    cout << "Enter the password: ";
    cin >> password; // Get user password
    vector<string> words_in_a_row; // To store words from the CSV file
    string line_in_csv, word;
    fstream file("all_users_data.csv", ios::in); // Open the CSV file with user data
    int count = 0;
    if(file.is_open()) { // If file is successfully opened
        while(getline(file, line_in_csv)) { // Read each line
            words_in_a_row.clear(); // Clear previous row data
            stringstream str(line_in_csv); // Create stringstream to split the line
            while(getline(str, word, ',')) words_in_a_row.push_back(word); // Split by commas
            if(words_in_a_row[1] == id) { // Check if ID matches
                count = 1;
                if(words_in_a_row[2] == password) { // Check if password matches
                    count++; // User is logged in
                    break;
                } else { // Password mismatch
                    while(password != words_in_a_row[2]) {
                        cout << "You entered the wrong password. Press 1 to reenter the password and 2 to exit\n";
                        char c;
                        cin >> c; // Read input
                        if(c == '1') {
                            cout << "Enter the password: ";
                            cin >> password; // Get the new password
                            if(password == words_in_a_row[2]) count++; // Check if password is correct
                        } else if(c == '2') {
                            cout << "Exiting..."; // Exit the login process
                            return;
                        } else {
                            cout << "Please enter a valid input.\n"; // Invalid input
                        }
                    }
                }
            }
        }
    }
}
#include <bits/stdc++.h>
#include <chrono>
using namespace std;

vector<vector<string>> content;
vector<string> toupdate;

void readfile(string fname){ //reference : https://java2blog.com/read-csv-file-in-cpp/
    vector<string> row;
    string line, word;

    fstream file (fname,ios::in);
    if(file.is_open()){
        while(getline(file, line)){
            row.clear();
            stringstream str(line);
            while(getline(str, word, ',')) row.push_back(word);
            content.push_back(row);
        }
    }
    else cout<<"Could not open the file\n";
}
void writefile(vector<vector<string>> par, string fname){  
    fstream fout(fname,ios::out);
    for(auto x:par){
        for(auto y:x){
            fout<<y;
            if(y!=x.back()) fout<<",";
        }
        fout<<"\n";
    }

}
void writefileappend(vector<string> par, string fname){  
    fstream fout(fname,ios::out | ios::app);
    for(auto x:par){
        fout<<x;
        if(x!=par.back()) fout<<",";
    }
    fout<<"\n";
}
void printdata(vector<vector<string>> par){
    int c=1;
    for(auto x:par){
        cout<<c<<". ";
        for(auto y:x){
            cout<<y;
            if(y!=x.back()) cout<<" | ";
        }
        c++;
        cout<<"\n";
    }
}
void printdata_notpassword(vector<vector<string>> par){
    int c=1;
    for(auto x:par){
        cout<<c<<". ";
        cout<<x[0]<<" | "<<x[1]<<" | "<<x[3]<<'\n';
        c++;
    }
}

class User{
    private:
        string password;
    public:
        string name;
        string id;
        vector<string> isbns_issued;
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
class Book{
    public:
        string title;
        string author;
        string isbn;
        string publication;
        int is_issued;
    void Book_request(string id,string bookname,string type_user);
    void Show_duedate(string isbn_no);
    void Book_add(string id);
    void Book_update(string id);
    void Book_delete(string id);
    void Book_search(string id);
    
};
class Student : public User{
    public:
        int Fine_amount;
        void display_student_menu(string id);
};
class Professor : public User{
    public:
        int Fine_amount;
    public:
        void display_prof_menu(string id);

};
class Librarian : public User{
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
        void see_issued_to_user(string id,string uid);
        void see_issued_book(string id,string isbn);
        
};

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
    switch(c){
        case '1':
            s.see_all_books(id);
            s.display_student_menu(id);
            break;
        case '2':
            s.see_issued_books(id);
            s.display_student_menu(id);
            break;
        case '3':
            cout<<"Enter the name of the book you want to check : ";
            cin.ignore();
            getline(cin,book_name);
            s.check_available(id,book_name);
            s.display_student_menu(id);
            break;
        case '4':
            s.calc_fine(id,"1");
            s.display_student_menu(id);
            break;
        case '5':
            cout<<"Enter the name of the book you want to issue : ";
            cin.ignore();
            getline(cin,book_name);
            b.Book_request(id,book_name,"1");
            s.display_student_menu(id);            
            break;
        case '6':
            cout<<"Enter the isbn code of the book you want to return : ";
            cin>>isbncode;
            s.return_book(id,isbncode);
            s.display_student_menu(id);
            break;
        case '7':
            s.clear_fine_amount(id,"1");
            s.display_student_menu(id);
            break;           
        case '8':
            logout();
            break;
    }
}

void Professor :: display_prof_menu(string id){
    cout<<"---------------------------------------------------------------------\n";
    cout<<"\nYou have been logged in as Lecturer\n";
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
    Professor s;
    switch(c){
        case '1':
            s.see_all_books(id);
            s.display_prof_menu(id);
            break;
        case '2':
            s.see_issued_books(id);
            s.display_prof_menu(id);
            break;
        case '3':
            cout<<"Enter the name of the book you want to check : ";
            cin>>book_name;
            s.check_available(id,book_name);
            s.display_prof_menu(id);
            break;
        case '4':
            s.calc_fine(id,"2");
            s.display_prof_menu(id);
            break;
        case '5':
            cout<<"Enter the name of the book you want to issue : ";
            cin>>book_name;
            s.issue_book(id,book_name,"2");
            s.display_prof_menu(id);            
            break;
        case '6':
            cout<<"Enter the isbn code of the book you want to return : ";
            cin>>isbncode;
            s.return_book(id,isbncode);
            s.display_prof_menu(id);
            break;
        case '7':
            s.clear_fine_amount(id,"2");
            s.display_prof_menu(id);
            break;
        case '8':
            logout();
            break;
    }
}

void User :: display_menu(){
    char c;
    cout<<"---------------------------------------------------------------------\n";
    cout<<"\n\nWelcome to the library management system!\n\n";
    cout<<"1. Press 1 to log in : \n";
    cout<<"2. Press 2 to exit\n";
    cout<<"---------------------------------------------------------------------\n";
    cin>>c;
    if(c=='1'){ //logs in a user 
        User u;
        u.login();
    }
    else{
        cout<<"Thanks for using the system !";
        exit(1);
    }
}
void User :: login(){

    string id,password;
    cout<<"Enter your id : ";
    cin>>id;
    cout<<"Enter the password : ";
    cin>>password;
    vector<string> words_in_a_row;
    string line_in_csv,word;
    fstream file("all_users_data.csv",ios::in);
    int count=0;
    if(file.is_open()){
        while(getline(file,line_in_csv)){
            words_in_a_row.clear();
            stringstream str(line_in_csv);
            while(getline(str,word,',')) words_in_a_row.push_back(word);
            if(words_in_a_row[1]==id){
                count=1;
                if(words_in_a_row[2]==password){
                    count++;
                    break;
                }
                else{
                    while(password!=words_in_a_row[2]){
                        cout<<"You entered the wrong password. Press 1 to reenter the password and 2 to exit\n";
                        char c;
                        cin>>c;
                        if(c=='1'){
                            cout<<"Enter the password : ";
                            cin>>password;
                            if(password==words_in_a_row[2]) count++;
                        }
                        else if(c=='2'){
                            cout<<"Exiting...";
                            return;
                        }
                        else{
                            cout<<"Please enter a valid input.\n";
                        }
                    }
                }