#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib> // for system()
using namespace std;

struct Student {
    string studentNumber;
    string name;
    string password;
};

bool registerStudent() {
    Student s;
    cout << "\n--- Student Registration ---\n";
    cout << "Enter Student Number: ";
    cin >> s.studentNumber;

    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, s.name);

    cout << "Enter Password: ";
    cin >> s.password;

    // Check if exists
    ifstream infile("students.txt");
    string sn, nm, pw;
    while (infile >> sn >> nm >> pw) {
        if (sn == s.studentNumber) {
            cout << "❌ Student already exists!\n";
            return false;
        }
    }
    infile.close();

    ofstream outfile("students.txt", ios::app);
    outfile << s.studentNumber << " " << s.name << " " << s.password << endl;
    outfile.close();

    cout << "✅ Registration successful!\n";
    return true;
}

bool loginStudent() {
    string snInput, pwInput;

    cout << "\n--- Student Login ---\n";
    cout << "Enter Student Number: ";
    cin >> snInput;
    cout << "Enter Password: ";
    cin >> pwInput;

    ifstream infile("students.txt");
    string sn, nm, pw;
    while (infile >> sn >> nm >> pw) {
        if (sn == snInput && pw == pwInput) {
            cout << "✅ Welcome " << nm << "! Login successful.\n";
            infile.close();

            // Call menu.cpp program
        #ifdef _WIN32
            system("menu.exe");  // Windows
        #else
            system("./menu");    // Linux / Mac
        #endif
            return true;
        }
    }
    infile.close();

    cout << "❌ Incorrect student number or password.\n";
    return false;
}

int main() {
    int choice;

    while (true) {
        cout << "\n=== Secondary Education Learning System ===\n";
        cout << "1. Login\n";
        cout << "2. Register\n";
        cout << "3. Exit\n";
        cout << "Choose an option: ";
        cin >> choice;

        if (choice == 1) {
            loginStudent();
        } 
        else if (choice == 2) {
            registerStudent();
        } 
        else if (choice == 3) {
            cout << "Goodbye!\n";
            break;
        } 
        else {
            cout << "Invalid choice!\n";
        }
    }

    return 0;
}