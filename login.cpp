#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct Student {
    string studentNumber;
    string name;
    string password;
};

vector<Student> students;

// ---------------------- LOAD & SAVE --------------------------
void loadStudents() {
    ifstream infile("students.txt");
    string sn, nm, pw;
    students.clear();

    string header;
    getline(infile, header); // skip header

    while (infile >> sn) {
        infile >> ws;
        getline(infile, nm, '\n');
        size_t pos = nm.rfind(' ');
        if (pos != string::npos) {
            pw = nm.substr(pos + 1);
            nm = nm.substr(0, pos);
        } else {
            pw = "";
        }

        students.push_back({sn, nm, pw});
    }
    infile.close();
}

void saveStudents() {
    ofstream outfile("students.txt");
    outfile << "StudentID Name Password\n";
    for (auto &s : students) {
        outfile << s.studentNumber << " " << s.name << " " << s.password << endl;
    }
    outfile.close();
}

// ---------------------- REGISTER --------------------------
bool registerStudent() {
    Student s;
    cout << "\n--- Student Registration ---\n";
    cout << "Enter Student Number: ";
    cin >> s.studentNumber;

    for (char &c : s.studentNumber) c = toupper(c);

    cin.ignore();
    cout << "Enter Full Name: ";
    getline(cin, s.name);

    // PASSWORD INPUT LOOP
    cout << "Enter Password: ";
    while (true) {
        getline(cin, s.password);

        if (s.password.length() < 4) {
            cout << "❌ Password must be at least 4 characters. Try again: ";
        } else if (s.password.find(" ") != string::npos) {
            cout << "❌ Password cannot contain spaces. Try again: ";
        } else {
            break;
        }
    }

    // Check duplicate
    for (auto &st : students) {
        if (st.studentNumber == s.studentNumber) {
            cout << "❌ Student already exists!\n";
            return false;
        }
    }

    students.push_back(s);
    saveStudents();
    cout << "✅ Registration Successful!\n";
    return true;
}

// ---------------------- LOGIN --------------------------
bool loginStudent() {
    string snInput;
    cout << "\n--- Student Login ---\n";

    while (true) {
        cout << "Enter Student Number: ";
        cin >> snInput;
        for (char &c : snInput) c = toupper(c);

        Student* found = nullptr;
        for (auto &s : students) {
            if (s.studentNumber == snInput) {
                found = &s;
                break;
            }
        }

        if (!found) {
            cout << "❌ Student number not found. Try again.\n";
            continue;
        }

        string pwInput;
        cout << "Enter Password: ";
        cin >> pwInput;

        if (pwInput == found->password) {
            cout << "✅ Welcome " << found->name << "! Login successful.\n";

            // --- CALL EXTERNAL MENU ---
            #ifdef _WIN32
                system("menu.exe");
            #else
                system("./menu");
            #endif

            return true;
        } else {
            cout << "❌ Incorrect password.\n";
        }
    }
}

// ---------------------- MAIN --------------------------
int main() {
    int choice;
    loadStudents();

    while (true) {
        cout << "\n=== Student System ===\n";
        cout << "1. Login\n";
        cout << "2. Register\n";
        cout << "3. Exit\n";
        cout << "Choose an option: ";
        cin >> choice;

        if (choice == 1) loginStudent();
        else if (choice == 2) registerStudent();
        else if (choice == 3) {
            cout << "Goodbye!\n";
            break;
        } else {
            cout << "Invalid choice!\n";
        }
    }

    return 0;
}