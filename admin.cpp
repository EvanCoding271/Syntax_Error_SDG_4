#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iomanip>

using namespace std;

// ==================== DATA STRUCTURES ====================

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
        if (total == 0) return 0;
        return (finished * 100.0) / total;
    }
};

struct StudentRecord {
    Student s;
    vector<SubjectProgress> subjects;
};

// ==================== FILE LOADERS ====================

// Load students.txt
vector<Student> loadStudents(const string& file = "students.txt") {
    vector<Student> list;
    ifstream in(file);

    if (!in) {
        cout << "[ERROR] Cannot open students.txt\n";
        return list;
    }

    string header;
    getline(in, header); // skip header line

    while (in) {
        string id;
        if (!(in >> id)) break;

        string rest;
        getline(in, rest);

        while (!rest.empty() && rest[0] == ' ')
            rest.erase(rest.begin());

        size_t pos = rest.rfind(' ');
        string name = rest.substr(0, pos);
        string password = rest.substr(pos + 1);

        list.push_back({id, name, password});
    }
    return list;
}

// Load reportcards.txt
unordered_map<string, vector<SubjectProgress>> loadReportCards(const string& file = "reportcards.txt") {
    unordered_map<string, vector<SubjectProgress>> rc;
    ifstream in(file);

    if (!in) return rc;

    string line, currentID;

    while (getline(in, line)) {
        if (line == "---") {
            currentID = "";
            continue;
        }

        if (currentID.empty()) {
            currentID = line;
            rc[currentID] = {};
        }
        else {
            string subject;
            int f, t;
            stringstream ss(line);
            ss >> subject >> f >> t;
            rc[currentID].push_back({subject, f, t});
        }
    }
    return rc;
}

// ==================== SORT (MERGE SORT) ====================

double getPercent(const StudentRecord& rec, const string& subj) {
    string s = subj;
    transform(s.begin(), s.end(), s.begin(), ::tolower);

    for (auto& x : rec.subjects) {
        string sub = x.subject;
        transform(sub.begin(), sub.end(), sub.begin(), ::tolower);
        if (sub == s)
            return x.percent();
    }
    return 0.0;
}

void mergeSort(vector<StudentRecord>& arr, const string& subject) {
    if (arr.size() <= 1) return;

    int mid = arr.size() / 2;
    vector<StudentRecord> L(arr.begin(), arr.begin() + mid);
    vector<StudentRecord> R(arr.begin() + mid, arr.end());

    mergeSort(L, subject);
    mergeSort(R, subject);

    arr.clear();
    int i = 0, j = 0;

    while (i < L.size() && j < R.size()) {
        if (getPercent(L[i], subject) >= getPercent(R[j], subject))
            arr.push_back(L[i++]);
        else
            arr.push_back(R[j++]);
    }

    while (i < L.size()) arr.push_back(L[i++]);
    while (j < R.size()) arr.push_back(R[j++]);
}

// ==================== BINARY SEARCH ====================

int binarySearch(const vector<Student>& sorted, const string& id) {
    int L = 0, R = sorted.size() - 1;

    while (L <= R) {
        int mid = (L + R) / 2;
        if (sorted[mid].id == id) return mid;
        if (sorted[mid].id < id) L = mid + 1;
        else R = mid - 1;
    }
    return -1;
}

// ==================== PRINTING HELPERS ====================

void printMatrix(const SubjectProgress& s) {
    int remaining = s.total - s.finished;
    string recommendation;

    if (s.percent() >= 90) recommendation = "Excellent progress";
    else if (s.percent() >= 70) recommendation = "Keep studying";
    else if (s.percent() >= 40) recommendation = "Needs improvement";
    else recommendation = "At risk — needs tutoring";

    cout << "+-----------------------+-----------------------+\n";
    cout << "| Lessons Finished: " << setw(4) << s.finished
         << " | Lessons Remaining: " << setw(4) << remaining << " |\n";
    cout << "+-----------------------+-----------------------+\n";
    cout << "| Recommendation: " << recommendation << "\n";
    cout << "+-----------------------------------------------+\n\n";
}

void printReport(const StudentRecord& rec) {
    cout << "\n========== REPORT CARD ==========\n";
    cout << "ID: " << rec.s.id << "\n";
    cout << "Name: " << rec.s.name << "\n\n";

    if (rec.subjects.empty()) {
        cout << "No report card available.\n";
        return;
    }

    for (auto& x : rec.subjects) {
        cout << x.subject << ": " << x.finished << "/" << x.total
             << " (" << fixed << setprecision(2) << x.percent() << "%)\n";
        printMatrix(x);
    }
}

// ==================== ADMIN MENU ====================

void adminMenu(
    vector<Student>& students,
    unordered_map<string, vector<SubjectProgress>>& rc
) {
    vector<StudentRecord> records;

    for (auto& s : students) {
        StudentRecord rec;
        rec.s = s;
        if (rc.count(s.id)) rec.subjects = rc[s.id];
        records.push_back(rec);
    }

    while (true) {
        cout << "\n============ ADMIN MENU ============\n";
        cout << "1. Sort students by subject (Divide & Conquer)\n";
        cout << "2. Display all students + Print report card\n";
        cout << "3. Search student by ID (Binary Search)\n";
        cout << "0. Exit\n";
        cout << "Choice: ";

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 0) break;

        if (choice == 1) {
            string subj;
            cout << "Enter subject name: ";
            getline(cin, subj);

            vector<StudentRecord> sorted = records;
            mergeSort(sorted, subj);

            cout << "\nSorted by " << subj << ":\n";
            for (auto& r : sorted) {
                cout << r.s.id << " - " << r.s.name
                     << " | " << getPercent(r, subj) << "% complete\n";
            }
        }

        else if (choice == 2) {
            for (auto& r : records) {
                cout << r.s.id << " - " << r.s.name << "\n";
            }

            cout << "\nPrint a report card? (y/n): ";
            char q; cin >> q; cin.ignore();

            if (q == 'y' || q == 'Y') {
                string id;
                cout << "Enter student ID: ";
                getline(cin, id);

                bool found = false;
                for (auto& r : records) {
                    if (r.s.id == id) {
                        printReport(r);
                        found = true;
                    }
                }
                if (!found)
                    cout << "Student not found.\n";
            }
        }

        else if (choice == 3) {
            vector<Student> sorted = students;
            sort(sorted.begin(), sorted.end(),
                 [](const Student& a, const Student& b) {
                     return a.id < b.id;
                 });

            string id;
            cout << "Enter ID to search: ";
            getline(cin, id);

            int idx = binarySearch(sorted, id);
            if (idx == -1) {
                cout << "Student not found.\n";
            } else {
                Student target = sorted[idx];
                StudentRecord rec;
                rec.s = target;

                if (rc.count(target.id))
                    rec.subjects = rc[target.id];

                printReport(rec);
            }
        }
    }
}

// ==================== MAIN ====================

int main() {
    auto students = loadStudents();
    auto reportcards = loadReportCards();

    adminMenu(students, reportcards);
    return 0;
}
