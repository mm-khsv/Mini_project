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
		bool allTest, allWriting;
		string name, creator;
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
		//when showExam called some datas about exam will be shown and after that on each object of questions
		// the display method will print the question datas such as the 4 options the question text and answer....
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
			std::string filename = "exams.json";
			cout << "Opening file: " << filename << endl;
			ofstream examsFile(filename, ios::out |  ios::app);
			// Create a JSON object to store exam data
			nlohmann::json examData;
			examData["name"] = name;
			examData["numQuestions"] = numQuestions;

			// Create a JSON array to store question data
			nlohmann::json questionsArray;
			for (int i = 0; i < numQuestions; i++) {
				nlohmann::json questionData;
				questionData["text"] = questions[i].text;
				questionData["type"] = questions[i].type;
				questionData["answer"] = questions[i].answer;

				// Add options only if question type is "test"
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

			// Write the JSON object to the file
			examsFile << examData << std::endl;

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
    teacherList[0].getData("tanaz", "kjnv");
    teacherList[1].getData("mari", "785");
    teacherList[2].getData("tina", "852");

    Students studentsList[4];
    studentsList[0].getData("sara", "111");
    studentsList[1].getData("ali", "215");
    studentsList[2].getData("mmd", "639");
    studentsList[3].getData("sana", "693");

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
    //here is where the teacher panel displays and the while loop will finish in the case user enter command 6
    if (isTeacher) {
        while (true) {
            cout << endl << "enter proper command number from list bellow" << endl;
            cout
                    << "1.Create New Exam\n2.Display all Exams\n3.Create New List\n4.Add Students To The Existing List\n5.Display Lists"
                    <<
                    "\n6.Exit The Program" << endl;
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
//                if (!exams.empty()) {
//                    cout << "Displaying all exams..." << endl;
//                    for (int i = 0; i < Exams::returnCount(); i++) {
//                        exams[i].showExam();
//                    }
//                }
                std::ifstream examsFile("exams.json");
                if (examsFile.is_open()) {
                    std::string jsonString;
                    std::string line;
                    while (std::getline(examsFile, line)) {
                        jsonString += line;
                    }
                    nlohmann::json examsData = nlohmann::json::parse(jsonString);

                    for (const auto &examData: examsData) {
                        std::string name = examData["name"];
                        int numQuestions = examData["numQuestions"];

                        cout << "Exam name: " << name << ", number of questions: " << numQuestions << endl;

                        const auto &questionsArray = examData["questions"];
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

                } else if (command == 3) {
                    int tedad;
                    string listId;
                    cout << "Creating new list" << endl;
                    cout << "Enter listId (Includes both number and text or alone) :" << endl;
                    getline(cin >> ws, listId);
                    cout << "What is the maximum number of your list?" << endl;
                    cin >> tedad;
                    string id[tedad];
                    for (int i = 0; i < tedad; i++) {
                        cout << "Enter the password of student " << i + 1 << endl;
                        getline(cin >> ws, id[i]);
                    }
                    for (int j = 0; j < tedad; j++) {
                        for (int i = 0; i < 4; i++) {
                            if (studentsList[i].getPassWord() == id[j]) {
                                studentsList[i].updateId(listId);
                            }
                        }
                    }

                } else if (command == 4) {
                    int t;
                    string list;
                    cout << "Adding new student to the existed list" << endl;
                    cout << "Enter the listId that you want to add student to :" << endl;
                    getline(cin >> ws, list);
                    cout << "How many students do you want to add?" << endl;
                    cin >> t;
                    string id[t];
                    for (int i = 0; i < t; i++) {
                        cout << "Enter the password of student " << i + 1 << endl;
                        getline(cin >> ws, id[i]);
                    }
                    for (int i = 0; i < 4; i++) {
                        for (int j = 0; j < t; j++) {
                            if (studentsList[i].getPassWord() == id[j]) {
                                studentsList[i].updateId(list);
                            }
                        }
                    }
                } else if (command == 5) {
                    string listId;
                    cout << "Displaying lists" << endl;
                    cout << "Enter the listId that you want to see" << endl;
                    getline(cin >> ws, listId);
                    ofstream file1("list.txt", ios::app | ios::out);
                    file1 << listId;
                    cout << "Student list of " << listId << " :" << endl;
                    for (int i = 0; i < 4; i++) {
                        if (studentsList[i].returnId() == listId) {
                            file1 << studentsList[i].getUserName() << " ";
                            file1 << studentsList[i].getPassWord() << endl;
                            cout << studentsList[i].getUserName() << " ";
                            cout << studentsList[i].getPassWord() << endl;
                        }
                    }
                } else if (command == 6) {
                    cout << "Hope to see you soon :)" << endl;
                    exit(0);
                } else {
                    cout << "Invalid command. Please try again." << endl;
                }
            }
        }
        return 0;
    }
}