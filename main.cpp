#include <iostream>

#include <string>

#include<fstream>

#include <vector>


#include <fstream>


#include "json.hpp"


#include <filesystem>


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
        //the id will help me find the user in the file of users in json format
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
class Students: public Users {
private:
    string listId;
public:
    string returnId() {
        return listId;
    }
    void updateId(string givenId) {
        listId = givenId;
    }

};
class Teachers: public Users {};
class Questions {
public:
    string text;
    string type;
    string answer;
    string options[4];
    //will get the text of question no mather its test or writting
    void createQuestion() {
        cout << "Enter question text: " << endl;
        getline(cin, text);
    }

    void setType() {
        int type;
        cout << "Enter question type:\n1.test\n2.writting" << endl;
        cin >> type;
        cin.ignore();
        if (type == 1) {
            this -> type = "test";
        } else if (type == 2) {
            this -> type = "writting";
        }
    }

    void setType(string givenType) {
        type = givenType;
    }
    string getType() {
        return type;
    }
    //will get the correst answer and 4 options  of the question with type of test
    void createTest() {
        for (int i = 0; i < 4; i++) {
            cout << "Enter option " << i + 1 << " : " << endl;
            getline(cin, options[i]);
        }
        cout << "Enter correct answer: " << endl;
        getline(cin, answer);
    }
    //will get the answer of the question with type writting
    void createWriting() {
        cout << "Enter answer: " << endl;
        getline(cin, answer);
    }
    // This method displays the questions that are test or written
    void display() {
        cout << "Question: " << this -> text << endl;
        if (this -> type == "test") {
            cout << "Options:" << endl;
            for (int i = 0; i < 4; i++) {
                cout << i + 1 << ". " << this -> options[i] << endl;
            }
            cout << "Correct answer: " << this -> answer << endl;
        } else if (this -> type == "writting") {
            cout << "Answer: " << this -> answer << endl;
        }
        for (int i = 0; i < 20; i++) {
            cout << "*";
        }
    }
};
//with composition each exam object has an array of objects quetions
class Exams {
    static int count;
    Questions * questions;
    int numQuestions, enterType, id, time;
    bool allTest, allWriting,display;
    string name, creator,list;
public:
    Exams(int numQuestions, int time, string creator) {
        this -> numQuestions = numQuestions;
        questions = new Questions[numQuestions];
        this -> time = time;
        this -> creator = creator;
        count++;
        //the id will help us find the exam in the json file that contains exams
        this -> id = count;
    }

    static int returnCount() {
        return count;
    }
    //this method will fill the questions array of objects
    void createExam() {
        cout << "Write a name for your exam: " << endl;
        getline(cin, name);
        cout << "What Type of Questions your Exam Has:\n1.All Tests\n2.All Writing\n3.Combinatorial\n";
        cin >> enterType;
        cin.ignore();
        cout<<"For which list of students are you creating this exam??";
        getline(cin, list);
        cout<<"you want to display the exam write after creating it on students panel??(1-yes ,0-No)\n"
              "you can change the setting later so by choosing the command 6 on your panel";
        cin>>display;
        // Asking the type of questions will be useful if the exam only consists of tests or written questions
        //no need to ask for every question what is the type of this question
        if (enterType == 1) {
            allTest = true;
            allWriting = false;
        } else if (enterType == 2) {
            allWriting = true;
            allTest = false;
        } else if (enterType == 3) {
            allTest = false;
            allWriting = false;
        }

        for (int i = 0; i < numQuestions; i++) {
            cout << "Question " << i + 1 << ":" << endl;
            //no mather what is the type of question it should have question text so..
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
    //when showExam called some datas about "exam" will be shown and after that on each object of questions
    // the display method will print the "question" datas such as the 4 options ,the question text and answer....
    void showExam() {
        cout << endl << endl << name << " : " << endl;
        cout << "Totall Number Of Questions : " << numQuestions << endl;
        cout << "Totall time : " << time << endl;
        cout << "created by : " << creator << endl;
        for (int i = 0; i < numQuestions; i++) {
            cout << "Question " << (i + 1) << ":" << endl;
            this -> questions[i].display();
            cout << endl;
        }
    }
    void saveExamJSON() {
        ifstream examsFile("exams.json");
        //  if file doesn't exist, create it
        if (!examsFile.good()) {
            ofstream examsFile("exams.json");
            examsFile.close();
        }
        examsFile.open("exams.json");

        // Read the json data from the existing file
        nlohmann::json jsonData;
        examsFile.seekg(0, ios::end);
        //check if the file is empty fill the array of json data with empty array
        if (examsFile.tellg() == 0) {
            jsonData = nlohmann::json::array();
        } else {
            examsFile.seekg(0, ios::beg);
            examsFile >> jsonData;
        }

        // Create a JSON object to save the new exams
        nlohmann::json examData;
        examData["id"] =id;
        examData["name"] = name;
        examData["numQuestions"] = numQuestions;
        examData["creator"] = creator;
        examData["display"]=display;

        // Create a JSON array to save new exmas`s question datas
        nlohmann::json questionsArray;
        for (int i = 0; i < numQuestions; i++) {
            nlohmann::json questionData;
            questionData["text"] = questions[i].text;
            questionData["type"] = questions[i].type;
            questionData["answer"] = questions[i].answer;

            // if question type is test
            if (questions[i].type == "test") {
                nlohmann::json optionsArray;
                for (int j = 0; j < 4; j++) {
                    optionsArray.push_back(questions[i].options[j]);
                }
                questionData["options"] = optionsArray;
            }

            questionsArray.push_back(questionData);
        }

        examData["questions"] = questionsArray;
        examData["time"]=time;
        examData["list"]=list;

        // Add the new exam to the JSON data
        jsonData.push_back(examData);

        // update the file content
        ofstream examsFileOut("exams.json");
        examsFileOut << jsonData.dump(4) << std::endl;

        cout << "Exam saved successfully to exams.json" << endl;
    }
};
int Users::count = 0;
int Exams::count = 0;

int main() {
    string username;
    string pass;
    int trying = 0;
    vector<Exams> exams;
    string test[4];
    int loginedId, count, key;
    bool isTeacher;
    string examType;
    bool isValid;
    int command;
    int time;
    int firstOrder;
    //Default users defined here
    Teachers teacherList[3];
    teacherList[0].getData("tanaz", "kjnv");//id=1
    teacherList[1].getData("mari", "785");//id=2
    teacherList[2].getData("tina", "852");//id=3

    Students studentsList[4];
    studentsList[0].getData("sara", "111"); // id=4
    studentsList[1].getData("ali", "215");//id=5
    studentsList[2].getData("mmd", "639");//id=6
    studentsList[3].getData("sana", "693");//id=7

    //user can attempt to login for 3 times the last time if username and password didnt match the program will finish.
    while (trying < 3) {
        cout << "Enter the username: " << endl;
        getline(cin, username);

        cout << "Enter the password: " << endl;
        getline(cin, pass);
        for (int j = 0; j < 20; j++) {
            cout << "-";
        }
        cout << endl;
        isValid = false;
        for (int i = 0; i < 3; i++) {
            if (teacherList[i].getUserName() == username && teacherList[i].getPassWord() == pass) {
                cout << "Welcome  " << teacherList[i].getUserName() << " Role : Teacher" << endl;
                for (int j = 0; j < 20; j++) {
                    cout << "-";
                }
                loginedId = teacherList[i].getId();
                isTeacher = true;
                isValid = true;
                break;
            }
        }

        if (!isTeacher) {
            for (int i = 0; i < 9; i++) {
                if (studentsList[i].getUserName() == username && studentsList[i].getPassWord() == pass) {
                    cout << "Welcome  " << studentsList[i].getUserName() << " Role : Student" << endl;
                    loginedId = studentsList[i].getId();
                    isTeacher = false;
                    isValid = true;
                    break;
                }
            }
        }

        if (isValid) {
            break;
        } else {
            trying++;
            if (trying < 3)
                cout << "User Didnt Found ,Try again." << endl;
        }
    }
    if (trying == 3) {
        cout << "Try Again Later.." << endl;
        exit(0);
    }
    cout << endl;
    //here is where the teacher panel displays and the while loop will finish in the case user enter command 8
    if (isTeacher) {
        while (true) {
            cout << endl << "enter proper command number from list bellow" << endl;
            cout
                    << "1.Create New Exam\n2.Display all Exams\n3.Create New List\n4.Add Students To The Existing List\n5.Display Lists"
                    <<
                    "\n6.change the display settings of exams\n7.display the history of exams\n8.Exit The Program" << endl;
            cout << "command : ";
            cin >> command;
            cout << endl;
            if (command == 1) {
                int numQuestions;
                cout << "Creating Exam" << endl;
                cout << "Enter number of questions: " << endl;
                cin >> numQuestions;
                cout << "Enter the totall time in minute" << endl;
                cin >> time;
                cin.ignore();
                Exams exam(numQuestions, time, username);
                exam.createExam();
                exams.push_back(exam);
                exam.saveExamJSON();

            } else if (command == 2) {
                std::ifstream examsFile("exams.json");
                if (examsFile.is_open()) {
                    nlohmann::json examsData = nlohmann::json::parse(examsFile);

                    for (const auto &examData: examsData) {
                        std::string name = examData["name"];
                        int numQuestions = examData["numQuestions"];

                        cout << "Exam name: " << name << ", number of questions: " << numQuestions << endl;

                        nlohmann::json questionsArray = examData["questions"];
                        for (const auto &questionData: questionsArray) {
                            std::string text = questionData["text"];
                            std::string type = questionData["type"];
                            std::string answer = questionData["answer"];

                            cout << "Question text: " << text << ", type: " << type << ", answer: " << answer << endl;

                            if (type == "test") {
                                const auto &optionsArray = questionData["options"];
                                cout << "Options: ";
                                for (const auto &option: optionsArray) {
                                    cout << option << " ";
                                }
                                cout << endl;
                            }
                        }
                        cout << endl;
                    }
                }
            } else if (command == 3) {
                ifstream ListsFile("lists.json");
                //  if file doesn't exist, create it
                if (!ListsFile.good()) {
                    ofstream listsFileOut("lists.json");
                    listsFileOut.close();
                }
                ListsFile.open("lists.json");
                nlohmann::json jsonData;
                ListsFile.seekg(0, ios::end);
                if (ListsFile.tellg() == 0) {
                    // File is empty, create a new JSON array
                    jsonData = nlohmann::json::array();
                } else {
                    ListsFile.seekg(0, ios::beg);
                    ListsFile >> jsonData;
                }
                string listId;
                cout << "Creating new list" << endl;
                cout << "Enter listId (Includes both number and text or alone) :" << endl;
                getline(cin >> ws, listId);
                cout << "What is the maximum number of your list?" << endl;
                int tedad;
                cin >> tedad;
                string id[tedad];

                nlohmann::json ListData;
                ListData[listId] = nlohmann::json::array();
                for (int i = 0; i < tedad; i++) {
                    cout << "Enter the password of student " << i + 1 << endl;
                    getline(cin >> ws, id[i]);
                    ListData[listId].push_back(id[i]);
                }
                for (int j = 0; j < tedad; j++) {
                    for (int i = 0; i < 4; i++) {
                        if (studentsList[i].getPassWord() == id[j]) {
                            studentsList[i].updateId(listId);
                        }
                    }
                }

                jsonData.push_back(ListData);
                //update the content of lists file
                ofstream examsFileOut("lists.json");
                examsFileOut << jsonData.dump(4) << std::endl;
            }  else if (command == 4) {
                string listId;
                cout << "Adding new student to the existed list" << endl;
                cout << "Enter the listId that you want to add student to :" << endl;
                getline(cin >> ws, listId);

                ifstream ListsFile("lists.json");
                nlohmann::json jsonData;
                ListsFile >> jsonData;

                bool listFound = false;
                for (auto& list : jsonData) {
                    if (list.find(listId)!= list.end()) {
                        listFound = true;
                        cout << "Enter the password of the new student:" << endl;
                        string newPassword;
                        getline(cin >> ws, newPassword);
                        list[listId].push_back(newPassword);

                        for (int i = 0; i < 4; i++) {
                            if (studentsList[i].getPassWord() == newPassword) {
                                studentsList[i].updateId(listId);
                            }
                        }
                        break;
                    }
                }

                if (!listFound) {
                    cout << "List not found. Please try again." << endl;
                } else {
                    ofstream examsFileOut("lists.json");
                    //update the content of lists file
                    examsFileOut << jsonData.dump(4) << std::endl;
                    cout << "Student added successfully to the list." << endl;
                }
            } else if (command == 5) {
                ifstream ListsFile("lists.json");
                nlohmann::json jsonData;
                ListsFile >> jsonData;

                cout << "Displaying lists:" << endl;
                for (auto& list : jsonData) {
                    for (auto& element : list.items()) {
                        string listId = element.key();
                        cout << "List : " << listId << endl;
                        cout << "Students in this list:" << endl;
                        for (auto& student : element.value()) {
                            cout << student << endl;
                        }
                        cout << endl;
                    }
                }
            }
            else if(command==6){
                string name;
                cout<<endl<<"Which exam display settings do you want to change?";
                cin>>name;

                // Read the examm file
                ifstream examsFile("exams.json");
                nlohmann::json examsJson;
                examsFile >> examsJson;
                examsFile.close();
                bool examFound = false;
                for (auto& exam : examsJson) {
                    if (exam["name"] == name) {
                        examFound = true;
                        // Update the display property
                        cout << "Enter new display setting (true/false): ";
                        bool newDisplay;
                        cin >> newDisplay;
                        exam["display"] = newDisplay;
                        break;
                    }
                }

                if (!examFound) {
                    cout << "Exam not found." << endl;
                }
                // update file
                ofstream updatedExamsFile("exams.json");
                updatedExamsFile << examsJson.dump(4)<<endl;
                updatedExamsFile.close();

                cout << "Exam display settings updated successfully." << endl;

            }else if(command==7){
                //display the history
                ifstream file("exams.json");
                nlohmann::json jsonData;
                file >> jsonData;
                file.close();

                for (auto& exam : jsonData) {
                    if (exam["creator"] == username) {
                        std::cout << "Exam name: " << exam["name"] << std::endl;
                    }
                }
            }
            else if (command == 8) {
                cout << "Hope to see you soon :)" << endl;
                exit(0);
            } else {
                cout << "Invalid command. Please try again." << endl;
            }
        }
    }
    //the logged in user is student
    else{
        while(true){
            cout << endl << "enter proper command number from list bellow" << endl;
            cout
                    << "1.See availble Exams\n2.Refer to the history of your exams and register objections to the grade\n3.exit";
            cout <<endl<< "command : ";
            cin >> command;
            cout<<endl;
            if(command==1){
                int examnum;
                string selectedExam;
                string studentId = to_string(loginedId);
                //exams to be done
                ifstream examsFile("exams.json");
                nlohmann::json exams;
                examsFile >> exams;
                examsFile.close();

                ifstream listsFile("lists.json");
                nlohmann::json lists;
                listsFile >> lists;
                listsFile.close();

                string foundList;
                for (const auto& list : lists) {
                    for (const auto& item : list.items()) {
                        for (const auto& id : item.value()) {
                            if (id == studentId) {
                                foundList = item.key();
                                break;
                            }
                        }
                    }
                }
                vector<string>availbleExams;
                for (const auto& exam : exams) {
                    if (exam["list"] == foundList and exam["display"]==true) {
                        availbleExams.push_back(exam["name"]);
                    }
                }
                for(int i=0;i<availbleExams.size();i++){
                    cout<<"Displaying availble Exams"<<endl;
                    cout << "Exam " <<i+1<<": " << availbleExams[i] << endl;
                }
                if(!availbleExams.empty()) {
                    cout << "enter the number of exam to display the questions";
                    cin >> examnum;
                    if (examnum > 0 && examnum <= availbleExams.size()) {
                        selectedExam = availbleExams[examnum - 1];
                        //displaying but the time is not fixed yet:(
                        for (const auto &exam: exams) {
                            if (exam["name"] == selectedExam) {
                                cout << "Exam Name: " << exam["name"] << endl;
                                cout << "Creator: " << exam["creator"] << endl;
                                cout << "Time: " << exam["time"] << " minutes" << endl;
                                cout << "Questions:" << endl;
                                for (const auto &question: exam["questions"]) {
                                    cout << "  - " << question["text"] << endl;
                                    if (question["type"] == "test") {
                                        cout << "    Options:" << endl;
                                        for (const auto &option: question["options"]) {
                                            cout << "      - " << option << endl;
                                        }
                                    }
                                    break;
                                }
                            }
                        }
                    } else {
                        cout << "Invalid exam number. Please try again." << endl;
                    }
                }
            }
            if(command==2){
                //history and protest
                string studentId = to_string(loginedId);

                ifstream examsFile("exams.json");
                nlohmann::json exams;
                examsFile >> exams;
                examsFile.close();

                ifstream listsFile("lists.json");
                nlohmann::json lists;
                listsFile >> lists;
                listsFile.close();

                string foundList;
                for (const auto& list : lists) {
                    for (const auto& item : list.items()) {
                        for (const auto& id : item.value()) {
                            if (id == studentId) {
                                foundList = item.key();
                                break;
                            }
                        }
                    }
                }
                vector<string>historyExams;
                for (const auto& exam : exams) {
                    if (exam["list"] == foundList) {
                        historyExams.push_back(exam["name"]);
                    }
                }
                if(!historyExams.empty()) {
                    for (int i = 0; i < historyExams.size(); i++) {
                        cout << "Displaying history of exams" << endl;
                        cout << "Exam " << i + 1 << ": " << historyExams[i] << endl;
                    }
                }
                else{
                    cout<<"history is empty";
                }
            }
            else if(command==3){
                cout<<"Hope To See You Again,Bye:)";
                exit(0);
            }
        }
    }
    return 0;
}