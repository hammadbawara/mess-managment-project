#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
using namespace std;

struct Student {
  int id;
  string name;
  string dept;
  int bill;
};

// GLOBAL VARIABLES
Student *studentsList;
int numOfStudents;
const string FILENAME = "data.csv";

void ClearScreen() { cout << "\033[2J\033[1;1H"; }

int takeInput(int start, int end) {
  int option;
  while (true) {
    cout << "Select: ";
    cin >> option;
    if (cin && option > start && option < end) {
      return option;
    } else {
      // remove two lines from console
      cout << "\033[2A\033[3K";
      cout << option << " is not a valid option." << endl;
      // This clear previous entered value from cin buffer
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
  }
  return 0;
}

int countLines(string FILENAME) {
  ifstream file(FILENAME);
  string line;
  int numOfLines = 0;
  while (getline(file, line)) {
    numOfLines++;
  }

  return numOfLines;
}

// This function saves data in studentsList in File
void saveStudentsInFile() {
  ofstream file(FILENAME);

  for (int i = 0; i < numOfStudents; i++) {
    Student student = studentsList[i];
    file << student.id << "," << student.name << "," << student.dept << ","
         << student.bill << "," << endl;
  }

  file.close();
}

void loadStudentsFromFile() {

  numOfStudents = countLines(FILENAME);

  studentsList = new Student[numOfStudents];

  ifstream file(FILENAME);
  string line;

  int lineNum = 0;
  while (getline(file, line)) {
    int i = 0;
    int start = 0;
    int pos = 0;
    Student student;
    while (line[i]) {
      if (line[i] == ',') {
        if (pos == 0) {
          student.id = stoi(line.substr(start, i - start));
        } else if (pos == 1) {
          student.name = line.substr(start, i - start);
        } else if (pos == 2) {
          student.dept = line.substr(start, i - start);
        } else if (pos == 3) {
          student.bill = stoi(line.substr(start, i - start));
        }
        start = i + 1;
        pos++;
      }
      i++;
    }
    studentsList[lineNum] = student;
    lineNum++;
  }
}

void markAttendance() {

  cout << "   ------------------------------------------" << endl;
  cout << "  |               Mark Attendance            |" << endl;
  cout << "   ------------------------------------------" << endl;

  int messPrice;

  cout << "What is the price of today mess?: ";
  cin >> messPrice;

  int select;

  bool *attendence = new bool[numOfStudents];
  for (int i = 0; i < numOfStudents; i++) {
    attendence[i] = false;
  }

  while (true) {
    // Displaying All Students
    for (int i = 0; i < numOfStudents; i++) {
      cout << left << setw(2) << i + 1 << "|" << setw(15)
           << studentsList[i].name << "|";
      if (attendence[i] == true) {
        cout << "P";
      }
      cout << endl;
    }
    cout << endl << "Press 0 to quit" << endl;
    int select = takeInput(-1, numOfStudents + 1);
    if (select == 0) {
      break;
    }
    if (attendence[select - 1] == true) {
      attendence[select - 1] = false;
    } else {
      attendence[select - 1] = true;
    }

    ClearScreen();

    cout << "   ------------------------------------------" << endl;
    cout << "  |               Mark Attendance            |" << endl;
    cout << "   ------------------------------------------" << endl;
    cout << "What is the price of today mess?: " << messPrice << endl;
  }

  // Adding today mess price in student bill
  for (int i = 0; i < numOfStudents; i++) {
    if (attendence[i] == true) {
      studentsList[i].bill += messPrice;
    }
  }

  // Getting today date
  time_t now = time(nullptr);
  tm tm = *localtime(&now);
  string date;
  date = to_string(tm.tm_mday) + "-" + to_string((tm.tm_mon + 1)) + "-" +
         to_string((tm.tm_year + 1900));

  // Saving all present student record in file
  for (int i = 0; i < numOfStudents; i++) {
    if (attendence[i]) {
      ofstream file("record/" + to_string(studentsList[i].id) + ".txt",
                    ios_base::app);
      file << date << " - " << messPrice << endl;
      file.close();
    }
  }

  saveStudentsInFile();
}

void viewRecord() {

  while (true) {
    ClearScreen();
    cout << "   ------------------------------------------" << endl;
    cout << "  |                 View Record              |" << endl;
    cout << "   ------------------------------------------" << endl;
    cout << endl;

    cout << "  ID | NAME           | BILL" << endl;

    for (int i = 0; i < numOfStudents; i++) {
      Student student = studentsList[i];
      cout << "  ";
      cout << left << setw(2) << student.id << " | " << setw(15) << student.name
           << "| " << student.bill << endl;
    }

    int response;
    cout << "\nEnter Student ID:";
    cin >> response;
    if (response == 0) {
      break;
    }

    ClearScreen();
    ifstream file("record/" + to_string(response) + ".txt");

    string line;
    while (getline(file, line)) {
      cout << line << endl;
    }

    cout << "\nEnter 0 to exit: ";
    int res;
    cin >> res;
  }
}

void addStudent() {
  Student *students;
  int n;
  cout << "How many student want to add?";
  cin >> n;
  students = new Student[n];
  for (int i = 0; i < n; i++) {
    cout << "Student " << i + 1 << ": " << endl;
    cout << "Enter ID: ";
    cin >> students[i].id;
    if (students[i].id == -1)
      break;
    cout << "Enter Name: ";
    cin >> students[i].name;
    cout << "Enter Department: ";
    cin >> students[i].dept;
    students[i].bill = 0;
  }
  ofstream file(FILENAME, ios_base::app);
  for (int i = 0; i < n; i++) {

    if (students[i].id == -1) {
      break;
    }
    file << students[i].id << "," << students[i].name << "," << students[i].dept
         << "," << students[i].bill << "," << endl;
    loadStudentsFromFile();
  }
  file.close();
  int response;
  cout << "Record added successfully." << endl;
  cout << "Press 0 to exit";
  cin >> response;
}

void removeStudent() {
  while (true){
    int id;

    cout << "Enter 0 to exit\n";
    cout << "Enter ID to remove: ";
    cin >> id;

    if (id==0){
      return;
    }
  
    int index = -1;
    for (int i = 0; i < numOfStudents; i++) {
      if (studentsList[i].id == id) {
        index = i;
        break;
      }
    }
  
    if (index == -1) {
      cout << "Record not found." << endl;
    }
    else{
      for (int i = index; i < numOfStudents - 1; i++) {
        studentsList[i] = studentsList[i + 1];
      }
    
      numOfStudents--;
    
      saveStudentsInFile();
      ClearScreen();
    }
  }
}

int main() {

  loadStudentsFromFile();

  while (true) {

    // Clears all text on console
    ClearScreen();
    cout << "   ------------------------------------------" << endl;
    cout << "  |               Mess Managment             |" << endl;
    cout << "   ------------------------------------------" << endl;

    cout << "  1. Mark Attendance" << endl;
    cout << "  2. View Record" << endl;
    cout << "  3. Add Student" << endl;
    cout << "  4. Remove Student" << endl;
    cout << "  5. Quit" << endl;
    cout << endl << endl;

    int option = takeInput(0, 6);

    ClearScreen();

    if (option == 1) {
      markAttendance();
    }
    else if (option == 2) {
      viewRecord();
    }
    else if (option == 3) {
      addStudent();
    } else if (option == 4) {
      removeStudent();
    }
    else if( option == 5) {
      break;
    }
  }
}