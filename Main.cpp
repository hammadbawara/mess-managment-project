#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
using namespace std;

int takeInput(int, int);
void ClearScreen() {
	cout << "\033[2J\033[1;1H";
}

struct Student {
	int id;
	string name;
	int bill;
};

Student* studentsList;
int numOfStudents;
const string FILENAME = "data.txt";

void saveStudentsInFile() {
	ofstream file(FILENAME);

	for (int i = 0; i < numOfStudents; i++) {
		Student student = studentsList[i];
		file << student.id << "," << student.name << "," << student.bill << ","<< endl;
	}

	file.close();
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

void loadStudentsFromFile(){

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
			cout << line[i];
			if (line[i] == ',') {
				if (pos == 0) {
					student.id = stoi(line.substr(start, i-start));
				}
				else if (pos == 1) {
					student.name = line.substr(start, i-start);
				}
				else if (pos == 2) {
					student.bill = stoi(line.substr(start, i-start));
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

	bool* attendence = new bool[numOfStudents];
	for (int i = 0; i < numOfStudents; i++) {
		attendence[i] = false;
	}


	while (true) {
		// Displaying All Students
		for (int i = 0; i < numOfStudents; i++) {
			cout << left << setw(2) << i+1 << "|" << setw(15) << studentsList[i].name
				<< "|";
			if (attendence[i] == true) {
				cout << "P";
			}
			cout << endl;
		}
		cout << endl << "Press 0 to quit" << endl;
		int select = takeInput(-1, numOfStudents+1);
		if (select == 0) {
			break;
		}
		if (attendence[select - 1] == true) {
			attendence[select - 1] = false;
		}
		else {
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

	saveStudentsInFile();
}

void viewRecord() {

	cout << "   ------------------------------------------" << endl;
	cout << "  |                 View Record              |" << endl;
	cout << "   ------------------------------------------" << endl;
	cout << endl;

	cout << "  ID | NAME           | BILL" << endl;

	for (int i = 0; i < numOfStudents; i++) {
		Student student = studentsList[i];
		cout << "  ";
		cout << left << setw(2) << student.id << " | " << setw(15) << student.name << "| " << student.bill << endl;
	}

	int response;
	cin >> response;
}

void addRemoveStudent() {
}

int main()
{

	loadStudentsFromFile();



	while (true) {

		// Clears all text on console
		ClearScreen();
		cout << "   ------------------------------------------" << endl;
		cout << "  |               Mess Managment             |" << endl;
		cout << "   ------------------------------------------" << endl;

		cout << "  1. Mark Attendance" << endl;
		cout << "  2. View Record" << endl;
		cout << "  3. Add / Remove Student" << endl;
		cout << "  4. Quit" << endl;
		cout << endl << endl;

		int option = takeInput(0, 5);

		ClearScreen();

		if (option == 1) {
			markAttendance();
		}

		else if (option == 2) {
			viewRecord();
		}

		else if (option == 3) {
			addRemoveStudent();
		}
		else if (option == 4) {
			break;
		}
	}

}

int takeInput(int start, int end) {
	int option;
	while (true) {
		cout << "Select: ";
		cin >> option;
		if (cin && option > start && option < end) {
			return option;
		}
		else {
			// remove old line
			cout << "\033[2A\033[3K";
			cout << option << " is not a valid option." << endl;
			// This clear previous entered value from cin buffer
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
	return 0;
}

