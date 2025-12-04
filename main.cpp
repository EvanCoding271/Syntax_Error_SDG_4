#include <iostream>
#include <string>
using namespace std;

// Struct for a subject
struct Subject {
    string name;
    string lesson;
    string questions[5];
    string answers[5];
};

// Function to show a subject lesson and quiz
void takeSubject(Subject &sub) {
    string studentAnswer;
    cout << "\n--- " << sub.name << " ---\n";
    cout << sub.lesson << "\n\n";

    for (int i = 0; i < 5; i++) {
        cout << "Question " << i + 1 << ": " << sub.questions[i] << "\n";
        cout << "Answer: ";
        getline(cin, studentAnswer);

        if (studentAnswer == sub.answers[i])
            cout << "✅ Correct!\n\n";
        else
            cout << "❌ Incorrect. Recommendation: Read the lesson carefully.\n\n";
    }

    cout << "You finished all questions for " << sub.name << "!\n";
}

int main() {
    // Step 1: Prepare subjects using array of structs
    Subject subjects[7] = {
        {"Math",
         "Addition and Subtraction\nAdd numbers together. Example: 2+3=5\nSubtract numbers. Example: 5-2=3.",
         {"3 + 4 = ?", "5 + 2 = ?", "10 - 3 = ?", "7 - 5 = ?", "6 + 2 = ?"},
         {"7", "7", "7", "2", "8"}},

        {"Science",
         "Plants\nPlants need sunlight, water, and soil. They make food through photosynthesis.",
         {"What do plants need to grow?", "What do plants make to get food?", "Do plants need sunlight?", "Do plants need water?", "Do plants need soil?"},
         {"Sunlight", "Food", "Yes", "Yes", "Yes"}},

        {"English",
         "Simple Sentences\nA sentence tells a complete idea. Start with capital, end with period.",
         {"She ___ happy.", "I ___ apples.", "They ___ running.", "He ___ tall.", "We ___ playing."},
         {"is", "like", "are", "is", "are"}},

        {"Filipino",
         "Wika\nAng wika ay paraan ng pakikipag-usap. Pambansang wika ay Filipino.",
         {"Ano ang pambansang wika ng Pilipinas?", "Ano ang tawag sa paraan ng pakikipag-usap?", "Gaano kahalaga ang wika?", "Gamitin mo ba ang wika araw-araw?", "Wika ba ay mahalaga?"},
         {"Filipino", "Wika", "Mahalaga", "Oo", "Oo"}},

        {"Araling Panlipunan",
         "Philippine Heroes\nJose Rizal is a national hero. He wrote books to teach people about rights.",
         {"Who is Jose Rizal?", "What did he write?", "Is he a national hero?", "Did he fight for freedom?", "Should we remember him?"},
         {"National Hero", "Books", "Yes", "Yes", "Yes"}},

        {"E.S.P",
         "Honesty\nHonesty means telling the truth. It builds trust and respect.",
         {"What does honesty mean?", "Should we be honest?", "Honesty builds ___?", "Who should we be honest with?", "Is honesty good?"},
         {"Telling the truth", "Yes", "Trust", "Everyone", "Yes"}},

        {"P.E",
         "Exercise\nExercise keeps you healthy. You can run, jump, or play games.",
         {"Name one activity that is exercise.", "Does exercise make you healthy?", "Can you run to exercise?", "Is jumping exercise?", "Should we exercise daily?"},
         {"Running", "Yes", "Yes", "Yes", "Yes"}}
    };

    int choice;

    do {
        // Step 2: Show menu using data structure
        cout << "\n=== STUDENT MENU ===\n";
        cout << "Choose a subject to study:\n";
        for (int i = 0; i < 7; i++)
            cout << i + 1 << ". " << subjects[i].name << endl;
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // clear buffer

        if (choice >= 1 && choice <= 7) {
            takeSubject(subjects[choice - 1]);
        } else if (choice == 0) {
            cout << "Exiting menu...\n";
        } else {
            cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 0);

    return 0;
}
