#include <iostream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

struct LessonLevel {
    string lesson;
    string questions[5];
    string options[5][4]; // a/b/c/d options
    char answers[5];       // correct options a/b/c/d
};

struct Subject {
    string name;
    LessonLevel levels[3];
    int currentLevel = 0;
    int scores[3] = {0,0,0};
    double averageScore = 0.0;
};

string toLower(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// ====================== PROGRESS MANAGEMENT ======================
bool loadAllProgress(Subject subjects[], const string &studentID) {
    ifstream inFile("student_progress.txt");
    if(!inFile.is_open()) return false;

    string line;
    while(getline(inFile, line)) {
        istringstream ss(line);
        string id;
        ss >> id;

        if(id == studentID) {
            for(int i=0;i<7;i++){
                int level, s0,s1,s2;
                if(ss >> level >> s0 >> s1 >> s2){
                    subjects[i].currentLevel = level;
                    subjects[i].scores[0] = s0;
                    subjects[i].scores[1] = s1;
                    subjects[i].scores[2] = s2;
                }
            }
            inFile.close();
            return true;
        }
    }

    inFile.close();
    return false; // student not found
}

void saveAllProgress(Subject subjects[], const string &studentID){
    vector<string> allLines;
    ifstream inFile("student_progress.txt");
    string line;
    bool found = false;

    while(getline(inFile, line)) {
        istringstream ss(line);
        string id;
        ss >> id;
        if(id == studentID) {
            string newLine = studentID;
            for(int i=0;i<7;i++){
                newLine += " " + to_string(subjects[i].currentLevel) +
                           " " + to_string(subjects[i].scores[0]) +
                           " " + to_string(subjects[i].scores[1]) +
                           " " + to_string(subjects[i].scores[2]);
            }
            allLines.push_back(newLine);
            found = true;
        } else {
            allLines.push_back(line);
        }
    }
    inFile.close();

    if(!found){
        string newLine = studentID;
        for(int i=0;i<7;i++){
            newLine += " 0 0 0 0";
        }
        allLines.push_back(newLine);
    }

    ofstream outFile("student_progress.txt");
    for(const string &l : allLines){
        outFile << l << endl;
    }
    outFile.close();
}

// ====================== QUIZ & LESSON ======================
void takeSubject(Subject &sub) {
    string studentAnswer;
    LessonLevel currentData = sub.levels[sub.currentLevel];
    int correctCount = 0;

    clearScreen();
    cout << "\n=== " << sub.name << " - Level " << sub.currentLevel + 1 << " ===\n\n";
    cout << currentData.lesson << "\n\n";
    cout << "Press Enter to start the quiz...";
    cin.ignore();
    cin.get();

    for(int i=0;i<5;i++){
        clearScreen();
        cout << "\nQuestion " << i+1 << ": " << currentData.questions[i] << "\n";
        cout << "a) " << currentData.options[i][0] << "  ";
        cout << "b) " << currentData.options[i][1] << "\n";
        cout << "c) " << currentData.options[i][2] << "  ";
        cout << "d) " << currentData.options[i][3] << "\n";
        cout << "Your answer (a/b/c/d): ";
        cin >> studentAnswer;
        studentAnswer = toLower(studentAnswer);

        if(studentAnswer[0] == currentData.answers[i]){
            cout << "\n[RESULT] Correct!\n";
            correctCount++;
        } else {
            cout << "\n[RESULT] Incorrect. Correct answer: " << currentData.answers[i] << "\n";
        }
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }

    sub.scores[sub.currentLevel] = correctCount;

    clearScreen();
    if(sub.currentLevel < 2){
        sub.currentLevel++;
        cout << "\n--- Level Complete ---\n";
        cout << "You scored " << correctCount << "/5 correct.\n";
        cout << "Difficulty for " << sub.name << " has increased!\n";
        cout << "Press Enter to return to menu and try Level " << sub.currentLevel+1 << "...\n";
        cin.ignore();
        cin.get();
    } else {
        double totalScore = 0;
        for(int score : sub.scores) totalScore += score;
        sub.averageScore = (totalScore/15.0)*100.0;
        cout << "\n*** Subject MASTERED ***\n";
        cout << "You finished all levels for " << sub.name << ".\n";
        cout << "Total score: " << fixed << setprecision(2) << sub.averageScore << "%\n";
        cout << "Press Enter to return to menu...";
        cin.ignore();
        cin.get();
    }
}

// ====================== REPORT CARD ======================
void viewReportCard(Subject subjects[]) {
    clearScreen();
    cout << "\n=========================================\n";
    cout << "          STUDENT REPORT CARD\n";
    cout << "=========================================\n\n";

    double overallTotalScore = 0.0;
    int subjectsMasteredCount = 0;

    cout << setw(20) << left << "SUBJECT" << setw(15) << "SCORE (%)" << setw(15) << "LEVELS" << endl;
    cout << string(50,'-') << endl;

    for(int i=0;i<7;i++){
        if(subjects[i].currentLevel==2){
            double totalScore=0;
            for(int score:subjects[i].scores) totalScore+=score;
            subjects[i].averageScore=(totalScore/15.0)*100.0;

            cout << setw(20) << left << subjects[i].name 
                 << setw(15) << fixed << setprecision(2) << subjects[i].averageScore
                 << setw(15) << "MASTERED" << endl;

            overallTotalScore += subjects[i].averageScore;
            subjectsMasteredCount++;
        } else {
            cout << setw(20) << left << subjects[i].name
                 << setw(15) << "N/A"
                 << setw(15) << "In Progress (Lvl " << subjects[i].currentLevel+1 << ")" << endl;
        }
    }

    cout << string(50,'-') << endl;
    if(subjectsMasteredCount>0){
        double overallAverage = overallTotalScore/subjectsMasteredCount;
        cout << setw(20) << left << "OVERALL AVERAGE:" 
             << setw(15) << fixed << setprecision(2) << overallAverage << endl;
    } else {
        cout << "OVERALL AVERAGE: N/A (Complete at least one subject to calculate.)\n";
    }

    cout << "\nPress Enter to return to menu...";
    cin.ignore();
    cin.get();
}

// ====================== MAIN ======================
int main() {
    // ====================== SUBJECTS + LESSONS + QUIZZES ======================
    Subject subjects[7] = {
        {"Math", {
            {"Addition & Subtraction", {"3+4=?","5+2=?","10-3=?","7-5=?","3*4=?"}, 
             {{"6","7","5","8"},{"7","6","8","5"},{"7","6","5","3"},{"2","3","4","1"},{"12","11","10","9"}}, {'b','a','c','a','a'}},
            {"Multiplication & Division", {"8*7=?","9*6=?","25/5=?","18/3=?","15+15=?"}, 
             {{"54","56","55","58"},{"52","54","53","55"},{"5","6","4","3"},{"6","5","7","3"},{"30","25","35","20"}}, {'b','b','a','a','a'}},
            {"Order of Operations", {"2^3=?","10-2*3=?","6+4/2=?","(1+3)*2=?","5*5=?"}, 
             {{"6","8","9","7"},{"4","6","8","2"},{"8","7","6","9"},{"8","7","9","6"},{"25","20","15","30"}}, {'b','a','a','a','a'}}
        },0},

        {"Science", {
            {"Photosynthesis", {"Plants need to grow?","Process plants make food?","Gas absorbed by plants?","Gas released by plants?","Do plants need soil?"}, 
             {{"sunlight","water","soil","air"},{"photosynthesis","respiration","digestion","pollination"},{"oxygen","carbon dioxide","nitrogen","hydrogen"},{"oxygen","carbon dioxide","nitrogen","hydrogen"},{"yes","no","sometimes","maybe"}}, {'a','a','b','a','a'}},
            {"Cells", {"Basic unit of life?","Study of living things?","One biological kingdom?","Do animals have cells?","All living things have cells?"}, 
             {{"cell","tissue","organ","organism"},{"biology","chemistry","physics","math"},{"animalia","plantae","fungi","protista"},{"yes","no","sometimes","maybe"},{"true","false","sometimes","maybe"}}, {'a','a','a','a','a'}},
            {"Ecosystems", {"Producer in food web?","Ecosystem includes?","Consumer eats only plants?","Light living/non-living?","Sun powers most food webs?"}, 
             {{"plants","animals","fungi","humans"},{"living/non-living","plants only","animals only","none"},{"herbivore","carnivore","omnivore","decomposer"},{"living","non-living","both","none"},{"true","false","maybe","sometimes"}}, {'a','a','a','b','a'}}
        },0},

        {"English", {
            {"Simple Sentences", {"She ___ happy.","I ___ apples.","They ___ running.","He ___ tall.","We ___ playing."}, 
             {{"is","are","am","be"},{"like","likes","liking","liked"},{"are","is","am","be"},{"is","are","am","be"},{"are","is","am","be"}}, {'a','a','a','a','a'}},
            {"Compound Sentences", {"I like dogs ___ she likes cats.","We can go now ___ wait later.","He ran fast ___ he missed bus.","I studied, ___ I passed test.","Sun is hot ___ moon is cool."}, 
             {{"and","but","so","or"},{"or","and","but","so"},{"but","and","so","or"},{"so","and","but","or"},{"and","but","so","or"}}, {'b','a','a','c','a'}},
            {"Complex Sentences", {"___ it rained, we stayed inside.","I will call you ___ I get home.","She was happy ___ she won.","We left early ___ to avoid traffic.","I bought new book ___ finished old."}, 
             {{"Because","When","Although","Since"},{"when","because","although","since"},{"because","although","when","since"},{"because","since","although","when"},{"after","before","since","when"}}, {'a','a','a','b','a'}}
        },0},

        {"Filipino", {
            {"Wika at Kultura", {"Pambansang wika ng Pilipinas?","Paraan ng pakikipag-usap?","Gaano kahalaga wika?","Gamitin araw-araw?","Wika ba mahalaga?"}, 
             {{"Filipino","Tagalog","English","Spanish"},{"wika","kultura","panitikan","bansa"},{"mahalaga","hindi mahalaga","optional","di alam"},{"oo","hindi","paminsan","bihira"},{"oo","hindi","paminsan","bihira"}}, {'a','a','a','a','a'}},
            {"Bahagi ng Pananalita", {"Bahay ay ___","Tumakbo ay ___","Maganda ay ___","Bahagi ng pananalita Pangngalan?","Pang-uri ba 'mabagal'?"}, 
             {{"Pangngalan","Pandiwa","Pang-uri","Pang-abay"},{"Pangngalan","Pandiwa","Pang-uri","Pang-abay"},{"Pangngalan","Pandiwa","Pang-uri","Pang-abay"},{"Noun","Verb","Adjective","Adverb"},{"oo","hindi","paminsan","bihira"}}, {'a','b','c','a','a'}},
            {"Wastong Gamit", {"Ako (may/mayroon) libro","Naglaro (ng/nang) malakas","Kailangan (ng/nang) payong","(May/Mayroon) bisita si lola","Ako kumain (ng/nang) saging"}, 
             {{"may","mayroon","nang","ng"},{"ng","nang","may","mayroon"},{"ng","nang","may","mayroon"},{"May","Mayroon","ng","nang"},{"ng","nang","may","mayroon"}}, {'a','b','b','b','a'}}
        },0},

        {"Araling Panlipunan", {
            {"Bayani", {"Who is Jose Rizal?","What did he write?","Is he hero?","Fight for freedom?","Remember him?"}, 
             {{"National hero","Writer","Student","Teacher"},{"Books","Letters","Essays","None"},{"Yes","No","Maybe","False"},{"Yes","No","Maybe","False"},{"Yes","No","Maybe","False"}}, {'a','a','a','a','a'}},
            {"Pamahalaan", {"Ilang sangay?","Sangay Presidente?","Sangay gumawa batas?","Korte Suprema?","Pinuno Ehekutibo?"}, 
             {{"2","3","4","5"},{"Ehekutibo","Lehislatibo","Hudikatura","Pulis"},{"Lehislatibo","Hudikatura","Ehekutibo","Batas"},{"Hudikatura","Lehislatibo","Ehekutibo","Pulis"},{"Presidente","Senador","Mayor","Gobyerno"}}, {'b','a','a','a','a'}},
            {"Heograpiya", {"Pilipinas arkipelago?","Dibisyon bansa?","Capital Pilipinas?","Ilan pangunahing isla?","True/False: Asia?"}, 
             {{"Yes","No","Maybe","False"},{"Yes","No","Maybe","False"},{"Manila","Cebu","Davao","Quezon"},{"3","4","5","7"},{"True","False","Maybe","No"}}, {'a','a','a','a','a'}}
        },0},

        {"E.S.P", {
            {"Honesty", {"Honesty meaning?","Should be honest?","Honesty builds?","Who be honest with?","Is honesty good?"}, 
             {{"Truth","Lie","Confuse","Nothing"},{"Yes","No","Maybe","Sometimes"},{"Trust","Money","Fame","Happiness"},{"Everyone","Self","Family","None"},{"Yes","No","Maybe","Sometimes"}}, {'a','a','a','a','a'}},
            {"Respeto", {"Paggalang sa kapwa?","Gawain takdang aralin responsibilidad?","Dapat igalang nakatatanda?","Sino respetuhin?","True/False responsable?"}, 
             {{"Respeto","Huwag","Ignore","Mahalaga"},{"Yes","No","Sometimes","Maybe"},{"Yes","No","Sometimes","Maybe"},{"Lahat","Pamilya","Kaibigan","Teacher"},{"True","False","Maybe","Sometimes"}}, {'a','a','a','a','a'}},
            {"Patriotism", {"Pagmamahal sa bayan?","Proud sa bansa?","Community part of ESP?","Ano gawin basura?","Tapon ilog pagmamahal?"}, 
             {{"Patriotism","Love","Duty","Respect"},{"Yes","No","Maybe","Sometimes"},{"Yes","No","Maybe","Sometimes"},{"Itapon sa tama","Ilagay sa kalsada","Walang ginawa","Iba"},{"False","True","Maybe","Sometimes"}}, {'a','a','a','a','b'}}
        },0},

        {"P.E", {
            {"Exercise", {"Activity is exercise?","Exercise healthy?","Can run to exercise?","Is jumping exercise?","Exercise daily?"}, 
             {{"Running","Sleeping","Eating","Watching"},{"Yes","No","Maybe","Sometimes"},{"Yes","No","Maybe","Sometimes"},{"Yes","No","Maybe","Sometimes"},{"Yes","No","Maybe","Sometimes"}}, {'a','a','a','a','a'}},
            {"Safety & First Aid", {"Stretch before exercise?","First Aid meaning?","Body warm up name?","Exercise bad for heart?","Clean wound?"}, 
             {{"Yes","No","Sometimes","Maybe"},{"First Aid","Rest","Medicine","Help"},{"Stretching","Running","Eating","Jumping"},{"True","False","Maybe","Sometimes"},{"Yes","No","Maybe","Sometimes"}}, {'a','a','a','b','a'}},
            {"Nutrition & Diet", {"Proper eating called?","Vegetables nutritious?","Protein from meat/eggs?","Reduce sweets?","Need protein for bones?"}, 
             {{"Balanced diet","Unhealthy","Too much","Random"},{"Yes","No","Maybe","Sometimes"},{"Protein","Fat","Carbs","Vitamins"},{"Yes","No","Maybe","Sometimes"},{"Yes","No","Maybe","Sometimes"}}, {'a','a','a','a','b'}}
        },0}
    };

    string studentID;

    ifstream tempFile("current_student_id.tmp");
    if(tempFile.is_open()){
        getline(tempFile, studentID);
        tempFile.close();
        remove("current_student_id.tmp");
    } else {
        clearScreen();
        cout << "FATAL ERROR: Student ID not found. Use login.exe first.\n";
        cout << "Press Enter to exit...";
        cin.ignore();
        return 1;
    }

    clearScreen();
    cout << "Loading progress for Student ID: " << studentID << "\n";
    loadAllProgress(subjects, studentID);

    int choice;
    do {
        clearScreen();
        cout << "\n=== STUDENT MENU (ID: " << studentID << ") ===\n";
        cout << "Choose a subject to study:\n";
        for(int i=0;i<7;i++){
            if(subjects[i].currentLevel==2)
                cout << i+1 << ". " << subjects[i].name << " (MASTERED!)\n";
            else
                cout << i+1 << ". " << subjects[i].name << " (Level " << subjects[i].currentLevel+1 << ")\n";
        }
        cout << "\n8. View Report Card\n0. Exit Program (Saves Data)\nEnter choice: ";
        cin >> choice;
        cin.ignore();

        if(choice>=1 && choice<=7) takeSubject(subjects[choice-1]);
        else if(choice==8) viewReportCard(subjects);
        else if(choice==0) saveAllProgress(subjects, studentID);
        else {
            cout << "Invalid choice. Press Enter to retry...";
            cin.get();
        }
    } while(choice!=0);

    cout << "Exiting program. Goodbye!\n";
    return 0;
}
