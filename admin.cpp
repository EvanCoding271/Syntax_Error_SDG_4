#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iomanip>

using namespace std;

// ----------------------------------------------------------
// ---------------------- STRUCTURES -------------------------
// ----------------------------------------------------------

struct Student {
    string id;
    string name;
    string password;
};

struct SubjectProgress {
    string subject;
    int finished;
    int total;

    double percent() const {
        if (total == 0) return 0.0;
        return (finished * 100.0) / total;
    }
};

struct StudentRecord {
    Student s;
    vector<SubjectProgress> subjects;
};

// ----------------------------------------------------------
// ---------------------- UTILITIES --------------------------
// ----------------------------------------------------------

string trim(const string &str) {
    size_t a = str.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = str.find_last_not_of(" \t\r\n");
    return str.substr(a, b - a + 1);
}

// ----------------------------------------------------------
// ----------------- LOAD STUDENTS.TXT -----------------------
// ----------------------------------------------------------

vector<Student> loadStudents(const string &filename = "students.txt") {
    vector<Student> students;
    ifstream in(filename);
    if (!in.is_open()) {
        cerr << "[WARNING] Cannot open students.txt\n";
        return students;
    }

    string header;
    getline(in, header); // skip header
    
    string line;
    while (getline(in, line)) {
        if (trim(line).empty()) continue;
        istringstream ss(line);
        Student s;
        ss >> s.id >> s.name >> s.password;
        students.push_back(s);
    }
    return students;
}

// ----------------------------------------------------------
// ---------- LOAD student_progress.txt INTO MAP -------------
// ----------------------------------------------------------

struct ProgressRow {
    string id;
    int level[7];
    int s0[7], s1[7], s2[7];
};

unordered_map<string, ProgressRow> loadProgressFile() {
    unordered_map<string, ProgressRow> mp;
    ifstream in("student_progress.txt");
    if (!in.is_open()) return mp;

    while (!in.eof()) {
        ProgressRow row;
        if (!(in >> row.id)) break;

        for (int i = 0; i < 7; i++) {
            in >> row.level[i]
               >> row.s0[i]
               >> row.s1[i]
               >> row.s2[i];
        }

        mp[row.id] = row;
    }

    return mp;
}

SubjectProgress convertProgress(
    const string &subjectName,
    int level, int a, int b, int c)
{
    int finished = 0;
    if (level == 0) finished = a;
    else if (level == 1) finished = a + b;
    else finished = a + b + c;

    int total = (level + 1) * 5;

    return {subjectName, finished, total};
}

StudentRecord buildStudentRecord(const Student &s,
                                 const unordered_map<string, ProgressRow> &mp)
{
    StudentRecord rec;
    rec.s = s;

    static const string SUBJECTS[7] =
        {"Math","Science","English","Filipino","Araling Panlipunan","E.S.P","P.E"};

    if (mp.count(s.id) == 0)
        return rec;

    const ProgressRow &row = mp.at(s.id);

    for (int i = 0; i < 7; i++) {
        rec.subjects.push_back(
            convertProgress(SUBJECTS[i],
                            row.level[i], row.s0[i], row.s1[i], row.s2[i])
        );
    }

    return rec;
}

// ----------------------------------------------------------
// ----------------- PRINTING FUNCTIONS ----------------------
// ----------------------------------------------------------

void print2x2Matrix(const SubjectProgress &sp) {
    int remaining = sp.total - sp.finished;
    if (remaining < 0) remaining = 0;

    string rec;
    double pct = sp.percent();

    if (pct >= 90) rec = "Excellent — Enrichment recommended";
    else if (pct >= 70) rec = "Good — Continue practice";
    else if (pct >= 40) rec = "Needs improvement — Remediation";
    else rec = "At risk — Tutoring recommended";

    cout << "+----------------------+----------------------+\n";
    cout << "| Finished: " << setw(3) << sp.finished
         << "            | Remaining: " << setw(3) << remaining << "        |\n";
    cout << "+----------------------+----------------------+\n";
    cout << "| Recommendation: " << left << setw(26) << rec << "|\n";
    cout << "+---------------------------------------------+\n";
}

void printStudentReportCard(const StudentRecord &rec) {
    cout << "\n============================================\n";
    cout << "Student ID: " << rec.s.id << "\n";
    cout << "Name: " << rec.s.name << "\n";
    cout << "--------------------------------------------\n";

    if (rec.subjects.empty()) {
        cout << "No progress found.\n";
        return;
    }

    cout << setw(15) << left << "Subject"
         << setw(12) << "Percent"
         << setw(12) << "Status" << "\n";
    cout << "--------------------------------------------\n";

    for (const auto &sp : rec.subjects) {
        string status = (sp.total == 15 ? "MASTERED" : "In Prog");

        cout << setw(15) << left << sp.subject
             << setw(12) << fixed << setprecision(2) << sp.percent()
             << setw(12) << status << "\n";

        print2x2Matrix(sp);
    }

    cout << "============================================\n\n";
}

// ----------------------------------------------------------
// --------------------- SORTING -----------------------------
// ----------------------------------------------------------

double getPercentForSubject(const StudentRecord &rec, const string &subj) {
    string s = subj;
    transform(s.begin(), s.end(), s.begin(), ::tolower);

    for (const auto &sp : rec.subjects) {
        string x = sp.subject;
        transform(x.begin(), x.end(), x.begin(), ::tolower);
        if (x == s) return sp.percent();
    }
    return 0.0;
}

void mergeSort(vector<StudentRecord> &arr, const string &subject) {
    if (arr.size() <= 1) return;

    int mid = arr.size() / 2;
    vector<StudentRecord> L(arr.begin(), arr.begin() + mid);
    vector<StudentRecord> R(arr.begin() + mid, arr.end());

    mergeSort(L, subject);
    mergeSort(R, subject);

    arr.clear();
    int i = 0, j = 0;

    while (i < L.size() && j < R.size()) {
        if (getPercentForSubject(L[i], subject) >=
            getPercentForSubject(R[j], subject))
            arr.push_back(L[i++]);
        else
            arr.push_back(R[j++]);
    }

    while (i < L.size()) arr.push_back(L[i++]);
    while (j < R.size()) arr.push_back(R[j++]);
}

// ----------------------------------------------------------
// ------------------- BINARY SEARCH -------------------------
// ----------------------------------------------------------

int binarySearchByID(const vector<Student> &arr, const string &id) {
    int L = 0, R = arr.size() - 1;
    while (L <= R) {
        int mid = (L + R) / 2;
        if (arr[mid].id == id) return mid;
        if (arr[mid].id < id) L = mid + 1;
        else R = mid - 1;
    }
    return -1;
}

// ----------------------------------------------------------
// ---------------------- ADMIN MENU -------------------------
// ----------------------------------------------------------

void adminMenu(vector<Student> &students) {
    auto progress = loadProgressFile();

    while (true) {
        cout << "\n========== ADMIN MENU ==========\n";
        cout << "1. Sort students by subject progress\n";
        cout << "2. Display all students\n";
        cout << "3. Search student by ID (Binary Search)\n";
        cout << "0. Exit\n";
        cout << "Choice: ";

        int c;
        cin >> c;
        cin.ignore();

        if (c == 0) break;

        if (c == 1) {
            cout << "Enter subject to sort by (Math/Science/etc): ";
            string subj;
            getline(cin, subj);

            vector<StudentRecord> recs;
            for (auto &s : students)
                recs.push_back(buildStudentRecord(s, progress));

            mergeSort(recs, subj);

            cout << "\nSorted by " << subj << ":\n";
            cout << setw(10) << "ID" << setw(20) << "Name" << "Percent\n";
            cout << "----------------------------------------\n";
            
            for (auto &r : recs)
                cout << setw(10) << r.s.id << setw(20) << r.s.name
                     << fixed << setprecision(2)
                     << getPercentForSubject(r, subj) << "%\n";
        }

        else if (c == 2) {
            cout << "\nStudents:\n";
            for (auto &s : students)
                cout << s.id << " - " << s.name << "\n";

            cout << "\n1. Print ONE student\n2. Print ALL\nChoice: ";
            int sub;
            cin >> sub;
            cin.ignore();

            if (sub == 1) {
                string id;
                cout << "Enter ID: ";
                getline(cin, id);

                bool found = false;
                for (auto &s : students) {
                    if (s.id == id) {
                        printStudentReportCard(buildStudentRecord(s, progress));
                        found = true;
                        break;
                    }
                }
                if (!found) cout << "Student not found.\n";
            }
            else if (sub == 2) {
                for (auto &s : students)
                    printStudentReportCard(buildStudentRecord(s, progress));
            }
        }

        else if (c == 3) {
            vector<Student> sorted = students;
            sort(sorted.begin(), sorted.end(),
                 [](auto &a, auto &b){ return a.id < b.id; });

            string id;
            cout << "Enter ID: ";
            getline(cin, id);

            int idx = binarySearchByID(sorted, id);
            if (idx == -1)
                cout << "Not found.\n";
            else
                printStudentReportCard(
                    buildStudentRecord(sorted[idx], progress)
                );
        }
    }
}

// ----------------------------------------------------------
// --------------------------- MAIN --------------------------
// ----------------------------------------------------------

int main() {
    auto students = loadStudents();
    if (students.empty()) {
        cout << "No students found.\n";
        return 0;
    }

    adminMenu(students);
    return 0;
}
