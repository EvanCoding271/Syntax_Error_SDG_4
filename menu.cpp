#include <iostream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <iomanip>

using namespace std;

struct LessonLevel {
    string lesson;
    string questions[5];
    string answers[5];
};

struct Subject {
    string name;
    LessonLevel levels[3];
    int currentLevel = 0;
    int scores[3] = {0, 0, 0};
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

void takeSubject(Subject &sub) {
    string studentAnswer;
    LessonLevel currentData = sub.levels[sub.currentLevel];
    int correctCount = 0;

    for (int i = 0; i < 5; i++) {
        clearScreen();

        cout << "\n=== " << sub.name << " - Level " << sub.currentLevel + 1 << " ===\n\n";
        cout << currentData.lesson << "\n\n";

        cout << "Question " << i + 1 << ": " << currentData.questions[i] << "\n";
        cout << "Answer: ";
        getline(cin, studentAnswer);

        if (toLower(studentAnswer) == toLower(currentData.answers[i])) {
            cout << "\n[RESULT] Correct!\n";
            correctCount++;
        } else {
            cout << "\n[RESULT] Incorrect. Review the lesson content.\n";
        }

        cout << "\nPress Enter to continue...";
        cin.ignore(1000, '\n');
    }

    sub.scores[sub.currentLevel] = correctCount;

    clearScreen();

    if (sub.currentLevel < 2) {
        sub.currentLevel++;

        cout << "\n--- Level Complete ---\n";
        cout << "You scored " << correctCount << "/5 correct.\n";
        cout << "Difficulty for " << sub.name << " has increased!\n";
        cout << "Press Enter to return to the main menu and try Level " << sub.currentLevel + 1 << "...\n";

    } else {
        double totalScore = 0;
        for (int score : sub.scores) {
            totalScore += score;
        }
        sub.averageScore = (totalScore / 15.0) * 100.0;

        cout << "\n*** Subject MASTERED ***\n";
        cout << "You have finished all levels for " << sub.name << ".\n";
        cout << "Your total score for this subject is " << fixed << setprecision(2) << sub.averageScore << "%.\n";
        cout << "Press Enter to return to the main menu...";
    }

    cin.ignore(1000, '\n');
}

void viewReportCard(Subject subjects[]) {
    clearScreen();
    cout << "\n=========================================\n";
    cout << "          STUDENT REPORT CARD\n";
    cout << "=========================================\n\n";

    double overallTotalScore = 0.0;
    int subjectsMasteredCount = 0;

    cout << setw(20) << left << "SUBJECT" << setw(15) << "SCORE (%)" << setw(15) << "LEVELS" << endl;
    cout << string(50, '-') << endl;

    for (int i = 0; i < 7; ++i) {
        if (subjects[i].currentLevel == 2) {
            double totalScore = 0;
            for (int score : subjects[i].scores) {
                totalScore += score;
            }
            subjects[i].averageScore = (totalScore / 15.0) * 100.0;

            cout << setw(20) << left << subjects[i].name
                 << setw(15) << fixed << setprecision(2) << subjects[i].averageScore
                 << setw(15) << "MASTERED" << endl;

            overallTotalScore += subjects[i].averageScore;
            subjectsMasteredCount++;

        } else {
            cout << setw(20) << left << subjects[i].name
                 << setw(15) << "N/A"
                 << setw(15) << "In Progress (Lvl " + to_string(subjects[i].currentLevel + 1) + ")" << endl;
        }
    }

    cout << string(50, '-') << endl;

    if (subjectsMasteredCount > 0) {
        double overallAverage = overallTotalScore / subjectsMasteredCount;
        cout << setw(20) << left << "OVERALL AVERAGE:"
             << setw(15) << fixed << setprecision(2) << overallAverage << endl;
    } else {
        cout << "OVERALL AVERAGE: N/A (Complete at least one subject to calculate.)\n";
    }

    cout << "\n-----------------------------------------\n";
    cout << "Individual Level Scores (Correct Answers):\n";

    for (int i = 0; i < 7; ++i) {
        cout << subjects[i].name << ": ";
        if (subjects[i].currentLevel > 0) {
            cout << "L1: " << subjects[i].scores[0] << "/5, ";
        }
        if (subjects[i].currentLevel > 1) {
            cout << "L2: " << subjects[i].scores[1] << "/5, ";
        }
        if (subjects[i].currentLevel == 2) {
            cout << "L3: " << subjects[i].scores[2] << "/5";
        }
        if (subjects[i].currentLevel == 0) {
            cout << "No scores recorded yet.";
        }
        cout << endl;
    }

    cout << "\nPress Enter to return to the menu...";
    cin.ignore(1000, '\n');
}


int main() {
    Subject subjects[7] = {
        {"Math",
         {
             {"**Level 1: Addition, Subtraction, and Basic Multiplication**\n\n"
              "1. **Addition** combines groups to find the **sum**.\n"
              "2. **Subtraction** finds the **difference**.\n"
              "3. **Multiplication** is repeated addition.",
              {"3 + 4 = ?", "5 + 2 = ?", "10 - 3 = ?", "7 - 5 = ?", "3 * 4 = ?"},
              {"7", "7", "7", "2", "12"}},

             {"**Level 2: Advanced Multiplication and Division**\n\n"
              "1. **Multiplication** involves larger factors.\n"
              "2. **Division** splits a number into equal groups. The result is the **quotient**.",
              {"8 * 7 = ?", "9 * 6 = ?", "25 / 5 = ?", "18 / 3 = ?", "15 + 15 = ?"},
              {"56", "54", "5", "6", "30"}},

             {"**Level 3: Order of Operations and Exponents**\n\n"
              "1. **PEMDAS/BODMAS** dictates the order of operations: Parentheses, Exponents, Multiplication/Division, Addition/Subtraction.\n"
              "2. **Exponents** indicate repeated multiplication (e.g., $2^3 = 2 \\times 2 \\times 2$).",
              {"$2^3$ = ?", "10 - 2 * 3 = ?", "6 + 4 / 2 = ?", "(1 + 3) * 2 = ?", "5 * 5 = ?"},
              {"8", "4", "8", "8", "25"}}
         },
         0},

        {"Science",
         {
             {"**Level 1: Photosynthesis and Plant Needs**\n\n"
              "1. **Plant Needs**: Plants require **sunlight**, **water**, and **soil**.\n"
              "2. **Photosynthesis**: Plants convert sun energy into **sugar (food)** and release **oxygen**.",
              {"What do plants need to grow?", "What is the process plants use to make food?", "What gas do plants absorb from the air?", "What important gas do plants release?", "Do plants need soil?"},
              {"sunlight", "photosynthesis", "carbon dioxide", "oxygen", "yes"}
             },
             {"**Level 2: Basic Cell Structure and Classification**\n\n"
              "1. The **cell** is the basic unit of life.\n"
              "2. Life is divided into **Kingdoms** (e.g., Animalia, Plantae, Fungi) based on characteristics.",
              {"What is the basic unit of life?", "What is the study of living things called?", "Name one biological kingdom.", "Do animals have cells?", "True or False: All living things have cells."},
              {"cell", "biology", "animalia", "yes", "true"}
             },
             {"**Level 3: Ecosystems and Food Webs**\n\n"
              "1. An **Ecosystem** is a community of living and non-living things interacting.\n"
              "2. A **Food Web** shows how energy flows through an ecosystem: Producers (plants) are eaten by Consumers (animals).",
              {"What is a producer in a food web?", "What does an ecosystem include?", "What type of consumer eats only plants?", "Is light a living or non-living part of an ecosystem?", "True or False: The sun powers most food webs."},
              {"plants", "living and non-living things", "herbivore", "non-living", "true"}
             }
         },
         0},

        {"English",
         {
             {"**Level 1: Simple Sentences and Punctuation**\n\nA **simple sentence** has one independent clause (one subject and one verb) and expresses a complete thought.",
             {"She ___ happy.", "I ___ apples.", "They ___ running.", "He ___ tall.", "We ___ playing."},
             {"is", "like", "are", "is", "are"}
             },
             {"**Level 2: Compound Sentences**\n\nA **compound sentence** joins two independent clauses using a coordinating conjunction (FANBOYS: For, And, Nor, But, Or, Yet, So) and a comma.",
             {"I like dogs ___ she likes cats.", "We can go now ___ wait until later.", "He ran fast ___ he missed the bus.", "I studied, ___ I passed the test.", "The sun is hot ___ the moon is cool."},
             {"but", "or", "but", "so", "and"}
             },
             {"**Level 3: Complex Sentences**\n\nA **complex sentence** contains one independent clause and one or more dependent clauses, linked by a subordinating conjunction (e.g., because, although, since, when, if).",
             {"___ it rained, we stayed inside.", "I will call you ___ I get home.", "She was happy ___ she won the game.", "We left early ___ we wanted to avoid traffic.", "I bought a new book ___ I finished the old one."},
             {"because", "when", "because", "since", "after"}
             }
        }, 0},

        {"Filipino",
         {
             {"**Level 1: Wika at Kultura**\n\nAng **wika** ay ang pangunahing paraan ng **pakikipag-ugnayan**. Ang pambansang wika ng Pilipinas ay **Filipino**.",
             {"Ano ang pambansang wika ng Pilipinas?", "Ano ang tawag sa paraan ng pakikipag-usap?", "Gaano kahalaga ang wika?", "Gamitin mo ba ang wika araw-araw?", "Wika ba ay mahalaga?"},
             {"filipino", "wika", "mahalaga", "oo", "oo"}
             },
             {"**Level 2: Bahagi ng Pananalita (Parts of Speech)**\n\nAng mga pangunahing bahagi ay **Pangngalan** (Noun), **Pandiwa** (Verb), at **Pang-uri** (Adjective).",
             {"Ang salitang 'bahay' ay isang ___.", "Ang salitang 'tumakbo' ay isang ___.", "Ang salitang 'maganda' ay isang ___.", "Anong bahagi ng pananalita ang Pangngalan?", "Pang-uri ba ang salitang 'mabagal'?"},
             {"pangngalan", "pandiwa", "pang-uri", "noun", "oo"}
             },
             {"**Level 3: Wastong Gamit ng mga Salita**\n\nAng tamang paggamit ng mga salitang may pagkakaiba (e.g., *may* vs. *mayroon*, *ng* vs. *nang*).",
             {"Ano ang tamang gamit: Ako (may/mayroon) libro.", "Naglaro sila (ng/nang) malakas.", "Kailangan mo ba (ng/nang) payong?", "Anong tamang salita: (May/Mayroon) bisita si lola.", "Ako ay kumain (ng/nang) saging."},
             {"may", "nang", "ng", "mayroon", "ng"}
             }
         }, 0},

        {"Araling Panlipunan",
         {
             {"**Level 1: Mga Bayani ng Pilipinas**\n\nAng ating mga **bayani** tulad ni **Jose Rizal** ay lumaban para sa kalayaan sa pamamagitan ng **edukasyon** at **pagsulat**.",
             {"Who is Jose Rizal?", "What did he write?", "Is he a national hero?", "Did he fight for freedom?", "Should we remember him?"},
             {"national hero", "books", "yes", "yes", "yes"}
             },
             {"**Level 2: Pamahalaan at Estruktura**\n\nAng pamahalaan ay may tatlong sangay: **Ehekutibo**, **Lehislatibo**, at **Hudikatura**.",
             {"Ilang sangay mayroon ang pamahalaan?", "Ano ang sangay na pinamumunuan ng Presidente?", "Ano ang tawag sa sangay na gumagawa ng batas?", "Saan nabibilang ang Korte Suprema?", "Sino ang pinuno ng sangay Ehekutibo?"},
             {"tatlo", "ehekutibo", "lehislatibo", "hudikatura", "presidente"}
             },
             {"**Level 3: Heograpiya at Rehiyon**\n\nAng Pilipinas ay isang **arkipelago** at nahahati sa maraming **rehiyon** at **lalawigan** (provinces).",
             {"Ano ang tawag sa pagiging maraming isla ng Pilipinas?", "Ano ang tawag sa dibisyon ng bansa na may magkakatulad na kultura?", "Ano ang capital ng Pilipinas?", "Ilan ang pangunahing isla ng Pilipinas?", "True or False: Ang Pilipinas ay matatagpuan sa Asia."},
             {"arkipelago", "rehiyon", "manila", "tatlo", "true"}
             }
         }, 0},

        {"E.S.P",
         {
             {"**Level 1: Honesty and Ethical Values**\n\nAng **Honesty (Katapatan)** ay ang pagsasabi ng **katotohanan** sa lahat ng oras. Ito ay nagtatatag ng **tiwala**.",
             {"What does honesty mean?", "Should we be honest?", "Honesty builds ___?", "Who should we be honest with?", "Is honesty good?"},
             {"telling the truth", "yes", "trust", "everyone", "yes"}
             },
             {"**Level 2: Respeto at Responsibilidad**\n\nAng **Respeto** ay paggalang sa kapwa, at ang **Responsibilidad** ay pagtupad sa mga tungkulin (duties).",
             {"Ano ang ibig sabihin ng paggalang sa kapwa?", "Ang paggawa ba ng takdang aralin ay responsibilidad?", "Dapat ba nating igalang ang nakatatanda?", "Sino ang dapat nating respetuhin?", "True or False: Mahalaga ang pagiging responsable."},
             {"respeto", "oo", "oo", "lahat", "true"}
             },
             {"**Level 3: Patriotism and Community**\n\nAng **Patriotism (Pagmamahal sa Bayan)** ay pagmamalaki sa ating bansa. Ang **Community Service** ay pagtulong sa komunidad.",
             {"Ano ang tawag sa pagmamahal sa bayan?", "Dapat ba tayong maging proud sa ating bansa?", "Ang pagtulong ba sa komunidad ay bahagi ng E.S.P?", "Ano ang dapat nating gawin sa ating basura?", "True or False: Ang pagtatapon ng basura sa ilog ay pagmamahal sa bayan."},
             {"patriotism", "oo", "oo", "itapon sa tama", "false"}
             }
         }, 0},

        {"P.E",
         {
             {"**Level 1: Exercise, Fitness, and Health**\n\nAng **Exercise** ay mahalaga upang panatilihing **malakas at malusog** ang ating katawan. Kasama sa mga activity ang **pagtakbo** at **pagtalon**.",
             {"Name one activity that is exercise.", "Does exercise make you healthy?", "Can you run to exercise?", "Is jumping exercise?", "Should we exercise daily?"},
             {"running", "yes", "yes", "yes", "yes"}
             },
             {"**Level 2: Safety and First Aid**\n\nLaging mag-stretching bago at pagkatapos mag-ehersisyo. Ang **First Aid** ay agarang tulong sa nasaktan.",
             {"Dapat bang mag-stretching bago mag-ehersisyo?", "Ano ang agarang tulong sa nasaktan?", "Ano ang tawag sa pag-iinit ng katawan bago mag-exercise?", "True or False: Ang ehersisyo ay nakasasama sa puso.", "Kapag may sugat, dapat ba itong linisin?"},
             {"oo", "first aid", "stretching", "false", "oo"}
             },
             {"**Level 3: Nutrition and Balanced Diet**\n\nAng **Nutrition** ay ang pagkuha ng tamang pagkain. Ang **Balanced Diet** ay pagkain ng iba't ibang uri ng pagkain sa tamang dami.",
             {"Ano ang tawag sa pagkain ng tamang uri at dami ng pagkain?", "Ang gulay ba ay masustansya?", "Anong sustansya ang nakukuha sa karne at itlog?", "Dapat ba nating bawasan ang pagkain ng matatamis?", "True or False: Kailangan natin ng protein para sa lakas ng buto."},
             {"balanced diet", "oo", "protein", "oo", "false"}
             }
         }, 0}
    };

    int choice;

    do {
        clearScreen();
        cout << "\n=== STUDENT MENU ===\n";
        cout << "Choose a subject to study:\n";

        for (int i = 0; i < 7; i++) {
            if (subjects[i].currentLevel == 2) {
                cout << i + 1 << ". " << subjects[i].name << " (MASTERED!)" << endl;
            } else {
                cout << i + 1 << ". " << subjects[i].name << " (Level " << subjects[i].currentLevel + 1 << ")" << endl;
            }
        }

        cout << "\n8. View Report Card\n";
        cout << "0. Exit Program\n";
        cout << "Enter your choice: ";

        cin >> choice;
        cin.ignore(1000, '\n');

        if (choice >= 1 && choice <= 7) {
            if (subjects[choice - 1].currentLevel > 2) {
                 cout << "\nYou have already mastered " << subjects[choice - 1].name << ". Press Enter to continue...\n";
                 cin.ignore(1000, '\n');
            } else {
                takeSubject(subjects[choice - 1]);
            }
        } else if (choice == 8) {
            viewReportCard(subjects);
        }
         else if (choice == 0) {
            cout << "\nExiting menu. Thank you for using the learning application.\n";
        } else {
            cout << "\nInvalid choice entered. Press Enter to retry...\n";
            cin.ignore(1000, '\n');
        }
    } while (choice != 0);

    return 0;
}
