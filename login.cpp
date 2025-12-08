#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

// ====================== STUDENT STRUCT ======================
struct Student {
    string studentNumber;
    string name;
    string password;
};

vector<Student> students;

// ====================== ADMIN STRUCT ========================
struct Admin {
    string adminID;
    string name;
    string password;
};

vector<Admin> admins;

// ====================== LOAD ADMINS =========================
void loadAdmins() {
    ifstream infile("admins.txt");
    if (!infile) {
        cout << "[ERROR] Cannot open admins.txt\n";
        return;
    }

    string header;
    getline(infile, header); // skip header: AdminID Name Password

    while (infile) {
        string id;
        if (!(infile >> id)) break;

        string rest;
        getline(infile, rest);

        // trim leading space
        while (!rest.empty() && rest[0] == ' ')
            rest.erase(rest.begin());

        size_t pos = rest.rfind(' ');
        string name = rest.substr(0, pos);
        string pw = rest.substr(pos + 1);

        admins.push_back({id, name, pw});
    }
}

// ====================== CHECK ADMIN LOGIN ====================
bool loginAdmin(const string& id, const string& pw) {
    for (auto& a : admins) {
        if (a.adminID == id && a.password == pw) {
            cout << "\n=============================\n";
            cout << "  ADMIN LOGIN SUCCESSFUL\n";
            cout << "  Welcome, " << a.name << "!\n";
            cout << "=============================\n";

            // Open admin.exe
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

// ====================== LOAD STUDENTS ========================
void loadStudents() {
    ifstream infile("students.txt");
    string sn, nm, pw;
    students.clear();

    string header;
    getline(infile, header);

    while (infile >> sn) {
        infile >> ws;
        getline(infile, nm, '\n');

        size_t pos = nm.rfind(' ');
        if (pos != string::npos) {
            pw = nm.substr(pos + 1);
            nm = nm.substr(0, pos);
        }

        students.push_back({sn, nm, pw});
    }
    infile.close();
}

// ====================== SAVE STUDENTS ========================
void saveStudents() {
    ofstream outfile("students.txt");
    outfile << "StudentID Name Password\n";
    for (auto &s : students) {
        outfile << s.studentNumber << " " << s.name << " " << s.password << endl;
    }
    outfile.close();
}

// ====================== REGISTER STUDENT =====================
bool registerStudent() {
    Student s;
    cout << "\n--- Student Registration ---\n";
    cout << "Enter Student Number: ";
    cin >> s.studentNumber;

    for (char &c : s.studentNumber) c = toupper(c);

    cin.ignore();
    cout << "Enter Full Name: ";
    getline(cin, s.name);

    cout << "Enter Password: ";
    while (true) {
        getline(cin, s.password);

        if (s.password.length() < 4) {
            cout << "❌ Password must be at least 4 characters. Try again: ";
        } else if (s.password.find(" ") != string::npos) {
            cout << "❌ Password cannot contain spaces. Try again: ";
        } else break;
    }

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

// ========================= LOGIN ==============================
bool loginStudent() {
    string idInput, pwInput;

    cout << "\n--- Login ---\n";
    cout << "Enter ID: ";
    cin >> idInput;

    // Uppercase ID
    for (char &c : idInput) c = toupper(c);

    cout << "Enter Password: ";
    cin >> pwInput;

    // ========== FIRST CHECK IF ADMIN ==========
    if (loginAdmin(idInput, pwInput)) {
        return true;
    }

    // ========== ELSE CHECK STUDENT ============
    Student* found = nullptr;
    for (auto &s : students) {
        if (s.studentNumber == idInput) {
            found = &s;
            break;
        }
    }

    if (!found) {
        cout << "❌ ID not found.\n";
        return false;
    }

    if (pwInput != found->password) {
        cout << "❌ Incorrect password.\n";
        return false;
    }

    cout << "✅ Welcome " << found->name << "! Login successful.\n";

    // open menu.exe
    #ifdef _WIN32
        system("menu.exe");
    #else
        system("./menu");
    #endif

    return true;
}

// =========================== MAIN =============================
int main() {
    int choice;

    loadAdmins();   // load admin accounts
    loadStudents(); // load students

    while (true) {
        cout << "\n=== System Login ===\n";
        cout << "1. Login\n";
        cout << "2. Register (Student Only)\n";
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
