#include <iostream>

#include <string>

#include<fstream>

#include <vector>


#include "json.hpp"

#include <filesystem>

#include <chrono>

#include <ctime>

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
    int numQuestions, enterType, id;
    bool allTest, allWriting,display;
    string name, creator,list;
    float totallScore;
	float time;
public:
    Exams(int numQuestions, float time, string creator) {
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
        cout << "For which list of students are you creating this exam??";
        getline(cin, list);
        cout << "you want to display the exam write after creating it on students panel??(1-yes ,0-No)\n"
              "you can change the setting later so by choosing the command 6 on your panel";
        cin >> display;
        cin.ignore();
        cout << "what is the totall Score of this exam";
        cin >> totallScore;
        cin.ignore();
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
        examData["totallScore"]=totallScore;

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
        examData["time"] = time;
        examData["list"] = list;


        bool sameName=false;
        for(auto exam :jsonData){
            if(exam["name"]==name){
                sameName=true;
            }
        }
        if(!sameName){
            // Add the new exam to the JSON data
            jsonData.push_back(examData);

            // update the file content
            ofstream examsFileOut("exams.json");
            examsFileOut << jsonData.dump(4) << endl;

            cout << "Exam saved successfully to exams.json" << endl;
        }
        else{
            cout << "exma didnt save beacause another exam with the same name exit ,try with anothe name";
        }
    }
};
int Users::count = 0;
int Exams::count = 0;

int main() {
    string username;
    string pass;
    string test[4];
    string examType;
    int trying;
    vector<Exams> exams;
    int command;
    float time;
    int firstOrder;
    int loginedId, count, key;
    bool isTeacher;
    bool isValid;

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
    trying=0;
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
            for (int i = 0; i < 4; i++) {
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
    //here is where the teacher panel displays and the while loop will finish in the case user enter command 10
    if (isTeacher) {
        while (true) {
            cout << endl << "enter proper command number from list bellow" << endl;
            cout
                    << "1.Create New Exam\n2.Display all Exams\n3.Create New List\n4.Add Students To The Existing List\n5.Display Lists"
                    <<
                    "\n6.change the display settings of exams\n7.display the students answears\n8.display the protects\n9.display the top studenst\n10.Exit The Program" << endl;
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
                //Displaying all the exams created by the logged in teacher

                //read the file
                ifstream examsFile("exams.json");
                nlohmann::json jsonData;
                examsFile >> jsonData;
                if (examsFile.is_open()) {
                    for (const auto &examData : jsonData) {
                        //check if the exam creator is the same as the logged in teacher
                        if (examData["creator"] == username) {
                            string name = examData["name"];
                            int numQuestions = examData["numQuestions"];

                            cout << "Exam name: " << name << ", number of questions: " << numQuestions << endl;

                            nlohmann::json questionsArray = examData["questions"];
                            for (const auto &questionData: questionsArray) {
                                string text = questionData["text"];
                                string type = questionData["type"];
                                string answer = questionData["answer"];

                                cout << "Question text: " << text << ", type: " << type << ", answer: " << answer
                                     << endl;
                                //check the type of question if its test display the options
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
                }
                else{
                    //in the case program cant open the exams.json file this line will print.
                    cout << "some thing wrong happened about the source of exams,Try Again Later";
                    continue;
                }
            } else if (command == 3) {
                // creating a new list of students the name of this list is used for creating exam and define the
                //students that can see this exam
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
                    cout << "Enter the id of student " << i + 1 << endl;
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
                ListData["teacher id"]=loginedId;
                jsonData.push_back(ListData);
                //update the content of lists file
                ofstream examsFileOut("lists.json");
                examsFileOut << jsonData.dump(4) << endl;
            }  else if (command == 4) {
                //add new student to the existing lists

                ifstream ListsFile("lists.json");
                nlohmann::json jsonData;
                ListsFile >> jsonData;

                string listId;
                cout << "Adding new student to the existed list" << endl;
                //display the list `s teacher id that are the same as logged in teacher
                for(auto& list :jsonData){
                    if(list["teacher id"]==loginedId){
                        for (auto &element: list.items()) {
                            string listId = element.key();
                            cout << "name of list   :  " << listId << endl;
                            break;
                        }
                    }
                }
                cout << "Enter the listId that you want to add student to from the list above :" << endl;
                getline(cin >> ws, listId);



                bool listFound = false;
                for (auto& list : jsonData) {
                    if (list.find(listId)!= list.end()) {
                        listFound = true;
                        cout << "Enter the id of the new student:" << endl;
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
                    examsFileOut << jsonData.dump(4) << endl;
                    cout << "Student added successfully to the list." << endl;
                }
            } else if (command == 5) {
                //display the lists that the teacher id of them match the logged in teacher

                ifstream ListsFile("lists.json");
                nlohmann::json jsonData;
                ListsFile >> jsonData;

                cout << "Displaying lists:" << endl;
                for (auto& list : jsonData) {
                    if (list["teacher id"]==loginedId){
                        for (auto &element: list.items()) {
                            string listId = element.key();
                            cout << "List : " << listId << endl;
                            cout << "Students in this list:" << endl;
                            for (auto &student: element.value()) {
                                cout << student << endl;
                            }
                            cout << endl;
                            break;
                        }
                    }
                }


            }
            else if(command==6){
                string name;
                cout << endl << "Which exam display settings do you want to change?";
                cin >> name;

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
                        cout << "Enter new display setting (true/false): "<<endl;
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

            }
            else if(command==7){
                string examName;
                //display the students answears
                cout << "For which test do you want to see students' answers?";
                cin >> examName;

                ifstream answersFile("answers.json");

                nlohmann::json answersJson;
                answersFile >> answersJson;

                // Search for the exam name in the JSON object
                cout << "Answers for " << examName << ":" << endl;
                bool isfound = false;
                float score;
                for (auto& answer : answersJson) {
                    if (answer["examName"] == examName) {
                        isfound = true;
                        // Print the answers
                        cout << "Answers by student"<<answer["student"] << endl;
                        for (auto& question : answer["answers"]) {
                            cout << "Question " << question["questionText"] << ": " << question["answer"] << endl;
                        }
                        cout << "enter the score(totall score is " << answer["totallScore"] << ")" << endl;
                        cin >> score;
                        answer["score"] = score;
                    }
                }
                if(!isfound){
                    cout << "no answears had saved yet";
                    continue;
                }
                answersFile.close();
                ofstream answersFileOut("answers.json");
                answersFileOut << answersJson.dump(4);
                answersFileOut.close();
            }else if(command==8){
                //display the protects on exmas
                ifstream file("answers.json");
                nlohmann::json jsonData;
                file >> jsonData;
                file.close();
                for (auto& obj : jsonData) {
                    string protest = obj["protest"];
                    if (!protest.empty()) {
                        int student = obj["student"];
                        string examName = obj["examName"];
                        cout << "Student: " << student << ",on Exam: " << examName << ", Protest text: " << protest << endl;

                        // Ask user if they want to change the protest answer
                        cout << "Do you want to change the protest answer? (y/n): "<<endl;
                        char response;
                        cin >> response;
                        if (response == 'y') {
                            cout << "Enter new protest answer: ";
                            string newProtestAnswer;
                            cin >> newProtestAnswer;
                            obj["protestAnswers"] = newProtestAnswer;
                            cout << "answer saved successfully!" << endl;
                        }
                    }
                }
                ofstream outputFile("answers.json");
                outputFile << jsonData.dump(4);
                outputFile.close();
            }
            else if(command==9){
                //top students

                ifstream examfile("exams.json");
                nlohmann::json jsonExam;
                examfile >> jsonExam;
                examfile.close();

                ifstream file("answers.json");
                nlohmann::json jsonData;
                file >> jsonData;
                file.close();

                int count;
                cout << "Enter the count of exam you want to see the average of scores for: " << endl;
                cout << "all Exams created by you" << endl;
                //diaplay all the exams created by the loggedin teacher
                for (auto& exam : jsonExam) {
                    if (exam["creator"] == username) {
                        cout << "Exam name: " << exam["name"] << endl;
                    }
                }
                cin >> count;
                cin.ignore();
                cout << endl << "now type the exam names to be chosen" << endl;

                vector<string> examNames;
                vector<int> ids;
                vector<float> scores;
                string name;

                for(int i=0;i<count;i++) {
                    cout << "Exam" << i + 1<<endl;
                    getline(cin, name);
                    examNames.push_back(name);
                }
                //save the ids and the scores of the student in the same indes number
                for (auto& obj : jsonData) {
                    for(int i=0;i<count;i++){
                        if(examNames[i]==obj["examName"]){
                            bool found = false;
                            if(ids.empty()){
                                ids.push_back(obj["student"]);
                                scores.push_back(0.0f);
                            }
                            for(int j=0;j<ids.size();j++){
                                if(ids[j]==obj["student"]){
                                    if(obj["score"]!=-1) {
                                        scores[j] += float(obj["score"]);
                                    }
                                    else{
                                        scores[j] += 0.0f;
                                    }
                                    found = true;
                                    break;
                                }
                            }
                            if(!found){
                                ids.push_back(obj["student"]);
                                if(obj["score"]!=-1){
                                    scores.push_back(obj["score"]);
                                }
                            }
                        }
                    }
                }
                for(int i=0;i<scores.size();i++){
                    scores[i]/=count;
                }
                //order the scores due to the question from least to most
                for(int i=0;i<scores.size();i++){
                    for(int j=i+1;j<scores.size();j++){
                        if(scores[j]<scores[i]){
                            float temp;
                            temp = scores[i];
                            scores[i] = scores[j];
                            scores[j] = temp;

                            int tempId;
                            tempId=ids[j];
                            ids[i]=ids[j];
                            ids[j]=temp;
                        }
                    }
                }
                int rotbe = ids.size();
                if(!ids.empty()) {
                    cout<<"Displaying the average scores of students from the selected exmas"<<endl;
                    cout<<"count of Users that had done the exam and had given their scores"<<rotbe<<endl;
                    for (int i = 0; i < ids.size(); i++) {
                        for (int j = 0; j < 4; j++) {
                            if (studentsList[j].getId() == ids[i]) {
                                cout << "rotbe" << rotbe << "\tstudent with usename " << studentsList[j].getUserName() << endl << "score : " << scores[i] << endl;
                            }
                        }
                        rotbe--;
                    }
                }

            }
            else if (command == 10) {
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
            cout << "1.See availble Exams\n2.Refer to the history of your exams and register objections to the grade\n3.display the protests answears\n4.exit";
            cout <<endl<< "command : ";
            cin >> command;
            cout<<endl;
            if (command == 1) {
                //displaying all available exams to the user and if the teacher has turned off the display mode
                //that exam won't be aceble here
                string protect;
                int protectAccpt;
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
                for (const auto &list: lists) {
                    for (const auto &item: list.items()) {
                        for (const auto &id: item.value()) {
                            if (id == studentId) {
                                foundList = item.key();
                                break;
                            }
                        }
                    }
                }
                vector<string> availbleExams;
                for (const auto &exam: exams) {
                    if (exam["list"] == foundList and exam["display"] == true) {
                        availbleExams.push_back(exam["name"]);
                    }
                }
                for (int i = 0; i < availbleExams.size(); i++) {
                    cout << "Displaying availble Exams" << endl;
                    cout << "Exam " << i + 1 << ": " << availbleExams[i] << endl;
                }
                //if there is count of availble exams..
                if (!availbleExams.empty()) {
                    cout << "enter the number of exam to display the questions";
                    cin >> examnum;
                    cin.ignore();
                    if (examnum > 0 && examnum <= availbleExams.size()) {
                        selectedExam = availbleExams[examnum - 1];
                        //displaying but the time is not fixed yet:(
                        for (const auto &exam: exams) {
                            if (exam["name"] == selectedExam) {
                                cout << "Exam Name: " << exam["name"] << endl;
                                cout << "Creator: " << exam["creator"] << endl;
                                cout << "Time: " << exam["time"] << " minutes" << endl;
                                cout << "Questions:" << endl;

                                // Read existing answers from the "answers.json" file
                                ifstream answersFile("answers.json");
                                nlohmann::json answers;
                                if (answersFile.is_open()) {
                                    answersFile >> answers;
                                    answersFile.close();
                                } else {
                                    answers = nlohmann::json::array();
                                }

                                bool examAlreadyDone = false;
                                //student only can do the exam once so here i check if there is answer with the
                                //same student id and exam name the student had done it once and cant enter the exam
                                for (const auto &existingAnswer: answers) {
                                    if (existingAnswer["student"] == loginedId &&
                                        existingAnswer["examName"] == selectedExam) {
                                        examAlreadyDone = true;
                                        break;
                                    }
                                }
                                if (examAlreadyDone) {
                                    cout << "You have already done this exam once." << endl;
                                }
                                    //if its the first time student try to answer questions
                                else {
                                    // Create a JSON object to store the answers
                                    nlohmann::json newAnswer;
                                    newAnswer["student"] = loginedId;
                                    newAnswer["examName"] = selectedExam;
                                    newAnswer["answers"] = nlohmann::json::array();
                                    newAnswer["totallScore"] = exam["totallScore"];
                                    newAnswer["score"] = -1;
                                    newAnswer["protest"] = "";
                                    newAnswer["protestAnswers"] = "";
                                    newAnswer["protestTime"] = "";

                                    //displaying the exam questions
									  time *= 60; // time as a second for comparing with timer
                                    for (const auto &question: exam["questions"]) {
                                        cout << " _. " << question["text"] << endl;
                                        if (question["type"] == "test") {
                                            cout << "Options:" << endl;
                                            for (const auto &option: question["options"]) {
                                                cout << "  - " << option << endl;
                                            }
                                        }

                                        // Get the answer from the user
                                        string answer;
                                        auto start_time = std::chrono::steady_clock::now();
                                        cout << "Enter your answer: "<<endl;
                                        getline(cin, answer);
                                        auto timer = std::chrono::steady_clock::now()-start_time;
                                        double timing = std::chrono::duration<double>(timer).count();
                                       
                                        if(timing >= time){
                                            cout<<"Your time is over \nExam finished good luck"<<endl;
                                            cout<<"Your extra time is :"<<" ";
                                            timing -= time;
                                            std::cout<<timing<<endl;
                                            break;
                                        } if( timing < time){
                                        	time -= timing;
                                            cout<<"Your remaining time is : "<<time;
                                            continue;
											}

                                        nlohmann::json questionAnswer;
                                        questionAnswer["questionText"] = question["text"];
                                        questionAnswer["answer"] = answer;

                                        newAnswer["answers"].push_back(questionAnswer);
                                    }
                                    time /= 60; // return time to minute
                                    
                                    //ask the student if he/she want to make a protest(اعتراض) on exam
                                    cout << "you can make protect on this exam enter 1 to continue or any other number to skip this part."<<endl;
                                    cin >> protectAccpt;
                                    cin.ignore();
                                    if (protectAccpt == 1) {
                                        if(!newAnswer["protest"].empty()){
                                            cout<<"you cant make protest on this exam cause you did once befor";
                                        }
                                        cout << "write your message";
                                        getline(cin, protect);
                                        newAnswer["protest"] = protect;
                                        auto time = std::chrono::system_clock::now();
                                        std::time_t now_c = std::chrono::system_clock::to_time_t(time);
                                        cout<<"Your protest saved in :"<<std::ctime(&now_c);
                                    }

                                    answers.push_back(newAnswer);

                                    // Save the answers to the same file named "answers.json"
                                    ofstream updateanswersFile("answers.json");
                                    if (updateanswersFile.is_open()) {
                                        updateanswersFile<< answers.dump(4); // pretty-print the JSON with 4 spaces indentation
                                        updateanswersFile.close();
                                    }
                                }
                            }
                        }
                    } else {
                        cout << "Invalid exam number. Please try again." << endl;
                        continue;
                    }

                }
                else{
                    cout << "There is no availbe Exam to display";
                    continue;
                }
            }
            else if(command==2){
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

                ifstream answersFile("answers.json");
                nlohmann::json answers;
                answersFile >> answers;
                answersFile.close();

                string foundList;
                //check for the student id existence in the list name that exam had created for
                //(the id of the student must be in the list that exam defines for)
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
                //if there is an exam and the histry is not empty..
                if(!historyExams.empty()) {
                    for (int i = 0; i < historyExams.size(); i++) {
                        cout << "Displaying history of exams" << endl;
                        cout << "Exam " << i + 1 << ": " << historyExams[i] << endl;

                        for (auto& answer : answers) {
                            if (answer["examName"] == historyExams[i] && answer["student"] == loginedId) {
                                //when the student try to answer the exam default value -1 is set for score
                                // this score will change if the teacher log in and set score for this exam
                                if(answer["score"]==-1){
                                    cout << "the exam creator hadn't set the score";
                                }
                                else{
                                    cout << "Score: " << answer["score"] << endl;
                                }
                            }
                        }
                    }
                }
                else{
                    cout << "history is empty";
                    continue;

                }
            }
            else if(command==3){
                //display protest answers
                ifstream file("answers.json");
                nlohmann::json jsonData;
                file >> jsonData;
                file.close();

                for (auto& obj : jsonData) {
                    int student = obj["student"];
                    if (student == loginedId) {
                        string protest = obj["protest"];
                        string protestAnswers = obj["protestAnswers"];
                        if (!protest.empty() &&!protestAnswers.empty()) {
                            cout << "for Exam" << obj["examName"] << endl;
                            cout << "your protest" << protest << endl;
                            cout << "Protest Answer: " << protestAnswers << endl;
                        }
                        else if(!protest.empty() && protestAnswers.empty()){
                            cout << "for Exam" << obj["examName"] << endl;
                            cout << "for protest" << protest << endl << "no answer had sent yet" << endl;
                            continue;
                        }
                        else if(protest.empty()){
                            cout << "for Exam" << obj["examName"] << endl;
                            cout << "there is no protest made by you" << endl;
                            continue;

                        }
                    }
                    else{
                        cout << "there is no answer made by your username";
                        continue;

                    }
                }
            }
            else if(command==4){
                cout << "Hope To See You Again,Bye:)";
                exit(0);
            }
        }
    }
    return 0;
}