#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

// ====================== STRUCTS ======================
struct Student {
    string studentNumber;
    string name;
    string password;
};

struct Admin {
    string adminID;
    string name;
    string password;
};

vector<Student> students;
vector<Admin> admins;

// ====================== CLEAR SCREEN ======================
void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// ====================== LOAD ADMINS ======================
void loadAdmins() {
    ifstream infile("admins.txt");
    if (!infile) {
        cout << "[ERROR] Cannot open admins.txt\n";
        return;
    }

    string header;
    getline(infile, header); // skip header

    while (infile) {
        string id;
        if (!(infile >> id)) break;

        string rest;
        getline(infile, rest);
        while (!rest.empty() && rest[0] == ' ') rest.erase(rest.begin());

        size_t pos = rest.rfind(' ');
        string name = rest.substr(0, pos);
        string pw = rest.substr(pos + 1);

        admins.push_back({id, name, pw});
    }
}

// ====================== ADMIN LOGIN ======================
bool loginAdmin(const string& id, const string& pw) {
    for (auto& a : admins) {
        if (a.adminID == id && a.password == pw) {
            cout << "\n=============================\n";
            cout << "  ADMIN LOGIN SUCCESSFUL\n";
            cout << "  Welcome, " << a.name << "!\n";
            cout << "=============================\n";

#ifdef _WIN32
            system("admin.exe");
#else
            system("./admin");
#endif
            return true;
        }
    }
    return false;
}

bool adminLoginMenu() {
    clearConsole();
    string id, pw;

    cout << "\n--- Admin Login ---\n";
    cout << "Enter Admin ID (or C to cancel): ";
    cin >> id;

    if (id == "C" || id == "c") return false;

    cout << "Enter Password: ";
    cin >> pw;

    return loginAdmin(id, pw);
}

// ====================== LOAD STUDENTS ======================
void loadStudents() {
    ifstream infile("students.txt");
    if (!infile) return;

    students.clear();

    string header;
    getline(infile, header);

    string sn, nm_pw;
    while (infile >> sn) {
        infile >> ws;
        getline(infile, nm_pw);

        size_t pos = nm_pw.rfind(' ');
        string pw = nm_pw.substr(pos + 1);
        string nm = nm_pw.substr(0, pos);

        students.push_back({sn, nm, pw});
    }
}

// ====================== SAVE STUDENTS ======================
void saveStudents() {
    ofstream outfile("students.txt");
    outfile << "StudentID Name Password\n";
    for (auto& s : students) {
        outfile << s.studentNumber << " " << s.name << " " << s.password << endl;
    }
}

// ====================== REGISTER STUDENT ======================
bool registerStudent() {
    clearConsole();
    Student s;

    cout << "\n--- Student Registration ---\n";
    cout << "Enter Student Number: ";
    cin >> s.studentNumber;
    for (char &c : s.studentNumber) c = toupper(c);

    cin.ignore();
    cout << "Enter Full Name: ";
    getline(cin, s.name);

    cout << "Enter Password: ";
    getline(cin, s.password);

    if (s.password.length() < 4 || s.password.find(' ') != string::npos) {
        cout << "X Invalid password.\n";
        return false;
    }

    for (auto& st : students) {
        if (st.studentNumber == s.studentNumber) {
            cout << "X Student already exists!\n";
            return false;
        }
    }

    students.push_back(s);
    saveStudents();

    cout << " Registration successful!\n";
    return true;
}

// ====================== STUDENT LOGIN ======================
bool loginStudent() {
    clearConsole();
    string idInput, pwInput;

    cout << "\n--- Student Login ---\n";
    cout << "Enter Student Number (or C to cancel): ";
    cin >> idInput;

    if (idInput == "C" || idInput == "c") return false;

    for (char& c : idInput) c = toupper(c);

    cout << "Enter Password: ";
    cin >> pwInput;

    for (auto& s : students) {
        if (s.studentNumber == idInput) {
            if (s.password == pwInput) {
                cout << " Welcome " << s.name << "! Login successful.\n";

                //  Save student ID to temp file for menu.cpp
                ofstream tempFile("current_student_id.tmp");
                if(tempFile.is_open()){
                    tempFile << idInput;
                    tempFile.close();
                } else {
                    cout << "ERROR: Cannot create temp file for menu.\n";
                    return false;
                }

                // Launch menu program
#ifdef _WIN32
                system("menu.exe");
#else
                system("./menu");
#endif
                return true;
            } else {
                cout << "X Wrong password.\n";
                return false;
            }
        }
    }

    cout << "X Student not found.\n";
    return false;
}

// =========================== MAIN ============================
int main() {
    loadAdmins();
    loadStudents();

    while (true) {
        clearConsole();
        int choice;

        cout << "\n=== Student System ===\n";
        cout << "1. Admin Login\n";
        cout << "2. Student Login\n";
        cout << "3. Register Student\n";
        cout << "4. Exit\n";
        cout << "Choose an option: ";
        cin >> choice;

        switch (choice) {
            case 1: adminLoginMenu(); break;
            case 2: loginStudent(); break;
            case 3: registerStudent(); break;
            case 4: cout << "Goodbye!\n"; return 0;
            default: cout << "Invalid choice!\n";
        }

        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }
}