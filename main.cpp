#include <iostream>
#include <string>
using namespace std;

class Users {
    static int count;
    int id;
    string passWord;
    string userName;

public:
    void getData(string givenname, string givenpass) {
        userName = givenname;
        passWord = givenpass;
        count++;
        id = count;
    }
    string getUserName() {
        return userName;
    }
    string getPassWord() {
        return passWord;
    }
    int getId() {
        return id;
    }
};
class Students : public Users {

};
class Teachers : public Users {

};
class Questions {
public:
    string text;
    string type;
    string answer;
    string options[4];

    void createQuestion() {
        cout << "Enter question text: "<<endl;
        getline(cin, text);
    }

    void setType() {
        cout << "Enter question type (test or writing): ";
        getline(cin, type);
    }

    void setType(string givenType){
        type = givenType;
    }
    string getType(){
        return type;
    }
    void createTest() {
        for (int i = 0; i < 4; i++) {
            cout << "Enter option " << i + 1 << " : "<<endl;
            getline(cin, options[i]);
        }
        cout << "Enter correct answer: "<<endl;
        getline(cin, answer);
    }

    void createWriting() {
        cout << "Enter answer: ";
        getline(cin, answer);
    }

    void display() {
        cout << "Question: " << text << endl;
        if (type == "test") {
            cout << "Options:" << endl;
            for (int i = 0; i < 4; i++) {
                cout << i + 1 << ". " << options[i] << endl;
            }
            cout << "Correct answer: " << answer << endl;
        } else if (type == "writting") {
            cout << "Answer: " << answer << endl;
        }
        for(int i=0;i<20;i++){
            cout<<"*";
        }
    }
};
class Exams {
public:
    static int count;
    Questions* questions;
    int numQuestions, enterType;
    bool allTest, allWriting;
    string name;

    Exams(){
        name = "Didnt set";
    }
    Exams(int numQuestions) {
        this->numQuestions = numQuestions;
        questions = new Questions[numQuestions];
        count++;
    }

    ~Exams() {
        delete[] questions;
    }

    static int returnCount(){
        return  count;
    }
    void createExam() {
        cout << "Write a name for your exam: "<<endl;
        getline(cin, name);
        cout << "What type of questions your exam have: all Test(1), all Writing(2) or Both(3), enter the proper number: "<<endl;
        cin >> enterType;
        cin.ignore();

        if (enterType==1) {
            allTest = true;
            allWriting = false;
        } else if (enterType==2) {
            allWriting = true;
            allTest = false;
        }
        else if(enterType == 3){
            allTest = false;
            allWriting = false;
        }

        for (int i = 0; i < numQuestions; i++) {
            cout << "Enter question " << i + 1 << ":" << endl;
            questions[i].createQuestion();
            if (allTest) {
                questions[i].setType("test");
                questions[i].createTest();
            } else if (allWriting) {
                questions[i].setType("writting");
                questions[i].createWriting();
            } else {
                questions[i].setType();
                if (questions[i].getType() == "test") {
                    questions[i].createTest();
                } else if (questions[i].getType() == "writting") {
                    questions[i].createWriting();
                }
            }
        }
    }

    void showExam() {
        cout<<endl<<endl<<name<<" : "<<endl;
        for (int i = 0; i < numQuestions; i++) {
            cout << "Question " << (i + 1) << ":" << endl;
            questions[i].display();
            cout << endl;
        }
    }
};
int Users::count = 0;
int Exams::count = 0;

int main() {
    string username;
    string pass;
    string test[4];
    int loginedId, count, key;
    bool isTeacher;
    string examType;
    Exams *exams;
    Teachers teacherList[3];
    teacherList[0].getData("tanaz", "kjnv");
    teacherList[1].getData("mari", "785");
    teacherList[2].getData("tina", "852");

    Students studentsList[9];
    studentsList[0].getData("sara", "111");
    studentsList[1].getData("ali", "215");
    studentsList[2].getData("mmd", "639");

    cout << "Enter the username: "<<endl;
    getline(cin, username);


    cout << "Enter the password: "<<endl;
    getline(cin, pass);


    for (int i = 0; i < 3; i++) {
        if (teacherList[i].getUserName() == username && teacherList[i].getPassWord() == pass) {
            cout << "Welcome  " << teacherList[i].getUserName()<< " role : Teacher" << endl;
            loginedId = teacherList[i].getId();
            isTeacher = true;
            break;
        }
    }

    if (!isTeacher) {
        for (int i = 0; i < 9; i++) {
            if (studentsList[i].getUserName() == username && studentsList[i].getPassWord() == pass) {
                cout << "Welcome  " << studentsList[i].getUserName() << " role : Student" << endl;
                loginedId = studentsList[i].getId();
                isTeacher = false;
                break;
            }
        }
    }

    if (isTeacher) {
        string command;
        while (true) {
            cout << "Enter a command (create, display, bye): "<<endl;
            getline(cin, command);

            if (command[0] == 'c') {
                int numQuestions;
                cout << "Enter number of questions: "<<endl;
                cin >> numQuestions;
                cin.ignore();

                Exams exam(numQuestions);
                exam.createExam();

            } else if (command[0] == 'd') {
                cout << "Displaying all exams..." << endl;
                // exam.displayExam();
            } else if (command[0] == 'b') {
                cout << "Goodbye!" << endl;
                exit(0);
            } else {
                cout << "Invalid command. Please try again." << endl;
            }
        }
    }

    return 0;
}