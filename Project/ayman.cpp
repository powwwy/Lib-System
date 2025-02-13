#include <bits/stdc++.h>
#include <chrono>
using namespace std;

vector<vector<string>> content; // Stores the entire content of the file as a 2D vector
vector<string> toupdate; // Stores data to be updated

// Function to read a CSV file and store its contents in 'content' vector
void readfile(string fname){ 
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

// Function to write data to a file (overwrites existing file)
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

// Function to append data to an existing file
void writefileappend(vector<string> par, string fname){  
    fstream fout(fname,ios::out | ios::app);
    for(auto x:par){
        fout<<x;
        if(x!=par.back()) fout<<",";
    }
    fout<<"\n";
}

// Function to print all data from the 'content' vector
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

// Function to print data excluding passwords (assuming password is at index 2)
void printdata_notpassword(vector<vector<string>> par){
    int c=1;
    for(auto x:par){
        cout<<c<<". ";
        cout<<x[0]<<" | "<<x[1]<<" | "<<x[3]<<'\n';
        c++;
    }
}

// User class to handle user-related functionalities
class User{
    private:
        string password; // Private member for user password
    public:
        string name;
        string id;
        vector<string> isbns_issued; // List of issued book ISBNs
        
        // User functionalities
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

// Book class to manage book-related operations
class Book{
    public:
        string title;
        string author;
        string isbn;
        string publication;
        int is_issued; // Flag to indicate if the book is issued (1: issued, 0: available)
    
    // Book functionalities
    void Book_request(string id,string bookname,string type_user);
    void Show_duedate(string isbn_no);
    void Book_add(string id);
    void Book_update(string id);
    void Book_delete(string id);
    void Book_search(string id);
};

// Student class inheriting from User class
class Student : public User{
    // Additional functionalities specific to students can be added here
};