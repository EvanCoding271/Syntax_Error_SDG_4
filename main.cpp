#include <iostream>
#include <fstream>
#include <string>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
using namespace std;

struct Student {
    string studentNumber;
    string name;
    string password;
};

struct Subject {
    string name;
    string lesson;
    string questions[5];
    string answers[5];
};

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void waitForEnter() {
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

// Registration
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

// Login
bool loginStudent(string &studentName) {
    string snInput, pwInput;
    cout << "\n--- Student Login ---\n";
    cout << "Enter Student Number: ";
    cin >> snInput;
    cout << "Enter Password: ";
    cin >> pwInput;
    cin.ignore();

    ifstream infile("students.txt");
    string sn, nm, pw;
    bool loggedIn = false;

    while (infile >> sn >> nm >> pw) {
        if (sn == snInput && pw == pwInput) {
            cout << "✅ Welcome " << nm << "! Login successful.\n";
            studentName = nm;
            loggedIn = true;
            break;
        }
    }
    infile.close();

    if (!loggedIn) {
        cout << "❌ Incorrect student number or password.\n";
    }

    return loggedIn;
}

// Menu & lessons
void showMenu() {
    clearScreen();

    Subject subjects[7] = {
        {"Math",
         "Lesson: Addition and Subtraction\nAddition is combining numbers. Example: 2 + 3 = 5\nSubtraction is taking away numbers. Example: 5 - 2 = 3\nPractice these skills!",
         {"3 + 4 = ?", "5 + 2 = ?", "10 - 3 = ?", "7 - 5 = ?", "6 + 2 = ?"},
         {"7", "7", "7", "2", "8"}},
        {"Science",
         "Lesson: Plants\nPlants need sunlight, water, and soil to grow. They make food by photosynthesis.\nObserve and care for plants to learn better.",
         {"What do plants need to grow?", "What do plants make to get food?", "Do plants need sunlight?", "Do plants need water?", "Do plants need soil?"},
         {"Sunlight", "Food", "Yes", "Yes", "Yes"}},
        {"English",
         "Lesson: Simple Sentences\nA sentence tells a complete idea. Example: I like apples.\nStart with a capital letter and end with a period.\nTry writing about daily life.",
         {"She ___ happy.", "I ___ apples.", "They ___ running.", "He ___ tall.", "We ___ playing."},
         {"is", "like", "are", "is", "are"}},
        {"Filipino",
         "Lesson: Wika\nAng wika ay paraan ng pakikipag-usap. Pambansang wika ng Pilipinas ay Filipino.\nGamitin araw-araw para masanay.",
         {"Ano ang pambansang wika ng Pilipinas?", "Ano ang tawag sa paraan ng pakikipag-usap?", "Gaano kahalaga ang wika?", "Gamitin mo ba ang wika araw-araw?", "Wika ba ay mahalaga?"},
         {"Filipino", "Wika", "Mahalaga", "Oo", "Oo"}},
        {"Araling Panlipunan",
         "Lesson: Philippine Heroes\nJose Rizal is a national hero. He wrote books to help people understand rights.\nOther heroes contributed to freedom.",
         {"Who is Jose Rizal?", "What did he write?", "Is he a national hero?", "Did he fight for freedom?", "Should we remember him?"},
         {"National Hero", "Books", "Yes", "Yes", "Yes"}},
        {"E.S.P",
         "Lesson: Honesty\nHonesty means telling the truth. Builds trust and respect.",
         {"What does honesty mean?", "Should we be honest?", "Honesty builds ___?", "Who should we be honest with?", "Is honesty good?"},
         {"Telling the truth", "Yes", "Trust", "Everyone", "Yes"}},
        {"P.E",
         "Lesson: Exercise\nExercise helps body stay strong and healthy. Run, jump, or play games daily.",
         {"Name one activity that is exercise.", "Does exercise make you healthy?", "Can you run to exercise?", "Is jumping exercise?", "Should we exercise daily?"},
         {"Running", "Yes", "Yes", "Yes", "Yes"}}
    };

    int subjectChoice;
    string studentAnswer;

    cout << "\n=== STUDENT MENU ===\n";
    cout << "Choose a subject to study:\n";
    for (int i = 0; i < 7; i++) {
        cout << i + 1 << ". " << subjects[i].name << endl;
    }

    cout << "Enter your choice: ";
    cin >> subjectChoice;
    cin.ignore();

    if (subjectChoice < 1 || subjectChoice > 7) {
        cout << "Invalid choice! Exiting menu.\n";
        return;
    }

    int idx = subjectChoice - 1;
    clearScreen();

    cout << "\n--- " << subjects[idx].name << " ---\n";
    cout << subjects[idx].lesson << "\n\n";

    // 5 questions
    for (int i = 0; i < 5; i++) {
        cout << "Question " << i + 1 << ": " << subjects[idx].questions[i] << "\n";
        cout << "Answer: ";
        getline(cin, studentAnswer);
        if (studentAnswer == subjects[idx].answers[i]) {
            cout << "✅ Correct!\n\n";
        } else {
            cout << "❌ Incorrect. Recommendation: Read the lesson carefully.\n\n";
        }
    }

    cout << "You finished all questions for " << subjects[idx].name << "!\n";
    waitForEnter();
}

int main() {
    int choice;
    string studentName;

    while (true) {
        cout << "\n=== Secondary Education Learning System ===\n";
        cout << "1. Login\n2. Register\n3. Exit\n";
        cout << "Choose an option: ";
        cin >> choice;

        if (choice == 1) {
            if (loginStudent(studentName)) {
                showMenu(); // call menu immediately after login
            }
        } else if (choice == 2) {
            registerStudent();
        } else if (choice == 3) {
            cout << "Goodbye!\n";
            break;
        } else {
            cout << "Invalid choice!\n";
        }
    }

    return 0;
}
